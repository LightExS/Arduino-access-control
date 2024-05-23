from fastapi import Request, FastAPI
from pydantic import BaseModel
import uvicorn
import json

app = FastAPI()

database = [
    [51, 125, 33, 248],
    [24, 99, 5, 7],
    [240, 59, 24, 80],
    [0, 42, 255, 93],
]


class IncomingData(BaseModel):
    # timestamp: str
    key: list = []


class OutgoingData(BaseModel):
    result: str


@app.post("/", response_model=OutgoingData)
def post_req(payload: IncomingData):
    data = payload.key
    print(data)
    return {"result": "post_success"}


@app.get("/")
def get_req():
    return {"keys": database, "size": len(database)}


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=80)
