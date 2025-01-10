import serial
import argparse
from pwn import p64
import os

# Run send_kernel.py in localhost to send kernel8.img to rpi3 when the bootload>

kernel_path = os.path.join(os.path.dirname(__file__), 'kernel', 'kernel8.img')
parser = argparse.ArgumentParser()
parser.add_argument('--device', metavar='TTY',default='/dev/ttyUSB0', type=str,  help='path to UART device')
args = parser.parse_args()
ser = serial.Serial(args.device, 115200, timeout=3)

def read_ack():
    ack = ser.read(1)
    if ack != b'\x06':
        print("ACK not received, aborting.")
        ser.close()
        exit(1)

# header
ser.write(b'\x1B')
read_ack()

with open(kernel_path, 'rb') as f:
    kernel = f.read()
    kernel_size = len(kernel)
    print(f"Kernel size: {kernel_size}")

    # send kernel size, default small endian
    ser.write(p64(kernel_size))
    ser.flush()
    read_ack()
    print("Kernel size sent.")

    for i in range(kernel_size):
        ser.write(kernel[i:i+1])
        ser.flush()

    print("Kernel sent")
    ser.write(b'\xFF') # footer, end of transmission
    read_ack()
    print("Receive ACK.")

ser.close()