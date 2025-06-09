import serial
import mysql.connector
from time import sleep

def connect_bluetooth():
    try:
        bluetooth = serial.Serial('/dev/rfcomm0', baudrate=9600, timeout=1)
        print("Connected to /dev/rfcomm0")
        return bluetooth
    except serial.SerialException as e:
        print(f"Bluetooth connection failed: {e}")
        return None

def delete_old_data(cursor, db, table_name, max_rows=1000):
    """최신 max_rows 개의 데이터만 유지하고 나머지 삭제"""
    cursor.execute(f"SELECT COUNT(*) FROM {table_name}")
    total_rows = cursor.fetchone()[0]
    if total_rows > max_rows:
        cursor.execute(f"""
            DELETE FROM {table_name}
            WHERE id NOT IN (
                SELECT id FROM (
                    SELECT id FROM {table_name}
                    ORDER BY timestamp DESC
                    LIMIT {max_rows}
                ) AS temp
            )
        """)
        db.commit()
        print(f"Deleted old data from {table_name}, kept {max_rows} rows")

# MySQL 연결
try:
    db = mysql.connector.connect(
        host="localhost",
        user="root",
        password="1234",
        database="DATA_01"
    )
    cursor = db.cursor()
    print("Connected to MySQL")
except mysql.connector.Error as e:
    print(f"MySQL connection failed: {e}")
    exit(1)

# 블루투스 초기 연결
bluetooth = connect_bluetooth()
if bluetooth is None:
    print("Initial Bluetooth connection failed, exiting...")
    db.close()
    exit(1)

# 데이터 수신 및 저장
try:
    print("Starting to receive data from STM32 via Bluetooth...")
    while True:
        try:
            if not bluetooth.is_open:
                print("Bluetooth disconnected, attempting to reconnect...")
                bluetooth.close()
                bluetooth = connect_bluetooth()
                if bluetooth is None:
                    sleep(2)
                    continue

            if bluetooth.in_waiting > 0:
                data = bluetooth.readline().decode('utf-8').strip()
                print(f"Received: {data}")

                if data.startswith("IN:"):
                    parts = data[3:].split(',')
                    if len(parts) == 2:
                        temp = int(parts[0])
                        hum = int(parts[1])
                        cursor.execute("INSERT INTO indoor_data (temperature, humidity) VALUES (%s, %s)", (temp, hum))
                        db.commit()
                        print("Indoor data saved")
                        delete_old_data(cursor, db, "indoor_data", max_rows=1000)
                elif data.startswith("OUT:"):
                    parts = data[4:].split(',')
                    if len(parts) == 2:
                        temp = int(parts[0])
                        hum = int(parts[1])
                        cursor.execute("INSERT INTO outdoor_data (temperature, humidity) VALUES (%s, %s)", (temp, hum))
                        db.commit()
                        print("Outdoor data saved")
                        delete_old_data(cursor, db, "outdoor_data", max_rows=1000)
                elif data in ("IN_ERR", "OUT_ERR"):
                    print(f"Error from STM32: {data}")
                else:
                    print("Invalid data format")
            else:
                print("No data available, waiting...")

        except serial.SerialException as e:
            print(f"Serial error: {e}")
            bluetooth.close()
            bluetooth = connect_bluetooth()
        except ValueError as e:
            print(f"Data parsing error: {e}")
        except mysql.connector.Error as e:
            print(f"MySQL error: {e}")
            db.close()
            db = mysql.connector.connect(
                host="localhost",
                user="root",
                password="1234",
                database="DATA_01"
            )
            cursor = db.cursor()
        sleep(1)

except KeyboardInterrupt:
    print("Stopped by user")
finally:
    if bluetooth and bluetooth.is_open:
        bluetooth.close()
    db.close()
    print("Connections closed")
