
import asyncio
from pymongo import AsyncMongoClient
import datetime
# pip3 install pymongo --break-system-packages

async def main():
    mongo_url = "mongodb://localhost:27017/"
    my_client = AsyncMongoClient(mongo_url)
    office = "office"
    office_db = my_client[office]
    environment_sampling = "environment_sampling"
    environment_sampling_collection = office_db[environment_sampling]

    # Note that documents can contain native Python types (like datetime.datetime instances)
    #  which will be automatically converted to and from the appropriate BSON types.
    post = {
        "time": datetime.datetime.now(tz=datetime.timezone.utc),
        "temp": 20.0,
        "humidity": 67.0,
        "air_quality": 91.99,
        "carbon_monoxide_concentration": 1.37,
        "carbon_dixide_concentration": 0.08,
        "metadata": {
            "temp": "celcius - °C ",
            "humidity": "percentage - %",
            "air_quality": "percentage - %",
            "carbon_monoxide_concentration": "parts per million - ppm",
            "carbon_dixide_concentration": "parts per million - ppm",
            "time": "bson",
        } 
    }

    result = await environment_sampling_collection.insert_one(post)
    print(result.inserted_id)

asyncio.run(main())
