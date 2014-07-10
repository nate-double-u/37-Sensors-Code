# wiichuck_adapter.py - print Wii Nunchuk acceleration and joystick
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import smbus # sudo apt-get -y install python-smbus    # <1>

bus = None
address = 0x52    # <2>

z = 0    # <3>
c = 0
joystick_x = 0
joystick_y = 0
ax_x = 0
ax_y = 0
ax_z = 0

def initNunchuck():
    global bus
    bus = smbus.SMBus(1) # <4>
    bus.write_byte_data(address, 0x40, 0x00)    # <5>

def send_request():
    bus.write_byte(address, 0x00)    # <6>

def get_data():
    global bus, z, c, joystick_x, joystick_y, ax_x, ax_y, ax_z
    data = [0]*6
    for i in range(len(data)): # <7>
        data[i] = bus.read_byte(address)
        data[i] ^= 0x17
        data[i] += 0x17


    z = data[5] & 0x01    # <8>
    c = (data[5] >> 1) & 0x01    # <9>

    joystick_x = data[0]    
    joystick_y = data[1]
    ax_x = data[2]
    ax_y = data[3]
    ax_z = data[4]
    send_request()

def main():
    initNunchuck()
    while True:
        get_data()
        print("Button Z: %d Button C: %d joy (x,y) (%d,%d) \
                acceleration (x,y,z) (%d,%d,%d)" \
                % (z,c,joystick_x,joystick_y,ax_x, ax_y, ax_z))
        time.sleep(0.1)


if __name__ == "__main__":
    main()
