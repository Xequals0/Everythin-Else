#    CS 352 project part 3
#    sock352 Library ~ CS352 Fall 2017
#    Anirudh Tunoori    (at813)
#    Alex   LaBrunda    (all158)

# this is the initial socket library for project 3
# You wil need to fill in the various methods in this
# library

# main libraries
import binascii
import socket as syssock
import struct
import sys

# For the Timeoout(s):
import time
from Queue import *
from random import *

PROTOCOL_VERSION = 3

# encryption libraries
import nacl.utils
import nacl.secret
import nacl.utils
from nacl.public import PrivateKey, Box

# if you want to debug and print the current stack frame
from inspect import currentframe, getframeinfo

# these are globals to the sock352 class and
# define the UDP ports all messages are sent
# and received from

# the ports to use for the sock352 messages
global sock352portTx
global sock352portRx
# the public and private keychains in hex format
global publicKeysHex
global privateKeysHex

# the public and private keychains in binary format
global publicKeys
global privateKeys

# the encryption flag
global ENCRYPT

# The window
global window
window = Queue()

# Dictionaries:
publicKeysHex = {}
privateKeysHex = {}
publicKeys = {}
privateKeys = {}

# this is 0xEC
ENCRYPT = 236

# Flag Bits:
SYN = 0x01
FIN = 0x02
ACK = 0x04
RESET = 0x08
HAS_OPT = 0xA0

# this is the structure of the sock352 packet
sock352HdrStructStr = '!BBBBHHLLQQLL'
STRUCT_TYPE = struct.Struct(sock352HdrStructStr)
HEADER_SIZE = 40
RECV_SIZE = 4096
WINDOW_SIZE = 5
window_size = 0
TIMEOUT = .2

ATTRIBUTES = {'version': 0, 'flags': 1, 'opt_ptr': 0, 'header_len': 4, 'sequence_no': 8, 'ack_no': 9, 'window_size': 0, 'payload_len': 11}

# create the sockets to send and receive UDP packets on
# if the ports are not equal, create two sockets, one for Tx and one for Rx
def init(UDPportTx,UDPportRx):
    global global_socket
    global sock352portTx
    global sock352portRx
    global send_port
    global recv_port
    
    sock352portTx = int(UDPportTx)
    sock352portRx = int(UDPportRx)
    
    global_socket = syssock.socket(syssock.AF_INET, syssock.SOCK_DGRAM)
    send_port = sock352portTx
    recv_port = sock352portRx

    if send_port < 1 or send_port > 65535:
        send_port = 27182

    if recv_port < 1 or recv_port > 65535:
        recv_port = 27182

    global_socket.bind(('', recv_port))

# read the keyfile. The result should be a private key and a keychain of
# public keys
def readKeyChain(filename):
    global publicKeysHex
    global privateKeysHex
    global publicKeys
    global privateKeys

    if (filename):
        try:
            keyfile_fd = open(filename,"r")
            for line in keyfile_fd:
                words = line.split()
                # check if a comment
                # more than 2 words, and the first word does not have a
                # hash, we may have a valid host/key pair in the keychain
                if ( (len(words) >= 4) and (words[0].find("#") == -1)):
                    host = words[1]
                    port = words[2]
                    keyInHex = words[3]
                    if (words[0] == "private"):
                        privateKeysHex[(host,port)] = keyInHex
                        privateKeys[(host,port)] = nacl.public.PrivateKey(keyInHex, nacl.encoding.HexEncoder)
                    elif (words[0] == "public"):
                        publicKeysHex[(host,port)] = keyInHex
                        publicKeys[(host,port)] = nacl.public.PublicKey(keyInHex, nacl.encoding.HexEncoder)
        except Exception,e:
            print ( "error: opening keychain file: %s %s" % (filename,repr(e)))
    else:
        print ("error: No filename presented")
    
    return (publicKeys,privateKeys)



