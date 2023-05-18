from typing import Any, List
from fastapi import FastAPI,WebSocket
from sqlalchemy import Column, Float,create_engine,Integer,String,func,inspect
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from pydantic import BaseModel
import json

app = FastAPI()

class Energy_scheme(BaseModel):
    id:int
    time:str
    energy_data:float
    accenergy:float
    current:float
    voltage:float

    class Config:
        orm_mode=True

Base = declarative_base()

class Energy(Base):

    __tablename__ = "energy"

    id = Column(Integer,primary_key=True)
    time = Column("time", String )
    energy_data = Column("energy_data", Float),
    accenergy = Column("accenergy", Float)
    current = Column("current",Float)
    voltage= Column("voltage",Float)

    def __init__(self,time,energy,accenergy,current,voltage):
        # self.id=id
        self.time=time
        self.energy_data=energy
        self.accenergy=accenergy
        self.current=current
        self.voltage=voltage

        


SQL_DATABASE_URL = "sqlite:///./Energy.db"

engine = create_engine(SQL_DATABASE_URL,echo=True,connect_args={"check_same_thread": False})
Session_db = sessionmaker(autocommit=False,autoflush=False,bind=engine)
session =Session_db()
Base.metadata.create_all(bind=engine)

count = session.query(func.count(Energy.id)).scalar()
print(count)
session.commit()
 


@app.get("/data/")
async def getData():
    data = session.query(Energy).all()
    return data

@app.websocket("/ws")
async def websocket_endpoint(websocket:WebSocket):
    await websocket.accept()
    while True :
        try:
            # Get the latest data from the database
            # sendDataFromDB()
            with Session_db() as session:
                data = session.query(Energy).all()
                for d in data:
                    await websocket.send_json(json.dumps(object_as_dict(d)))

            # # Send the data to the websocket client
            # await websocket.send_json(json.dumps(data))

        except Exception as e:
            print(e)
            break
        #  await websocket.send_text("helo")


def object_as_dict(obj):
    return {c.key: getattr(obj, c.key)
            for c in inspect(obj).mapper.column_attrs}


count = 0
def sendDataFromDB():
    with Session_db() as session:
        data = session.query(Energy).all()
        for row in data:
            
            print(object_as_dict(row))

