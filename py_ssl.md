```

client.py
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import ssl

ca_cert_file = '../ca/ca.crt'
cert_file = 'client.crt'
key_file = 'client.key'

context = ssl.SSLContext(ssl.PROTOCOL_SSLv23)
context.load_cert_chain(cert_file, key_file, password=b'123456')
context.load_verify_locations(ca_cert_file)

context.verify_mode = ssl.CERT_REQUIRED
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
ssl_client = context.wrap_socket(client_socket)
ssl_client.connect(('', 23000))
ssl_client.send('hello, world! hello %s' %key_file)

server.py
import socket
import ssl

ca_cert_file = '../ca/ca.crt'
cert_file = 'server.crt'
key_file = 'server.key'

context = ssl.SSLContext(ssl.PROTOCOL_SSLv23)
context.load_cert_chain(cert_file, key_file)
context.load_verify_locations(ca_cert_file)
context.set_default_verify_paths()

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('', 23000))
server_socket.listen(socket.SOMAXCONN)
def handle_client(ssl_socket):
    data = ssl_socket.read()
    while data:
        print("%s" % (str(data)))
        data = ssl_socket.read()
while True:
    client_socket, addr = server_socket.accept()
    ssl_client_socket = context.wrap_socket(client_socket, server_side=True)
handle_client(ssl_client_socket)

```
