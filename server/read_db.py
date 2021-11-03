__copyright__ = "Copyright (C) 2021 Andreas Andersson"

"""Read and print all items from the database."""

import datetime, settings
from levelmodel import LevelModel

level_model = LevelModel(raw_data_bounds=settings.measurement_bounds, level_bounds=settings.level_bounds)
time = datetime.datetime.now() - datetime.timedelta(days=400)
records = level_model.read_db(time.timestamp())
for r in records:
    print(f"Time: {datetime.datetime.fromtimestamp(r[0]).strftime('%Y-%m-%d %H:%M')}, Value: {r[1]}, Converted: {level_model.convert(r[1])}")