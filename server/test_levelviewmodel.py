import unittest, sys, datetime
from levelviewmodel import LevelViewModel

class TestLevelViewModel(unittest.TestCase):
    """Unit tests for the levelviewmodel.py module."""

    @classmethod
    def setUpClass(cls):
        cls.level_model = FakeLevelModel()

    def setUp(self):
        self.level_model.reset()

    def test_post(self):
        cut = LevelViewModel(self.level_model, 60, 120)
        self.assertTrue(cut.post(self.json(100)))
        self.assertEqual(100, self.level_model.posted)

    def test_post_malformed_json(self):
        cut = LevelViewModel(self.level_model, 60, 120)
        self.assertFalse(cut.post('{ "dm": 100 }'))
        self.assertFalse(cut.post('{ cm: 100 }'))
        self.assertFalse(cut.post(100))

    def test_post_returns_false_if_levelmodel_post_failes(self):
        cut = LevelViewModel(self.level_model, 60, 120)
        self.level_model.post_return_value = False
        self.assertFalse(cut.post(self.json(100)))

    def test_get_24h(self):
        cut = LevelViewModel(self.level_model, 60, 120)
        # Create a fake db with 24h of posts
        yesterday = int(datetime.datetime(2021, 10, 27).timestamp())
        self.level_model.db = [(yesterday + 3600 * (i + 1), 65 - i % 10) for i in range(24)]

        expected_x_axis, expected_y_axis = self.get_expected_from_db()
        expected_period = '24 hours'

        cut.get_24h()
        self.assertEqual(expected_x_axis, cut.x_axis)
        self.assertEqual(expected_y_axis, cut.y_axis)
        self.assertEqual(expected_period, cut.period)

    def test_get_24h_less_than_24_records(self):
        cut = LevelViewModel(self.level_model, 60, 120)
        # Create a fake db with 4 posts
        ts = int(datetime.datetime(2021, 10, 27).timestamp())
        self.level_model.db = [(ts + 3600, 60), (ts + 3600 * 2, 59), (ts + 3600 * 3, 50), (ts + 3600 * 3, 65)]

        expected_x_axis, expected_y_axis = self.get_expected_from_db()
        expected_period = '24 hours'

        cut.get_24h()
        self.assertEqual(expected_x_axis, cut.x_axis)
        self.assertEqual(expected_y_axis, cut.y_axis)
        self.assertEqual(expected_period, cut.period)

    def test_get_7h(self):
        cut = LevelViewModel(self.level_model, 60, 120)
        # Create a fake db with 7 * 24h of posts
        a_week_ago = int(datetime.datetime(2021, 10, 20).timestamp())
        self.level_model.db = [(a_week_ago + 3600 * (i + 1), 65 - i % 10) for i in range(24 * 7)]

        expected_x_axis, expected_y_axis = self.get_expected_from_db()
        expected_period = '7 days'

        cut.get_7d()
        self.assertEqual(expected_x_axis, cut.x_axis)
        self.assertEqual(expected_y_axis, cut.y_axis)
        self.assertEqual(expected_period, cut.period)

    def test_get_30d(self):
        cut = LevelViewModel(self.level_model, 60, 120)
        # Create a fake db with 30 posts over 30 days
        a_month_ago = int(datetime.datetime(2021, 10, 1).timestamp())
        self.level_model.db = [(a_month_ago + 24 * 3600 * (i + 1), 65 - i % 10) for i in range(30)]

        expected_x_axis, expected_y_axis = self.get_expected_from_db()
        expected_period = '30 days'

        cut.get_30d()
        self.assertEqual(expected_x_axis, cut.x_axis)
        self.assertEqual(expected_y_axis, cut.y_axis)
        self.assertEqual(expected_period, cut.period)

    def test_get_365d(self):
        cut = LevelViewModel(self.level_model, 60, 120)
        # Create a fake db with 365 posts over 365 days
        a_year_ago = int(datetime.datetime(2020, 10, 1).timestamp())
        self.level_model.db = [(a_year_ago + 24 * 3600 * (i + 1), 65 - i % 10) for i in range(365)]

        expected_x_axis, expected_y_axis = self.get_expected_from_db()
        expected_period = '365 days'

        cut.get_365d()
        self.assertEqual(expected_x_axis, cut.x_axis)
        self.assertEqual(expected_y_axis, cut.y_axis)
        self.assertEqual(expected_period, cut.period)

    def test_no_records(self):
        cut = LevelViewModel(self.level_model, 60, 120)
        fut = [cut.get_24h, cut.get_7d, cut.get_30d, cut.get_365d]
        expected_periods = ['24 hours', '7 days', '30 days', '365 days']
        self.level_model.db = []
        for i in range(len(fut)):
            fut[i]()
            self.assertEqual('', cut.x_axis)
            self.assertEqual('', cut.y_axis)
            self.assertEqual(expected_periods[i], cut.period)
            self.assertEqual('NONE', cut.current_level)
            self.assertEqual('alert-dark', cut.bs_class)

    def test_current_level(self):
        cut = LevelViewModel(self.level_model, 40, 90)
        fut = [cut.get_24h, cut.get_7d, cut.get_30d, cut.get_365d]
        for i in range(len(fut)):
            ts = int(datetime.datetime(2021, 10, 27).timestamp())
            self.level_model.db = [(ts + 3600 * 2, 50), (ts + 3600 * 2, 91), (ts + 3600 * 3, 90), (ts + 3600 * 4, 40), (ts + 3600 * 5, 39)]
            fut[i]()
            self.assertEqual('LOW', cut.current_level)
            self.assertEqual('alert-danger', cut.bs_class)
            self.level_model.db = self.level_model.db[:-1]
            cut.get_24h()
            self.assertEqual('MEDIUM', cut.current_level)
            self.assertEqual('alert-warning', cut.bs_class)
            self.level_model.db = self.level_model.db[:-1]
            cut.get_24h()
            self.assertEqual('MEDIUM', cut.current_level)
            self.assertEqual('alert-warning', cut.bs_class)
            self.level_model.db = self.level_model.db[:-1]
            cut.get_24h()
            self.assertEqual('HIGH', cut.current_level)
            self.assertEqual('alert-success', cut.bs_class)

    def test_static_attributes(self):
        level_model = FakeLevelModel(level_bounds=(10, 100))
        cut = LevelViewModel(level_model, 30, 80)
        self.assertEqual(10, cut.level_min)
        self.assertEqual(100, cut.level_max)
        self.assertEqual(30, cut.level_low_limit)
        self.assertEqual(80, cut.level_high_limit)

    def test_min_max_when_inverted_level_bounds(self):
        level_model = FakeLevelModel(level_bounds=(100, 10))
        cut = LevelViewModel(level_model, 30, 80)
        self.assertEqual(10, cut.level_min)
        self.assertEqual(100, cut.level_max)

    def get_expected_from_db(self):
        """Return expected attributes from the contents of self.level_model.db."""
        expected_datetimes = [datetime.datetime.fromtimestamp(d[0]) for d in self.level_model.db]
        expected_x_axis = ','.join([f"'{i.strftime('%Y-%m-%d %H:%M')}'" for i in expected_datetimes])
        expected_y_axis = ','.join([str(i[1]) for i in self.level_model.db])
        return expected_x_axis, expected_y_axis

    @staticmethod
    def json(val):
        """Return val as valid json for the post function."""
        return f'{{\n  "cm": {val}\n}}'

class FakeLevelModel:
    """Fake class for spying on operations on a LevelModel instance."""

    def __init__(self, ts_func=None, db_name=None, raw_data_bounds=(0, sys.maxsize), level_bounds=(0, sys.maxsize)):
        self.reset()
        self.level_bounds = level_bounds

    def reset(self):
        self.posted = None
        self.post_return_value = True
        self.db = None

    def post(self, measurement):
        self.posted = measurement
        self.db = []
        return self.post_return_value

    def get_for_period(self, days):
        return self.db

if __name__ == '__main__':
    unittest.main()