# Socket Class
class socket:

    def __init__(self):
        # your code goes here
        self.socket_open = False
        self.is_listening = False
        self.destination_hostname = None
        self.destination_port = None
        self.backlogged_connections = None
        self.outbound_queue = Queue(maxsize=WINDOW_SIZE)
        self.recv_ack_no = []
        self.sent_ack_no = []
        self.start_seq_no = 0
        self.resending_flag = False
        self.previous_seq_no = 0

        self.binding_address = ''
        self.binding_port = recv_port
        
        return

    def bind(self,address):
        # bind is not used in this assignment
        return

    def connect(self,*args):
        global sock352portTx
        global ENCRYPT
        global sender_box
        global nonce
        global encryption
        encryption = False
        
        # example code to parse an argument list
        if (len(args) >= 1):
            (host,port) = args[0]
        elif (len(args) >= 2):
            if (args[1] == ENCRYPT):
                encryption = True
        # print(encryption)
        # your code goes here
        if(encryption is True):
            if ('localhost', recv_port) in publicKeys and ('localhost', send_port) in privateKeys:
                # ENCRYPT = True
                print("true connect")
                receiversPublicKey = publicKeys[('localhost', recv_port)]
                sendersPrivateKey = privateKeys[('localhost', send_port)]
                sender_box = Box(sendersPrivateKey, receiversPublicKey)
                nonce = nacl.utils.random(Box.NONCE_SIZE)
        
        # Make sure self is open and global socket is initialized
        if self.socket_open or not global_socket:
            return

        data, sender = (None, (None, None))
        self.destination_hostname = args[0][0]
        self.destination_port = send_port
        # print(self.destination_hostname)
        if encryption is True: #set the options field if encryption is necessary
            syn_pack = STRUCT_TYPE.pack(PROTOCOL_VERSION, HAS_OPT, 1, 0, HEADER_SIZE, 0, 0, 0, 0, 0, 0, 0)
        else:
            syn_pack = STRUCT_TYPE.pack(PROTOCOL_VERSION, SYN, 0, 0, HEADER_SIZE, 0, 0, 0, 0, 0, 0, 0)
        
        # Now we wait for a response back from the user
        while True:
            # We resend the packet if we have a timeout
            global_socket.sendto(syn_pack, (self.destination_hostname, self.destination_port))
            try:
                global_socket.settimeout(TIMEOUT)
                data, sender = global_socket.recvfrom(RECV_SIZE)
                # We received an ACK
                break
            except syssock.timeout:
                continue
            finally:
                global_socket.settimeout(None)

        syn_pack = STRUCT_TYPE.unpack(data)
        self.socket_open = True
        return

    # Calls receive on the underlying UDP socket and waits for a connection request from the client.
    def listen(self,backlog):
        # listen is not used in this assignments
        if not global_socket:
            return

        data, sender = (None, (None, None))
        self.is_listening = True
        self.backlogged_connections = Queue(maxsize=backlog)
        
        while True:
            try:
                global_socket.settimeout(TIMEOUT)
                data, sender = global_socket.recvfrom(HEADER_SIZE)
            except syssock.timeout:
                continue
            finally:
                global_socket.settimeout(None)

            syn_pack = STRUCT_TYPE.unpack(data)
            sender_address = sender[0]
            sender_port = sender[1]
            sender_seqno = syn_pack[8]

            if syn_pack[1] == SYN:
                self.backlogged_connections.put((sender, sender_seqno))
            break

        return

    # Accept a new connection
    # @return a new socket object which the server uses to communicate to the client
    def accept(self,*args):
         # example code to parse an argument list
        global ENCRYPT
        global receiver_box
        global encryption
        encryption = False
        
        if (len(args) >= 1):
            if (args[0] == ENCRYPT):
                encryption = True
        # print(encryption)
        # your code goes here
        if(encryption is True):
            if ('localhost', send_port) in publicKeys and ('localhost', recv_port) in privateKeys:
                # ENCRYPT = True
                print("true accept")
                sendersPublicKey = publicKeys[('localhost', recv_port)]
                receiversPrivateKey = privateKeys[('localhost', send_port)]
                receiver_box = Box(receiversPrivateKey, sendersPublicKey)
        
        if not self.is_listening or not global_socket:
            return

        # Accepted a connection; stop listening
        self.socket_open = False
        # Check backlog for pending connection requests
        this_connection = self.backlogged_connections.get()
        if this_connection is None:
            return

        self.accepted_connection = this_connection[0]
        sequence_no = randint(0, 1000)
        ack_no = this_connection[1]

        # Complete connection setup handshake
        if encryption is True: #set the options field if encryption is necessary
            syn_pack = STRUCT_TYPE.pack(PROTOCOL_VERSION, HAS_OPT, 1, 0, HEADER_SIZE, 0, 0, 0, 0, 0, 0, 0)
        else:
            syn_pack = STRUCT_TYPE.pack(PROTOCOL_VERSION, (SYN & ACK), 0, 0, HEADER_SIZE, 0, 0, 0, sequence_no, ack_no, 0, 0)
        global_socket.sendto(syn_pack, self.accepted_connection)

        # Create new sock352 socket, initialize it, and return it
        return_socket = socket()
        return_socket.socket_open = True
        return_socket.destination_hostname = self.accepted_connection[0]
        return_socket.destination_port = self.accepted_connection[1]
        address = (self.accepted_connection[0], self.accepted_connection[1])

        return (return_socket, address)

    def close(self):
        # your code goes here
        if not global_socket:
            self.socket_open = False
            self.is_listening = False
            return

        # Close any open connections
        if self.socket_open:
            closing_connection = (self.destination_hostname, self.destination_port)
            fin_pack = STRUCT_TYPE.pack(PROTOCOL_VERSION, FIN, 0, 0, HEADER_SIZE, 0, 0, 0, 0, 0, 0, 0)
            global_socket.sendto(fin_pack, closing_connection)
            self.socket_open = False

        # Reject any pending connection requests
        while self.backlogged_connections and not self.backlogged_connections.empty():
            closing_connection = self.backlogged_connections.get()[0]
            syn_pack = STRUCT_TYPE.pack(PROTOCOL_VERSION, FIN, 0, 0, HEADER_SIZE, 0, 0, 0, 0, 0, 0, 0)
            global_socket.sendto(fin_pack, closing_connection)

        return


    def send(self,buffer):
        # We get a byte-stream buffer
        if not self.socket_open or not global_socket:
            return
        # Encryption Process
        payload = buffer[:4000]
        if encryption is True:
            has_opt = 1
            print(encrypted)
            new_data_to_send = sender_box.encrypt(payload, nonce)
        else:
            has_opt = 0
            new_data_to_send = payload

        self.start_seq_no += 1
        while True:
            data, sender = None, (None, None)
            sending_packet_type = struct.Struct("!BBBBHHLLQQLL")
            syn_pack = STRUCT_TYPE.pack(PROTOCOL_VERSION, ACK, has_opt, 0, HEADER_SIZE, 0, 0, 0, self.start_seq_no, 0, window_size, len(new_data_to_send))
            syn_pack += new_data_to_send
            loadSize = len(new_data_to_send)
            while True:
                size = 32000 - sys.getsizeof(window)
                if loadSize < size:
                    break
        
            bytessent = global_socket.sendto(syn_pack, (self.destination_hostname, self.destination_port))
            # Wait for an ACK
            while True:
                try:
                    global_socket.settimeout(1.0)
                    data, sender = global_socket.recvfrom(RECV_SIZE)
                    self.resending_flag = False
                    self.sent_ack_no.append(self.start_seq_no)
                except syssock.timeout:
                    self.resending_flag = True
                    continue
                finally:
                    global_socket.settimeout(None)
                unpacked = STRUCT_TYPE.unpack(data)
                load = unpacked[ATTRIBUTES['window_size']]
                if load > 0 and load < 32000:
                    break


            version_num = unpacked[ATTRIBUTES['version']]
            flag = unpacked[ATTRIBUTES['flags']]
            ack_no = unpacked[8]
            self.recv_ack_no.append(ack_no)
            if ack_no == self.start_seq_no:
                break
    
        return len(new_data_to_send)

    def recv(self,nbytes):
        windowSize = sys.getsizeof(window)
        resend = False
        if not self.socket_open or not global_socket:
            return

        data, sender = (None, (None, None))

        try:
            global_socket.settimeout(TIMEOUT)
            data, sender = global_socket.recvfrom(RECV_SIZE)
        except syssock.timeout:
            return ""
        finally:
            global_socket.settimeout(None)

        # Get header
        header = data[:HEADER_SIZE]
        syn_pack = STRUCT_TYPE.unpack(header)
        version_num = syn_pack[ATTRIBUTES['version']]
        header_len = syn_pack[ATTRIBUTES['header_len']]
        has_opt = syn_pack[ATTRIBUTES['opt_ptr']]
        flag = syn_pack[ATTRIBUTES['flags']]
        sequence_no = syn_pack[ATTRIBUTES['sequence_no']]
        recvWindowSize = syn_pack[ATTRIBUTES['window_size']]
        payload_len = syn_pack[ATTRIBUTES['payload_len']]
        

        if sequence_no in self.recv_ack_no:
            resend = True
        else:
            self.recv_ack_no.append(sequence_no)
        
        # Decryption Process:
        payload = data[HEADER_SIZE:]
        if has_opt == 1:
            data_to_return = receiver_box.decrypt(payload)
            print("decrypted")
        else:
            data_to_return = payload

        ack_no = sequence_no
        if payload_len < 32000:
            window.put(data_to_return)
            windowSize += payload_len
            recvWindowSize = 32000 - windowSize


        syn_pack = STRUCT_TYPE.pack(PROTOCOL_VERSION, ACK, has_opt, 0, HEADER_SIZE, 0, 0, 0, ack_no, 0, recvWindowSize, 0)

        # Sending ACK back
        bytessent = global_socket.sendto(syn_pack, (sender[0], sender[1]))

        self.sent_ack_no.append(ack_no)
        if resend:
            return ""
        
        data_to_return = window.get()
        return data_to_return
