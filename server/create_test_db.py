__copyright__ = "Copyright (C) 2021 Andreas Andersson"

"""Create a database file with one year of fake data that can be used to view the site."""

import sqlite3, datetime, settings
from levelmodel import LevelModel

level_model = LevelModel(raw_data_bounds=settings.measurement_bounds, level_bounds=settings.level_bounds)
time = datetime.datetime.now() - datetime.timedelta(days=365)
i = 0
posts = []
for d in range(365):
    for h in range(24):
        val = (settings.measurement_bounds[0] - settings.measurement_bounds[1] + i) % (settings.measurement_bounds[0] - settings.measurement_bounds[1]) + settings.measurement_bounds[1]
        posts.append((int(time.timestamp()), val))
        i += 1
        time += datetime.timedelta(hours=1)
db = sqlite3.connect(level_model.db_name)
cur = db.cursor()
cur.executemany(f'INSERT INTO {level_model.TABLE_NAME} VALUES (?, ?)', posts)
db.commit()
db.close()
