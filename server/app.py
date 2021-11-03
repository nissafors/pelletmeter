__copyright__ = "Copyright (C) 2021 Andreas Andersson"

from flask import Flask, request, render_template
from levelmodel import LevelModel
import http, settings
from levelviewmodel import LevelViewModel

app = Flask(__name__)
level_view_model = LevelViewModel(LevelModel(raw_data_bounds=settings.measurement_bounds, level_bounds=settings.level_bounds), settings.level_low, settings.level_high)

def log(msg):
    if settings.verbose:
        print(msg)

@app.route("/", methods=['GET', 'POST'])
def home():
    if request.method == 'GET':
        level_view_model.get_24h()
        return render_template('home.html', lvm=level_view_model)
    elif request.method == 'POST':
        body = request.data.decode('ascii')
        log(body)
        result = level_view_model.post(body)
        log(level_view_model.level_model.measurements)
        code = http.HTTPStatus.NO_CONTENT if result else http.HTTPStatus.BAD_REQUEST
        return ('', code)
 
@app.route("/last24h")
def last_24h():
        level_view_model.get_24h()
        return render_template('home.html', lvm=level_view_model)

@app.route("/last7d")
def last_7d():
        level_view_model.get_7d()
        return render_template('home.html', lvm=level_view_model)

@app.route("/last30d")
def last_30d():
        level_view_model.get_30d()
        return render_template('home.html', lvm=level_view_model)

@app.route("/last365d")
def last_365d():
        level_view_model.get_365d()
        return render_template('home.html', lvm=level_view_model)
