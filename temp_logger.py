import sys

import serial
import sqlite3
import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
import os
import time

DB_FILE = "serial_data.db"


def create_database():
    """Create a new SQLite database with the required table."""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()
    cursor.execute("""
    CREATE TABLE IF NOT EXISTS data (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        timestamp TEXT NOT NULL,
        value TEXT NOT NULL
    )
    """)
    conn.commit()
    conn.close()


def write_to_database(value):
    """Write a new value with a timestamp to the database."""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    cursor.execute("INSERT INTO data (timestamp, value) VALUES (?, ?)", (timestamp, value))

    conn.commit()
    conn.close()


def export_to_excel_and_chart(filename=None):
    """Export the database contents to Excel and generate a chart."""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # Read all data
    cursor.execute("SELECT * FROM data")
    rows = cursor.fetchall()
    conn.close()

    if not rows:
        print("No data to export. Exiting...")
        return

    # Convert to Pandas DataFrame
    df = pd.DataFrame(rows, columns=['ID', 'Timestamp', 'Temp'])
    df['Timestamp'] = pd.to_datetime(df['Timestamp'])
    df['Temp'] = pd.to_numeric(df['Temp'], errors='coerce')  # Convert to numeric if possible

    # Generate a filename based on the first timestamp
    first_timestamp = df['Timestamp'].iloc[0]
    if not filename:
        filename_xlsx = first_timestamp.strftime("%Y%m%d_%H%M%S") + ".xlsx"
    else:
        filename_xlsx = filename + ".xlsx"

    # Export to Excel
    df.to_excel(filename_xlsx, index=False)
    print(f"Data exported to {filename}")

    # Plot the data
    plt.figure(figsize=(10, 6))
    plt.plot(df['Timestamp'], df['Temp'], linestyle='-', label="Temp")
    plt.xlabel("Timestamp")
    plt.ylabel("Temperatur in Celsius")
    if filename:
        plt.title(filename + "\nTemperaturverlauf in Celsius am " + first_timestamp.strftime("%d.%m.%Y %H:%M:%S"))
    else:
        plt.title(filename + "Temperaturverlauf in Celsius am " + first_timestamp.strftime("%d.%m.%Y %H:%M:%S"))

    plt.legend()
    plt.grid(True)

    # Save plot as an image
    if not filename:
        chart_filename = first_timestamp.strftime("%Y%m%d_%H%M%S") + "_chart.png"
    else:
        chart_filename = filename + ".png"
    plt.savefig(chart_filename)
    # plt.show()

    print(f"Chart saved as {chart_filename}")


def delete_database():
    """Delete the current database file."""
    if os.path.exists(DB_FILE):
        os.remove(DB_FILE)
        print("Database deleted.")
    else:
        print("No database found to delete.")


def read_serial_data():
    """Read data from the serial port every 0.5 seconds and write to the database."""
    try:
        ser = serial.Serial('/dev/ttyUSB0', baudrate=9600, timeout=1)
        print("Reading from serial port...")

        while True:
            data = ser.readline().decode('utf-8').strip()
            if data:
                print(f"Data received: {data}")
                write_to_database(data)

            time.sleep(0.5)  # Read every 0.5 seconds

    except serial.SerialException as e:
        print(f"Error reading from serial port: {e}")
    except KeyboardInterrupt:
        print("\nStopping serial data collection...")


def main():
    """Main function to start the application."""
    # Export existing database contents (if any) to Excel and generate chart
    if os.path.exists(DB_FILE):
        export_to_excel_and_chart()
        delete_database()

    # Create new database for new data collection
    create_database()
    read_serial_data()


if __name__ == "__main__":

    if len(sys.argv) > 1:
        if sys.argv[1] == "export":
            if len(sys.argv) > 2:
                if sys.argv[2] != None:
                    export_to_excel_and_chart(sys.argv[2])
            else:
                export_to_excel_and_chart()
            delete_database()
    else:
        main()