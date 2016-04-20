drop table if exists entries;
create table entries (
  id integer primary key autoincrement,
  plant text not null,
  temperature text not null,
  -- light_on text not null,
  water text not null,
  light_intensity text not null
);
