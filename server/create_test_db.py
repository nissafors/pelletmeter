"""Create a database file with one year of fake data that can be used to view the site."""

import sqlite3, datetime
from levelmodel import LevelModel

level_model = LevelModel(raw_data_bounds=(40, 130))
time = datetime.datetime.now() - datetime.timedelta(days=365)
i = 0
posts = []
for d in range(365):
    for h in range(24):
        val = (131 - 40 + i) % (131 - 40) + 40
        posts.append((int(time.timestamp()), val))
        i += 1
        time += datetime.timedelta(hours=1)
db = sqlite3.connect(level_model.db_name)
cur = db.cursor()
cur.executemany(f'INSERT INTO {level_model.TABLE_NAME} VALUES (?, ?)', posts)
db.commit()
db.close()
