__copyright__ = "Copyright (C) 2021 Andreas Andersson"

import json, datetime
from json.decoder import JSONDecodeError
from levelmodel import LevelModel

class LevelViewModel:
    """View model class for LevelModel.
    
    Attributes:
        level_min (int): The minimum level.
        level_max (int): The maximum level.
        level_low_limit (int): Levels below this value will be considerered low.
        level_high_limit (int): Levels above this value will be considerered high.
        x_axis (str): Dates formatted for a plotly x axis.
        y_axis (str): Levels formatted fo a plotly y axis.
        period (str): The current period.
        current_level (str): The current level (HIGH, MEDIUM, LOW or NONE if there are no records for the current period).
        bs_class (str): Bootstrap alert class.
    """

    def __init__(self, level_model: LevelModel, level_low_limit: int, level_high_limit: int):
        """Initialize a new LevelViewModel.
        
        Args:
            level_model (LevelModel): The LevelModel object to wrap.
            level_low_limit (int): Sets the level_low_limit attribute.
            level_high_limit (int): Sets the level_high_limit attribute.
        """
        self.level_model = level_model
        self.level_min = min(level_model.level_bounds[0], level_model.level_bounds[1])
        self.level_max = max(level_model.level_bounds[0], level_model.level_bounds[1])
        self.level_low_limit = level_low_limit
        self.level_high_limit = level_high_limit
        self.x_axis = ''
        self.y_axis = ''
        self.period = ''
        self.current_level = 'NONE'
        self.bs_class = 'alert-success'

    def post(self, json_data: str) -> bool:
        """Post a new measurement.

        Args:
            json_data (str): Expected format is: { "cm": <value> }.

        Returns:
            True if post was successful, False otherwise.
        """
        try:
            data = json.loads(json_data)
            measurement = int(data['cm'])
        except KeyError:
            return False
        except JSONDecodeError:
            return False
        except TypeError:
            return False
        return self.level_model.post(measurement)

    def get_24h(self):
        """Get records for the last 24 hours.

        Sets the following attributes: x_axis, y_axis, period, current_level and bs_class.
        """
        records = self.level_model.get_for_period(1)
        self.set_attributes(records, '24 hours')

    def get_7d(self):
        """Get records for the last 7 days.

        Sets the following attributes: x_axis, y_axis, period, current_level and bs_class.
        """
        records = self.level_model.get_for_period(7)
        self.set_attributes(records, '7 days')

    def get_30d(self):
        """Get day averages for the last 30 days.

        Sets the following attributes: x_axis, y_axis, period, current_level and bs_class.
        """
        records = self.level_model.get_for_period(30)
        self.set_attributes(records, '30 days')

    def get_365d(self):
        """Get day averages for the last 365 days.

        Sets the following attributes: x_axis, y_axis, period, current_level and bs_class.
        """
        records = self.level_model.get_for_period(365)
        self.set_attributes(records, '365 days')

    def set_attributes(self, records, period):
        """Set x_axis, y_axis, period, current_level and bs_class.

        Args:
            records (list of tuple(int, int)): List of timestamps and levels.
            period (str): Period for records as a human readable string.
        """
        self.period = period
        dates = [f"'{datetime.datetime.fromtimestamp(r[0]).strftime('%Y-%m-%d %H:%M')}'"for r in records]
        self.x_axis = ','.join(dates)
        self.y_axis = ','.join([str(x[1]) for x in records])
        if len(records) == 0:
            self.current_level = 'NONE'
            self.bs_class = 'alert-dark'
        elif records[-1][1] > self.level_high_limit:
            self.current_level = 'HIGH'
            self.bs_class = 'alert-success'
        elif records[-1][1] < self.level_low_limit:
            self.current_level = 'LOW'
            self.bs_class = 'alert-danger'
        else:
            self.current_level = 'MEDIUM'
            self.bs_class = 'alert-warning'
