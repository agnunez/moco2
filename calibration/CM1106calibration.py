# install with: sudo python -m pip install pyserial
import serial

comport = 'COM4'
combaud = 9600
debug = True

def pba(data):      #print bytearray data
    print(''.join('{:02x}'.format(x) for x in data))
    
def cs(s):
    cs = 0
    for i in range(0,len(s)-1):
        cs+=s[i]
    return 256 - cs % 256

def readco2():
    ser = serial.Serial(comport, combaud, timeout=4)
    readcmd = [0x11,0x01,0x01,0xED]
    ser.write(serial.to_bytes(readcmd))
    res = ser.read(8)        # read up to 8 bytes (timeout)
    ppm = res[3]*256+res[4]
    if debug:
        print("df3: ", res[5]," ppm: ", end='')
    if (cs(res) != res[7]):
        ser.flushInput()
        print(res)
        raise ValueError('cs is wrong')
        ppm = -1
    ser.close()
    return ppm

def calibrate():
    ser = serial.Serial(comport, combaud, timeout=4)
    ppm = int(input("Set current ppm: "))
    df1=int(ppm/256)
    df2=ppm%256
    calcmd = serial.to_bytes([0x11,0x03,0x03,df1,df2,0x00])
    calcmd = calcmd[0:5]+serial.to_bytes([cs(calcmd)])
    ser.write(serial.to_bytes(calcmd))
    res = ser.read(4)
    ser.close()
    if (debug):
        print (res)
    return

def abc():
    try:
        auto = int(input("Set auto calibration ABC? (open=1,close=0,def=open): "))
    except:
        auto=1
    try:
        days = int(input("number of days (def=7):"))
    except:
        days = 7
    try:
        base = int(input("base value (def=400):"))
    except:
        base = 400
    if (auto == 1):
        df2=0 # ABC open 
    else:
        df2=2 # ABC close
    if (days > 0 and days < 31):
        df3=days
    else:
        print("invalid calibration cycles days, beyond (1-30)")
        return
    df1 = 0x64
    df4 = int(base/256)
    df5 = base%256
    df6 = 0x64
    ser = serial.Serial(comport, combaud, timeout=4)
    abccmd = serial.to_bytes([0x11,0x07,0x10,df1,df2,df3,df4,df5,df6,0x00])
    abccmd = abccmd[0:9]+serial.to_bytes([cs(abccmd)])
    if (debug): pba(abccmd)
    ser.write(serial.to_bytes(abccmd))
    res = ser.read(4)
    ser.close()    
    
while True:
    try:
        ppm = readco2()
    except:
        print("cs error")
    a = input("ret=ppm,c=cal,a=abc,q=quit?")
    if (ppm == 550):
        print("Heating, ", end='')
    print("ppm: ", ppm)
    if (a=="c"):
        calibrate()        
    if (a=="a"):
        abc()        
    if (a=="q"):
        break        
