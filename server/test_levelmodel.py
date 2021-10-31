import unittest, datetime, os
from levelmodel import LevelModel
import sqlite3

TEST_DB = "test_pelletmeter.db"

class TestLevelModel(unittest.TestCase):
    """Unit tests for the levelmodel.py module."""

    @classmethod
    def setUpClass(cls):
        cls.ts = TimeStamp()

    def setUp(self):
        # Remove database file if tearDown wasn't run for some reason
        if os.path.exists(TEST_DB):
            os.remove(TEST_DB)
        # Initialize timestamp generator
        self.ts.reset()

    def tearDown(self):
        # Remove database file if created by test
        if os.path.exists(TEST_DB):
            os.remove(TEST_DB)

    def test_post(self):
        val = 45
        cut = LevelModel(self.ts.get, TEST_DB)
        # Run once as first post with brand new db is expected to be written
        # immediately which may clear the list of measurements.
        cut.post(0)
        # Post should return true
        self.assertTrue(cut.post(val))
        # Post should be last in measurements
        self.assertEqual(val, cut.measurements[-1])

    def test_post_wrong_type(self):
        cut = LevelModel(self.ts.get, TEST_DB)
        self.assertFalse(cut.post('{ "cm": 10 }'))
        self.assertFalse(cut.post(10.1))
        self.assertFalse(cut.post([1, 2, 3]))

    def test_post_boundaries(self):
        vals = [50, 39, 40, 200, 101, 100, 60]
        cut = LevelModel(self.ts.get, TEST_DB, raw_data_bounds=(40, 100))
        # Only these three are within given boundaries (and 50 is lost due to
        # measurements being cleared after writing first post when db is new).
        expected = [40, 100, 60]
        for val in vals:
            result = cut.post(val)
            if val < 40 or val > 100:
                self.assertFalse(result)
        self.assertListEqual(expected, cut.measurements)

    def test_read_writedb(self):
        cut = LevelModel(self.ts.get, TEST_DB)
        # Write two measurements one day apart.
        expected = [(self.ts.now(), 45), (self.ts.get(days=-1), 55)]
        cut.write_db(expected[0][0], expected[0][1])
        cut.write_db(expected[1][0], expected[1][1])
        # Exactly at oldest write. Should return both entries.
        self.assertListEqual(expected, cut.read_db(self.ts.get(days=-1)))
        # 1 second after oldest write. Should return only newest entry.
        self.assertListEqual([expected[0]], cut.read_db(self.ts.get(days=-1) + 1))
        # Exactly at newest write. Should return only newest entry.
        self.assertListEqual([expected[0]], cut.read_db(self.ts.now()))
        # 1 second after newest write. Should return only newest entry.
        self.assertEqual(0, len(cut.read_db(self.ts.now() + 1)))

    def test_post_store_and_read_first_post(self):
        # Expect read to respond with a list with (timestamp, value) pairs.
        expected = [(int(self.ts.now()), 45)]
        cut = LevelModel(self.ts.get, TEST_DB)
        cut.post(expected[0][1])
        actual = cut.read_db(self.ts.now() - 1)
        self.assertListEqual(expected, actual)

    def test_post_dont_write_more_than_once_every_hour(self):
        self.ts.timetravel(hours=-2)
        cut = LevelModel(self.ts.get, TEST_DB)
        # Post every 1/2 hour
        cut.post(45)
        self.ts.timetravel(minutes=30)
        cut.post(55)
        self.ts.timetravel(minutes=30)
        cut.post(65)
        # Only compare timestamps. Values gets their own test.
        read_timestamps = [row[0] for row in cut.read_db(self.ts.get(hours=-1, seconds=-1))]
        # Assert only hourly timestamps are present in db.
        self.assertListEqual([self.ts.get(hours=-1), self.ts.now()], read_timestamps)
        # Do another hour to be completely sure.
        self.ts.timetravel(minutes=30)
        cut.post(75)
        self.ts.timetravel(minutes=30)
        cut.post(85)
        read_timestamps = [row[0] for row in cut.read_db(self.ts.get(hours=-2, seconds=-1))]
        self.assertListEqual([self.ts.get(hours=-2), self.ts.get(hours=-1), self.ts.now()], read_timestamps)

    def test_post_store_mean_of_last_hours_measurements(self):
        # First hour includes post at T=0, which will be stored immediately when the database is empty.
        first_hour = [50, 45, 54, 36, 63, 46, 64]
        second_hour = [44, 55, 33, 66, 50, 40]
        self.ts.timetravel(hours=-2)
        cut = LevelModel(self.ts.get, TEST_DB)
        # Post every 10 minutes
        for val in first_hour:
            cut.post(val)
            self.ts.timetravel(minutes=10)
        mean_first_hour = int(round(sum(first_hour[1:]) / 6))
        values = [row[1] for row in cut.read_db(self.ts.get(hours=-1, minutes=-10, seconds=-1))]
        self.assertListEqual([50, mean_first_hour], values)
        # Do another hour
        for val in second_hour:
            cut.post(val)
            self.ts.timetravel(minutes=10)
        mean_second_hour = round(int(sum(second_hour) / 6))
        values = [row[1] for row in cut.read_db(self.ts.get(hours=-2, minutes=-10, seconds=-1))]
        self.assertListEqual([50, mean_first_hour, mean_second_hour], values)

    def test_delete_from_db(self):
        rows = [(self.ts.now(), 45), (self.ts.get(days=-1), 55), (self.ts.get(days=-31), 65), (self.ts.get(days=-365), 65)]
        cut = LevelModel(self.ts.get, TEST_DB)
        cut.write_db(*rows[0])
        cut.write_db(*rows[1])
        cut.write_db(*rows[2])
        cut.delete_db(self.ts.get(days=-1))
        self.assertListEqual(rows[:2], cut.read_db(self.ts.get(days=-366)))

    def test_post_delete_more_than_one_year_old_measurements(self):
        cut = LevelModel(self.ts.get, TEST_DB)
        self.ts.timetravel(days=-366)
        cut.post(45)
        self.ts.timetravel(days=1)
        cut.post(55)
        self.ts.reset()
        cut.post(65)
        actual = cut.read_db(self.ts.get(days=-367))
        expected = [(self.ts.get(days=-365), 55), (self.ts.now(), 65)]
        self.assertListEqual(expected, actual)

    def test_convert(self):
        cut = LevelModel(self.ts.get, TEST_DB)
        self.assertEqual(45, cut.convert(45))
        self.assertEqual(145, cut.convert(145))
        cut = LevelModel(self.ts.get, TEST_DB, raw_data_bounds=(40, 160), level_bounds=(10, 90))
        self.assertEqual(10, cut.convert(40))
        self.assertEqual(90, cut.convert(160))
        self.assertEqual(50, cut.convert(100))
        cut = LevelModel(self.ts.get, TEST_DB, raw_data_bounds=(10, 90), level_bounds=(160, 40))
        self.assertEqual(160, cut.convert(10))
        self.assertEqual(40, cut.convert(90))
        self.assertEqual(100, cut.convert(50))

    def test_get_for_period_24h(self):
        cut = LevelModel(self.ts.get, TEST_DB)
        # Post each hour for 25 hours.
        # We should get the last 24 posts.
        self.ts.timetravel(hours=-25)
        expected = []
        for i in range(25):
            cut.post(i)
            if i >= 1:
                expected.append((self.ts.now(), i))
            self.ts.timetravel(hours=1)
        self.ts.reset()
        self.assertListEqual(expected, cut.get_for_period(1))

    def test_get_for_period_7d(self):
        cut = LevelModel(self.ts.get, TEST_DB)
        # Post every hour for 24 * 8 hours.
        # We should get last 24 * 7 posts.
        self.ts.timetravel(days=-8)
        expected = []
        for i in range(8 * 24):
            cut.post(i)
            if i >= 24:
                expected.append((self.ts.now(), i))
            self.ts.timetravel(hours=1)
        self.ts.reset()
        self.assertListEqual(expected, cut.get_for_period(7))

    def test_get_for_period_converts(self):
        cut = LevelModel(self.ts.get, TEST_DB, raw_data_bounds=(40, 160), level_bounds=(10, 90))
        self.ts.timetravel(hours=-1)
        cut.post(40)
        self.ts.timetravel(hours=1)
        cut.post(160)
        expected = [(self.ts.get(hours=-1), 10), (self.ts.now(), 90)]
        self.assertListEqual(expected, cut.get_for_period(1))

class TimeStamp:
    """Class to produce fake timestamps where "now" is an absolute value."""

    TS = 1634760000 # 20211020, 22:00 (Why not?)

    def __init__(self):
        """Create a new TimeStamp object.
        
        After initialization now() will return TimeStamp.TS.
        """
        self.reset()

    def now(self):
        """Return a fake "now" timestamp."""
        return self._now.timestamp()

    def reset(self):
        """Reset now() to TimeStamp.TS."""
        self._now = datetime.datetime.fromtimestamp(self.TS)

    def timetravel(self, **kwargs):
        """Move now() relative to it's current value using arguments valid for datetime.timedelta."""
        self._now += datetime.timedelta(**kwargs)

    def get(self, **kwargs):
        """Return a timestamp relative now() using arguments valid for datetime.timedelta."""
        return (self._now + datetime.timedelta(**kwargs)).timestamp()

if __name__ == '__main__':
    unittest.main()