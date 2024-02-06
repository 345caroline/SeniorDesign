import sys
from tracemalloc import start
import serial
import time
import struct

def WriteByte(oByte, port, bSpecial=False, bAscii=False):
        if bAscii:
            if not bSpecial and oByte < 0x10:
                oByte = struct.pack("B", oByte ^ 0x10)
                port.write(struct.pack("B", 0x02))                
            else:
                oByte = struct.pack("B", oByte)
            port.write(oByte)    
        else:
            if not bSpecial and ord(oByte) < 0x10:
                oByte = struct.pack("B", ord(oByte) ^ 0x10)
                port.write(struct.pack("B", 0x02))
            port.write(oByte)    


def WriteMessage(eMessageType, sData, port):
    u8Checksum = ((eMessageType >> 8) & 0xFF) ^ ((eMessageType >> 0) & 0xFF)
    u8Checksum = u8Checksum ^ (((len(sData)/2) >> 8) & 0xFF) ^ (((len(sData)/2) >> 0) & 0xFF)
    bIn=True
    for byte in sData:
        if bIn:
            u8Byte= int(byte,16)<<4 & 0xFF
            bIn=False
        else:
            u8Byte |= int(byte,16)>>0 & 0xFF          
            u8Checksum = u8Checksum ^ u8Byte
            bIn=True
        
    u16Length = len(sData)/2
    
    WriteByte(struct.pack("B", 0x01), port, True)
    WriteByte(struct.pack("B", (eMessageType >> 8) & 0xFF), port)
    WriteByte(struct.pack("B", (eMessageType >> 0) & 0xFF), port)
    WriteByte(struct.pack("B", (u16Length >> 8) & 0xFF), port)
    WriteByte(struct.pack("B", (u16Length >> 0) & 0xFF), port)
    WriteByte(struct.pack("B", (u8Checksum) & 0xFF), port)
    bIn= True
    
    for byte in sData:
        if bIn:
            u8Byte= int(byte,16)<<4 & 0xFF
            bIn=False
        else:
            u8Byte |= int(byte,16)>>0 & 0xFF                
            WriteByte(u8Byte, port, False,True)
            bIn=True
        
    WriteByte(struct.pack("B", 0x03), port, True)


def start_network(port):
    # Start network
    # The sequence of messages needed to start a network can be found in appendix C of the documentation
    # This just follows that sequence
    # 0x0011 is the message type for the reset message
    # The reset message does not contain a payload
    WriteMessage(0x0011, port)
    # Should recieve several messages back from the node
    # Should recieve Node Cluster List, Node Attribute List and Node Command ID List
    time.sleep(5)
    while port.in_waiting:
        print(port.readline())

    # 0x0010 is the message type for the get version message
    # The get version message does not contain a payload
    WriteMessage(0x0010, port)

    # Should recieve several messages back from the node
    # Should recieve status and version list
    time.sleep(1)
    while port.in_waiting:
        print(port.readline())
    
    # 0x0020 is the message type for the set extended pan id
    # The payload for the set extended pan id is the 64 bit extended pan id
    # I picked this Extended PAN ID at random, I really hope that doesn't matter
    WriteMessage(0x0020, 0x0001000400f000e0, port)

    # Should recieve status afrom the node
    time.sleep(1)
    while port.in_waiting:
        print(port.readline())

    # 0x0021 is the message type for the set channel mask
    # The payload for the set channel mask is the 32 bit channel mask
    # The channel mask tells the node what channels it can start the network on
    # A website I found said that this was default so I am using it
    WriteMessage(0x0021, 0x7FFF800, port)

    # Should recieve status afrom the node
    time.sleep(1)
    while port.in_waiting:
        print(port.readline())

    # 0x0022 is the message type for the set security state and key
    # The payload for the set security state and key is 8 bit key
    # The documentation doesn't explain this so I am going to comment it out and hope it isn't important
    # _WriteMessage(0x0022, 0x something)

    # Should recieve status afrom the node
    # while port.in_waiting:
    #     print(port.readline())

    # 0x0023 is the message type for the set device type
    # The payload for the set channel mask is the 8 bit device type
    # 0 is coordinator, 1 is router and 2 is legacy router
    WriteMessage(0x0023, 0x00, port)

    # Should recieve status afrom the node
    time.sleep(1)
    while port.in_waiting:
        print(port.readline())

    # 0x0024 is the message type for the start network message
    # The start netowrk message does not contain a payload
    WriteMessage(0x0024, port)
    # Should recieve several messages back from the node
    # Should recieve status and network joined/formed
    time.sleep(1)
    while port.in_waiting:
        print(port.readline())


def send_message(port):
    # 0x00b7 is the message type for the move color
    # The payload for the move color is 8 bit address mode, 16 bit target short address, 8 bit source endpoint, 8 bit destination endpoint, 16 bit color x, 16 bit color y
    # WriteMessage(0x0021, idk , port)

    # Should recieve status afrom the node
    time.sleep(1)
    while port.in_waiting:
        print(port.readline())




if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser()
    
    parser.add_option("-p", "--port", dest="port",
                      help="Serial port device name to use", default=None)
                      
    parser.add_option("-b", "--baudrate", dest="baudrate",
                      help="Baudrate", default=1000000)

    (options, args) = parser.parse_args()
                    
    if options.port is None:
        #print "Please specify serial port with --port"
        parser.print_help()
        sys.exit(1)

    port = serial.Serial(options.port)
    port.open()
    start_network(port)
    while(true):
        send_message(port)


    
