drop table if exists entries;
create table entries (
  id integer primary key,
  plant text not null,
  light_on boolean,
  current_water integer,
  current_light integer, --light intensity
  set_temperature integer,
  set_water integer,
  set_light integer
  
);
drop table if exists planter;
create table planter (
	id integer primary key,
	current_temperature integer,
	current_humidity integer,
	optimal_temperature integer,
	optimal_humidity integer
);
drop table if exists plot_0_history;
create table plot_0_history (
	timedate text primary key,
	water integer,
	light integer
);
drop table if exists plot_1_history;
create table plot_1_history (
	timedate text primary key,
	water integer,
	light integer
);
drop table if exists plot_2_history;
create table plot_2_history (
	timedate text primary key,
	water integer,
	light integer
);
drop table if exists plot_3_history;
create table plot_3_history (
	timedate text primary key,
	water integer,
	light integer
);
drop table if exists planter_history;
create table planter_history (
	timedate text primary key,
	humidity integer,
	temperature integer
);