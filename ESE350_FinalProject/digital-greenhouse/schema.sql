drop table if exists entries;
create table entries (
  id integer primary key,
  plant text not null,
  current_temperature integer,
  light_on boolean,
  current_water integer,
  current_light integer, --light intensity
  set_temperature integer,
  set_water integer,
  set_light integer
  
);
