from dataclasses import dataclass
import datetime
import time
import serial

def main() -> None:
    time_start = datetime.datetime.utcnow()
    a = 0
    serial_conn = serial.Serial('COM5', 115200, timeout=1)
    while True:
        line = serial_conn.readline().decode().strip()
        a += 1
        if a % 1000 == 0:
            print(datetime.datetime.utcnow() - time_start)
            print(line)

    serial_conn.close()
    return "Done"


if __name__ == '__main__':
    main()
