# Bluetooth LE scanner
# Prints the name and address of every nearby BLE device

import asyncio
from bleak import BleakScanner

async def main():
    devices = await BleakScanner.discover()
    for device in devices:
        print(device)


asyncio.run(main())
