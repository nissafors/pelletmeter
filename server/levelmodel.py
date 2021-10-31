import sqlite3, os, statistics, sys, datetime

def level_model_ts_func(**kwargs):
    """Return a timestamp relative now using arguments valid for datetime.timedelta."""
    return (datetime.datetime.now() + datetime.timedelta(**kwargs)).timestamp()


class LevelModel:
    """Store, convert and read back measured levels.
    
    Measurements should be posted on regular intervals shorter than one hour, e.g. every 1 or 10 minutes.
    These are stored as raw distance data, but will can read back converted to "levels".
    """

    TABLE_NAME = 'measurements'
    ROW_TS = 'timestamp'
    ROW_CM = 'centimeters'

    def __init__(self, ts_func=level_model_ts_func, db_name='pelletmeter.db', raw_data_bounds=(0, sys.maxsize), level_bounds=(0, sys.maxsize)):
        """Create a new LevelModel
        
        Args:
            ts_func (callable(int)):           Function that returns a Posix timestamps from datetime.timedelta parameters.
                                               Example: ts_func(hours=-365) should return a Posix timestamp from 1 year ago.
            db_name (str):                     Name of database file. Will be created if it doesn't exist.
            raw_data_bounds (tuple(int, int)): Lower and upper limit for measurements. Posts outside raw_data_bounds will
                                               be ignored.
            level_bounds (tuple(int, int)):    Levels corresponding to raw_data_bounds lower and upper limits respectively.
        """
        self.db_name = db_name
        self.new_db = not os.path.exists(self.db_name)
        if self.new_db:
            self.init_db()
        self.measurements = []
        self.ts_func = ts_func
        self.raw_data_bounds = raw_data_bounds
        self.level_bounds = level_bounds

    def post(self, measurement):
        """Process posted measurements and store in database.

        First post after a new database is stored. After that post() only writes to db if there are no post within the last
        hour. It then writes the average of all measurements that was posted during that hour, using the timestamp from the
        last post. Measurements older than one year are deleted from the database.
        
        Args:
            measurement (int): The measurement to post.

        Returns:
            bool: True if post was succesful, False otherwise.
        """
        # Get timestamp
        timestamp = self.ts_func(seconds=0)

        # Error checking
        if not isinstance(measurement, int):
            return False
        if measurement < self.raw_data_bounds[0] or measurement > self.raw_data_bounds[1]:
            return False

        self.measurements.append(measurement)

        # Check to see if there are any records for the last hour (or if the db is brand new)
        if self.new_db or not self.read_db(self.ts_func(hours=-1, seconds=1)):
            # Store mean of all measurements and then clear the list
            to_store = measurement if len(self.measurements) == 1 else int(round(statistics.mean(self.measurements)))
            self.write_db(timestamp, to_store)
            self.new_db = False
            self.measurements.clear()
            self.delete_db(self.ts_func(days=-365))
        return True

    def get_for_period(self, days):
        """Return converted records for each hour for the given period.
        
        Args:
            days (int): Period in days.

        Returns:
            list of tuples (int, int): Timestamp and level at each hour for the period.
        """
        records = self.read_db(self.ts_func(days=-days))
        converted = [(r[0], self.convert(r[1])) for r in records]
        return converted

    def convert(self, measurement):
        """Calculate level from raw measurement data.
        
        Args:
            measurement (int): Raw measurement data within raw_data_bounds.

        Returns:
            int: The calculated level within level_bounds.
        """
        return int(round((self.level_bounds[1] - self.level_bounds[0]) * (measurement - self.raw_data_bounds[0]) / (self.raw_data_bounds[1] - self.raw_data_bounds[0]) + self.level_bounds[0]))

    def init_db(self):
        """Create a new database."""
        db = sqlite3.connect(self.db_name)
        cur = db.cursor()
        cur.execute(f'CREATE TABLE {self.TABLE_NAME} ({self.ROW_TS} INT, {self.ROW_CM} INT)')
        db.commit()
        db.close()

    def write_db(self, timestamp, measurement):
        """Write a row (timestamp, measurement) to the database.
        
        Args:
            timestamp (int): Epoch timestamp for measurement.
            measurement (int): The measurement.
        """
        timestamp = int(timestamp)
        db = sqlite3.connect(self.db_name)
        cur = db.cursor()
        cur.execute(f'INSERT INTO {self.TABLE_NAME} VALUES (?, ?)', (timestamp, measurement))
        db.commit()
        db.close()

    def read_db(self, timestamp_from, timestamp_to=None):
        """Returns a list of all (timestamp, measurement) pairs after given time.
        
        Args:
            timestamp_from (int): Epoch timestamp to select from.
            timestamp_to (int):   Epoch timestamp to select up to.
        
        Returns:
            list of tuple (int, int): The selected data.
        """
        timestamp_from = int(timestamp_from)
        if not timestamp_to:
            timestamp_to = int(self.ts_func(seconds=0))
        db = sqlite3.connect(self.db_name)
        cur = db.cursor()
        cur.execute(f'SELECT * FROM {self.TABLE_NAME} WHERE {self.ROW_TS} >= {timestamp_from} AND {self.ROW_TS} <= {timestamp_to}')
        result = cur.fetchall()
        db.close()
        return result

    def delete_db(self, timestamp):
        """Delete all items older than given time.
        
        Args:
            timestamp (int): Epoch timestamp to delete before.
        """
        timestamp = int(timestamp)
        db = sqlite3.connect(self.db_name)
        cur = db.cursor()
        # Start by deleting measurements that are > 365 days old:
        cur.execute(f'DELETE FROM {self.TABLE_NAME} WHERE {self.ROW_TS} < {timestamp}')
        db.commit()
        db.close()
