# install with: sudo python -m pip install pyserial
# to stop reading and get options menu prompt, press ctrl-c

import serial
from time import sleep 

comport = 'COM18'
combaud = 9600
debug = False

def pba(data):      #print bytearray data
    print(''.join('{:02x}'.format(x) for x in data))
    
def cs(s):
    cs = 0
    for i in range(0,len(s)-1):
        cs+=s[i]
    return 256 - cs % 256

def readco2(info):
    ser = serial.Serial(comport, combaud, timeout=4)
    readcmd = [0x11,0x01,0x01,0xED]
    ser.write(serial.to_bytes(readcmd))
    res = ser.read(8)        # read up to 8 bytes (timeout)
    ppm = res[3]*256+res[4]
    if info:
        x=res[5]
        print("df3: ",bin(x),", ", end='')
        if (x&(1<<6)):
            print ("drift, ", end='')
        else:
            print ("no drift, ", end='')
        if (x&(1<<5)):
            print ("light aging, ", end='')
        else:
            print ("no aging, ", end='')
        if (x&(1<<4)):
            print ("non calibrated, ", end='')
        else:
            print ("calibrated, ", end='')
        if (x&(1<<3)):
            print ("< range, ", end='')
        else:
            print ("normal range, ", end='')
        if (x&(1<<2)):
            print ("> range, ",  end='')
        else:
            print ("normal range, ", end='')
        if (x&(1<<1)):
            print ("s.error, ",  end='')
        else:
            print ("s.ok, ", end='')
        if (x&(1<<0)):
            print ("preheat completed")
        else:
            print ("preheating, ")
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
    if (debug):  print (res)
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

def swver():
    ser = serial.Serial(comport, combaud, timeout=4)
    readcmd = [0x11,0x01,0x1E,0xD0]
    ser.write(serial.to_bytes(readcmd))
    res = ser.read(15)        # read up to 8 bytes (timeout)
    print("sw version: ", end='')
    for i in range(3,15):
        print(chr(res[i]), end='')
    print("")
    return

def sernum():
    ser = serial.Serial(comport, combaud, timeout=4)
    readcmd = [0x11,0x01,0x1F,0xCF]
    ser.write(serial.to_bytes(readcmd))
    res = ser.read(9)        # read up to 8 bytes (timeout)
    print("serial number: ", end='')
    for i in range(3,7):
        print(res[i], end='')
    print("")
    return

def info():
    ppm = readco2(True)
    return

def incmdexe():
    print("choose command letter and press [enter]")
    a = input("i=info, c=cal, a=abc, v=version, s=sernum, q=quit? ")
    if (a=="i"):
        info()        
    if (a=="c"):
        calibrate()        
    if (a=="a"):
        abc()        
    if (a=="v"):
        swver()        
    if (a=="s"):
        sernum()        
    if (a=="q"):
        quit()
    return

print("press just once CTRL-C to get options menu prompt and wait")
while True:
    try:
        while True:
            try:
                ppm = readco2(False)
                if (ppm == 550):
                    print("Heating, ", end='')
                print("ppm: ", ppm)
            except:
                print("cs error")    
            sleep(5)
    except KeyboardInterrupt:
        incmdexe()
