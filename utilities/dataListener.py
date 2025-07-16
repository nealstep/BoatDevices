#!/usr/bin/env python3

from socket import socket

from socket import AF_INET, SOCK_DGRAM

UDP_IP = "192.168.8.255"
UDP_PORT = 4041
PACKET_SIZE = 4096

sock = socket(AF_INET, SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP packets on {UDP_IP}:{UDP_PORT}")
while True:
    data, addr = sock.recvfrom(PACKET_SIZE)
    decoded_data = data.decode("utf-8")
    print(f"@{addr[0]}: {decoded_data}")
