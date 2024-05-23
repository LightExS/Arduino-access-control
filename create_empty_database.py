from sqlalchemy import create_engine
from sqlalchemy import Table, Column, Integer, String, MetaData

meta = MetaData()
engine = create_engine("sqlite:///users_database.db", echo=True)

authorized_users = Table(
    "authorized_users",
    meta,
    Column("id", Integer, primary_key=True),
    Column("name", String),
    Column("key", String, unique=True, nullable=False),
)

access_history = Table(
    "access_history",
    meta,
    Column("id", Integer, primary_key=True),
    Column("key", String, nullable=False),
)

meta.create_all(engine)
