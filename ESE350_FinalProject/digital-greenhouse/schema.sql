drop table if exists entries;
create table entries (
  id integer primary key autoincrement,
  plant text not null,
  temperature integer,
  light_on boolean,
  water integer,
  light_intensity integer
);
