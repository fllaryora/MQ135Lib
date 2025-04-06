import asyncio
from bleak import BleakClient
import struct
from pymongo import MongoClient
import datetime
#  pip3 install bleak --break-system-packages

# helper function for running a target periodically
async def periodic(interval_sec, coroutine_name, *args, **kwargs):
    # loop forever
    while True:
        # wait an interval
        await asyncio.sleep(interval_sec)
        # await the target
        await coroutine_name(*args, **kwargs)

async def work():
    ble_address = "B810CBF3-628F-38AF-DFAA-B5E5CBA67E76"
    air_quality_sensor = ble_address

    temperature_uuid = "00002a6e-0000-1000-8000-00805f9b34fb"
    humidity_uuid = "00002a6f-0000-1000-8000-00805f9b34fb"
    air_percentage = "43dc60ce-42b7-11ee-be56-0242ac120001"
    co_ppm = "00002bd0-0000-1000-8000-00805f9b34fb"
    co2_ppm = "43dc60ce-42b7-11ee-be56-0242ac120001"
    alcohol_ppm = "43dc60ce-42b3-11ee-be56-0242ac120001"
    acetone_ppm = "43dc60ce-42b4-11ee-be56-0242ac120001"
    toluene_ppm = "43dc60ce-42b5-11ee-be56-0242ac120001"
    amonium_ppm = "43dc60ce-42b6-11ee-be56-0242ac120001"

    littleEndianFloat = "<f"

    async with BleakClient(air_quality_sensor) as client:
        # print(client.is_connected)
        #serviceList = client.services

        bytesValue = await client.read_gatt_char(temperature_uuid)
        temperature = struct.unpack(littleEndianFloat, bytesValue)[0]
        print("Temperature: " + str(round(temperature,2)) + "°C")

        bytesValue = await client.read_gatt_char(humidity_uuid)
        humidity = struct.unpack(littleEndianFloat, bytesValue)[0]
        print("Humidity: " + str(round(humidity,2)) + "%")

        bytesValue = await client.read_gatt_char(co_ppm)
        carbon_monoxide_concentration = struct.unpack(littleEndianFloat, bytesValue)[0]
        print("Carbon Monoxide Concentration: " + str(round(carbon_monoxide_concentration,2)) + " ppm")

        bytesValue = await client.read_gatt_char(co2_ppm)
        carbon_dixide_concentration = struct.unpack(littleEndianFloat, bytesValue)[0]
        print("Carbon DIxide Concentration: " + str(round(carbon_dixide_concentration,2)) + " ppm")

        bytesValue = await client.read_gatt_char(alcohol_ppm)
        alcohol_concentration = struct.unpack(littleEndianFloat, bytesValue)[0]
        print("Alcohol PPM: " + str(round(alcohol_concentration,2)) + " ppm")

        bytesValue = await client.read_gatt_char(acetone_ppm)
        acetone_concentration = struct.unpack(littleEndianFloat, bytesValue)[0]
        print("Acetone PPM: " + str(round(acetone_concentration,2)) + " ppm")

        bytesValue = await client.read_gatt_char(toluene_ppm)
        toluene_concentration = struct.unpack(littleEndianFloat, bytesValue)[0]
        print("Toluene PPM: " + str(round(toluene_concentration,2)) + " ppm")

        bytesValue = await client.read_gatt_char(amonium_ppm)
        amonium_concentration = struct.unpack(littleEndianFloat, bytesValue)[0]
        print("Amonium PPM: " + str(round(amonium_concentration,2)) + " ppm")

        bytesValue = await client.read_gatt_char(air_percentage)
        floatValue = struct.unpack(littleEndianFloat, bytesValue)
        air_quality = (1.0 - floatValue[0]) * 100.0
        print("air percentage: " + str(round(air_quality,2)) + " %")

        sensor = (
            temperature,
            humidity,
            air_quality,
            carbon_monoxide_concentration,
            carbon_dixide_concentration,
            alcohol_concentration,
            acetone_concentration,
            toluene_concentration,
            amonium_concentration,
        )

        await post_in_mongo(sensor)


async def post_in_mongo(sensor):

    mongo_url = "mongodb://localhost:27017/"
    my_client = MongoClient(mongo_url)
    office = "office"
    office_db = my_client[office]
    environment_sampling = "environment_sampling"
    environment_sampling_collection = office_db[environment_sampling]

    # untuple
    (
        temperature,
        humidity,
        air_quality,
        carbon_monoxide_concentration,
        carbon_dixide_concentration,
        alcohol_concentration,
        acetone_concentration,
        toluene_concentration,
        amonium_concentration,
    ) = sensor

    # Note that documents can contain native Python types (like datetime.datetime instances)
    #  which will be automatically converted to and from the appropriate BSON types.
    post = {
        "time": datetime.datetime.now(tz=datetime.timezone.utc),
        "temperature": temperature,
        "humidity": humidity,
        "air_quality": air_quality,
        "carbon_monoxide_concentration": carbon_monoxide_concentration,
        "carbon_dixide_concentration": carbon_dixide_concentration,
        "alcohol_concentration": alcohol_concentration,
        "acetone_concentration": acetone_concentration,
        "toluene_concentration": toluene_concentration,
        "amonium_concentration": amonium_concentration,
        "metadata": {
            "temperature": "celcius - °C ",
            "humidity": "percentage - %",
            "air_quality": "percentage - %",
            "carbon_monoxide_concentration": "parts per million - ppm",
            "carbon_dixide_concentration": "parts per million - ppm",
            "alcohol_concentration": "parts per million - ppm",
            "acetone_concentration": "parts per million - ppm",
            "toluene_concentration": "parts per million - ppm",
            "amonium_concentration": "parts per million - ppm",
            "time": "bson time stamp",
        } 
    }

    result = environment_sampling_collection.insert_one(post)
    print(result.inserted_id)
    my_client.close

# main coroutine
async def main():
    # report a message
    print('Main starting')
    # configure the periodic task
    task = asyncio.create_task(periodic(5.0, work))
    # cancel the background task
    # task.cancel()

    # infinite wait for the task to cancel
    try:
        await task
    except asyncio.CancelledException:
        pass



# start the event loop
asyncio.run(main())
