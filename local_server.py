from fastapi import FastAPI
from pydantic import BaseModel
import uvicorn
import sqlalchemy as db

app = FastAPI()

engine = db.create_engine("sqlite:///users_database.db")
con = engine.connect()
metadata = db.MetaData()

authorized_users = db.Table("authorized_users", metadata, autoload_with=engine)
access_history = db.Table("access_history", metadata, autoload_with=engine)

data = con.execute(db.select(authorized_users)).fetchall()

keys = [list(map(int, row[2].split())) for row in data]


class IncomingData(BaseModel):
    # timestamp: str
    key: list = []


@app.post("/")
def post_req(payload: IncomingData):
    key = payload.key
    print(f"User with key: {key} used door")
    query = db.insert(access_history).values(key=" ".join(map(str, key)))
    con.execute(query)
    con.commit()


@app.get("/")
def get_req():
    return {"keys": keys}


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=80)
