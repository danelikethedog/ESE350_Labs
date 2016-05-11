# -*- coding: utf-8 -*-
"""
    Flaskr
    ~~~~~~

    A microblog example application written as Flask tutorial with
    Flask and sqlite3.

    :copyright: (c) 2015 by Armin Ronacher.
    :license: BSD, see LICENSE for more details.
"""

import os
from sqlite3 import dbapi2 as sqlite3
from flask import Flask, request, session, g, redirect, url_for, abort, \
     render_template, flash, jsonify
import json
import collections
import time

# create our little application :)
app = Flask(__name__)

# Load default config and override config from an environment variable
app.config.update(dict(
    DATABASE=os.path.join(app.root_path, 'flaskr.db'),
    DEBUG=True,
    SECRET_KEY='development key',
    USERNAME='admin',
    PASSWORD='default'
))
app.config.from_envvar('FLASKR_SETTINGS', silent=True)

def connect_db():
    """Connects to the specific database."""
    rv = sqlite3.connect(app.config['DATABASE'])
    rv.row_factory = sqlite3.Row
    return rv


def init_db():
    """Initializes the database."""
    db = get_db()
    with app.open_resource('schema.sql', mode='r') as f:
        db.cursor().executescript(f.read())
    db.commit()


@app.cli.command('initdb')
def initdb_command():
    """Creates the database tables."""
    init_db()
    print('Initialized the database.')


def get_db():
    """Opens a new database connection if there is none yet for the
    current application context.
    """
    if not hasattr(g, 'sqlite_db'):
        g.sqlite_db = connect_db()
    return g.sqlite_db


@app.teardown_appcontext
def close_db(error):
    """Closes the database again at the end of the request."""
    if hasattr(g, 'sqlite_db'):
        g.sqlite_db.close()


@app.route('/update_db')
def show_entries():
    db = get_db()
    cur = db.execute('select id, plant, set_temperature, set_water, set_light from entries order by id')
    entries = cur.fetchall()
    while(len(entries) < 4):
	    entries.append(None)
    return render_template('show_entries.html', entries=entries)

@app.route('/')
def html_dev():
	db = get_db()
	cur = db.execute('select id, plant, current_water, current_light, set_water, set_light, light_on from entries order by id')
	plants = cur.fetchall()
	cur = db.execute('select current_temperature, current_humidity from planter where id=0')
	planter = cur.fetchall()
	return render_template('index-new.html', entries=plants, planter=planter)
	
@app.route('/plant', methods=['POST'])
def add_plant():
	if not session.get('logged_in'):
		abort(401)
	db = get_db()
	db.execute('insert into entries(id, plant) values (?,?)', [request.form['id'], request.form['plant']])
	db.commit()
	return redirect(url_for('html_dev'))

@app.route('/add', methods=['POST'])
def add_entry():
    db = get_db()
    db.execute('insert into entries (plant, set_temperature, set_water, set_light) values (?, ?, ?, ?)',
               [request.form['plant'], request.form['set_temperature'], request.form['set_water'], request.form['set_light']])
    db.commit()
    flash('New entry was successfully posted')
    return redirect(url_for('show_entries'))

@app.route('/delete/<id>')
def delete_plant(id):
	if not session.get('logged_in'):
		abort(401)
	db = get_db()
	cur = db.execute('delete from entries where id=?', id)
	db.commit()
	return redirect(url_for('html_dev'))
	
@app.route('/login', methods=['GET', 'POST'])
def login():
    error = None
    if request.method == 'POST':
        if request.form['username'] != app.config['USERNAME']:
            error = 'Invalid username'
        elif request.form['password'] != app.config['PASSWORD']:
            error = 'Invalid password'
        else:
            session['logged_in'] = True
            flash('You were logged in')
            return redirect(url_for('html_dev'))
    return render_template('login.html', error=error)

