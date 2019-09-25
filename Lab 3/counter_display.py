from collections import namedtuple
import csv

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial

DataRow = namedtuple("DataRow", "shift_time count_10cm count_20cm count_30cm count_40cm count_invalid count_jam")

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

COM_PORT = "COM3"
ser = serial.Serial(COM_PORT, 9600, timeout=1)

csvfile = open('output.csv', mode='w')
csvwriter = csv.writer(csvfile)

# This function is called periodically from FuncAnimation
def animate(i, xs, ys):
    rows = []
    while True:
        try:
            datum = ser.readline()
        except serial.SerialTimeoutException:
            break
        row = DataRow(datum.split(","))
        csvwriter.writerow(row)

        if rows and row.shift_time < rows[-1].shift_time:
            # If we see a shift time reset during data transfers, dump all data
            # prior to that reset.
            rows = []
        rows.append(row)

    shift_times = [row.shift_time for row in rows]
    total_counts = [sum(row.count_10cm, row.count_20cm, row.count_30cm, row.count_40cm, row.count_invalid) for row in rows]

    if rows and rows[0].shift_time < ys[-1]:
        # Again, if we see a shift time reset, drop all displayed data so far.
        xs = []
        ys = []

    xs.extend(total_counts)
    ys.extend(shift_times)

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys)

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Total Box Counts over Time')
    plt.ylabel('Count')

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=100)

try:
    plt.show()
finally:
    csvfile.flush()
    csvfile.close()
