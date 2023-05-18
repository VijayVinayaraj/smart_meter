import websockets
import asyncio
import json

async def listen():
    url ="ws://localhost:8000/ws"
    async with websockets.connect(url) as websocket :
        while True:
        
            message = await websocket.recv()
            
            print(json.loads(message))


asyncio.get_event_loop().run_until_complete(listen())
