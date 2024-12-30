import serial
from pwn import p64

# Run send_kernel.py in localhost to send kernel8.img to rpi3 when the bootloader is running

kernel_path = "kernel8.img"
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

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
    read_ack()

    # send kernel by block
    block_size = 256
    for i in range(0, kernel_size, block_size):
        ser.write(kernel[i:i+block_size])
        ser.flush()
        read_ack()
    
    ser.write(b'\x00') # footer, end of transmission
    print("Kernel sent.")

ser.close()