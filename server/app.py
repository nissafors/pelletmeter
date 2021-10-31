from flask import Flask, request, render_template
from levelmodel import LevelModel
from werkzeug.wrappers import response
import http

from levelviewmodel import LevelViewModel

app = Flask(__name__)
level_view_model = LevelViewModel(LevelModel(raw_data_bounds=(40, 130), level_bounds=(90, 0)), 30, 60)

@app.route("/", methods=['GET', 'POST'])
def home():
    if request.method == 'GET':
        level_view_model.get_24h()
        return render_template('home.html', lvm=level_view_model)
    elif request.method == 'POST':
        body = request.data.decode('ascii')
        result = level_view_model.post(body)
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
