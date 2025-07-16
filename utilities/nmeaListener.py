#!/usr/bin/env python3

from socket import socket

from socket import AF_INET, SOCK_DGRAM

UDP_IP = "0.0.0.0"
UDP_PORT = 1456
PACKET_SIZE = 4096

sock = socket(AF_INET, SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP packets on {UDP_IP}:{UDP_PORT}")
while True:
    data, addr = sock.recvfrom(PACKET_SIZE)
    decoded_data = data.decode("utf-8")
    print(f"@{addr[0]}")
    for nmea in decoded_data.split():
        nmea = nmea.strip()
        print(nmea)
