import serial,time

def cs(s):
    cs = 0
    for i in range(0,len(s)-1):
        cs+=s[i]
    return 256 - cs % 256

def readco2():
    debug = True
    ser = serial.Serial('COM18', 9600, timeout=4)
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

def setcal(ppm):
    ser = serial.Serial('COM18', 9600, timeout=4)
    df1=int(ppm/256)
    df2=ppm%256
    calcmd = serial.to_bytes([0x11,0x03,0x03,df1,df2,0x00])
    calcmd = calcmd[0:5]+serial.to_bytes([cs(calcmd)])
    ser.write(serial.to_bytes(calcmd))
    res = ser.read(4)
    ser.close()
    return res

ppm = 550

while ppm == 550 or ppm < 0 :
    try:
      ppm = readco2()
      print("Heating")
    except:
      print("cs error")
    time.sleep(5)

ppm = int(input("Set current ppm: "))
print (setcal(ppm))

while True:
    try:
        print(readco2())
    except:
        print("cs error")
    time.sleep(5)