@app.route('/update', methods=['POST'])
def update_plant():
	if not session.get('logged_in'):
		abort(401)
	db = get_db()
	db.execute('update entries set set_water=?, set_light=? where id=?', [request.form['set_water'],request.form['set_light'],request.form['id']])
	db.commit()
	
	return redirect(url_for('html_dev'))
	
def query_to_dict(rows):
	d = collections.OrderedDict();
	d["numRows"] = len(rows)
	for i, row in enumerate(rows):
		row_keys = row.keys()
		l = []
		dict = {}
		for col in range(0, len(row)):
			dict[row_keys[col]] = row[col]
		l.append(dict)
		d[l[0]['id']] = l
	return d
	
@app.route('/api/get/<item>')
def get_json(item):
	db = get_db()
	if(item == 'plants'):
		curr = db.execute('select * from entries')
	elif(item == 'planter'):
		curr = db.execute('select * from planter')
	else:
		abort(400)
	rows = curr.fetchall()
	return jsonify(query_to_dict(rows))
	
	
@app.route('/api/update/<id>/current_light=<current_light>&current_water=<current_water>')
def update_water_light(id, current_light, current_water):
	db = get_db()
	curr = db.execute('update entries set current_light=?, current_water=? where id=?', [current_light, current_water,id])
	db.commit()
	curr = db.execute('select * from entries where id=?', [id])
	row = curr.fetchone()
	cur_water = row[3]
	cur_light = row[4]
	timestamp = str(time.time())
	table_name = "plot_" + id + "_history"
	db_command = "insert into " + table_name + " (timedate, water, light) VALUES (?,?,?)"
	curr = db.execute(db_command, [timestamp,cur_water,cur_light])
	db.commit()
	return redirect(url_for('get_json', item='plants'))

@app.route('/api/update/<id>/light=<int:light_on>')
def update_plant_lights(id,light_on):
	if (not(light_on == 0 or light_on == 1)):
		abort(400)
	db = get_db()
	curr = db.execute('update entries set light_on=? where id=?', [bool(light_on),id])
	db.commit()
	return redirect(url_for('get_json',item='plants'))

@app.route('/api/update/planter/current_temperature=<current_temperature>&current_humidity=<current_humidity>')
def update_planter(current_temperature,current_humidity):
	db = get_db()
	curr = db.execute('select * from planter')
	if(curr.fetchone() is None):
		curr = db.execute('insert into planter (id, current_temperature, current_humidity) VALUES (0, ?,?)', [current_temperature, current_humidity])
	else:
		curr = db.execute('update planter set current_temperature=?, current_humidity=? where id=0', [current_temperature, current_humidity])
	db.commit()
	curr = db.execute('select * from planter where id=?', [0])
	row = curr.fetchone()
	cur_temp = row[1]
	cur_humidity = row[2]
	timestamp = str(time.time())
	db_command = "insert into planter_history (timedate,humidity,temperature) VALUES (?,?,?)"
	curr = db.execute(db_command, [timestamp,cur_humidity,cur_temp])
	db.commit()
	return redirect(url_for('get_json',item='planter'))
	
@app.route('/history')
def history():
	db = get_db()
	curr = db.execute('select * from planter_history')
	planter_history = curr.fetchall()
	curr = db.execute('select * from plot_0_history')
	plot_0_history = curr.fetchall()
	curr = db.execute('select * from plot_1_history')
	plot_1_history = curr.fetchall()
	curr = db.execute('select * from plot_2_history')
	plot_2_history = curr.fetchall()
	curr = db.execute('select * from plot_3_history')
	plot_3_history = curr.fetchall()
	
	return render_template('history.html', planter_history=planter_history, plot_0_history = plot_0_history, plot_1_history = plot_1_history, plot_2_history = plot_2_history, plot_3_history = plot_3_history)
	
@app.route('/remote')
def remote():
	return render_template('remote.html')
	
if __name__ == '__main__':
	app.run(host='0.0.0.0', debug=False, port=5000)

