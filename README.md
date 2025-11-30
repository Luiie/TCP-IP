# TCP-IP
TCP/IP study using C

### 01 Server and Client
Server
<img width="1024" height="1024" alt="image" src="https://github.com/user-attachments/assets/679486ff-c868-428a-a34f-6ebf37b13e88" />

Client
<img width="1024" height="1024" alt="image" src="https://github.com/user-attachments/assets/13807b55-d1c9-481c-a904-1461cdecdb08" />

### 02 TCP/UDP
TCP/IP Protocol Stack
<img width="1024" height="1024" alt="image" src="https://github.com/user-attachments/assets/50923ca3-f87d-4e5f-b43f-912e0ef2cb46" />

### 03 TCP
TCP Server/Client Flow
<img width="1024" height="1024" alt="image" src="https://github.com/user-attachments/assets/0e97a602-2c28-4216-bb6b-0a4fc1ddacb6" />

### 04 Echo
Echo = Message sent from the client to the server, which is then returned back to the client by the server.

Server
+ The error is due to **operator precedence**: the comparison != executes before the assignment =. This assigns the Boolean result (1 or 0) to strLength, not the actual byte count read by read().
Consequently, the server only echos the first byte (strLength is 1), losing the rest of the message.
The fix is to use parentheses to prioritize the assignment: while((strLength = read(...)) > 0).
+ The issue occurs because the buffer isn’t null-terminated after read(), causing leftover data to appear in the output.
Clear the buffer and add a null terminator (message[strLength] = '\0') to prevent mixed or corrupted messages.
