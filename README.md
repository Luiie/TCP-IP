# TCP-IP
TCP/IP study using C

### 01 Server and Client
Server
<img width="1024" height="1024" alt="image" src="https://github.com/user-attachments/assets/679486ff-c868-428a-a34f-6ebf37b13e88" />

Client
<img width="1024" height="1024" alt="image" src="https://github.com/user-attachments/assets/13807b55-d1c9-481c-a904-1461cdecdb08" />

- **Socket Descriptor** = key for socket
  - 운영체제에게 어떤 소켓을 통해 데이터를 주고받을지 알려주는 역할
  - read, write, close 같은 함수는 이 descriptor를 이용해 데이터 송수신과 소켓 관리를 수행

TCP/IP Protocol Stack
<img width="1024" height="1024" alt="image" src="https://github.com/user-attachments/assets/50923ca3-f87d-4e5f-b43f-912e0ef2cb46" />

1. Application 계층
- 서버/클라이언트 프로그램 또는 HTTP, FTP 같은 서비스들이 동작
- 데이터를 어떻게 주고받을지, 메시지 형식은 무엇인지, 요청/응답의 규칙은 무엇인지 같은 “통신 방식”을 정의

2. TCP/UDP (Transport 계층)
- Application 계층에서 만든 데이터를 실제 전송 가능한 형태로 만들어 주는 계층
- TCP는 신뢰성(재전송, 순서 보장 등)을 제공하고, UDP는 빠른 전송을 위한 단순한 구조
- IP 계층이 정해준 경로를 사용해 데이터의 전달 자체를 담당

3. IP (Network 계층)
- 데이터를 목적지까지 보내기 위해 어떤 경로(Route)를 선택할지 결정
- IP는 연결을 유지하지 않고, 데이터가 잘 도착했는지도 확인하지 않기 때문에 비연결지향적이고 신뢰성이 없음
- 대신 빠르고 유연하게 여러 네트워크를 넘나들 수 있음

4. Link 계층 (Data Link / Physical 계층 포함)
- 실제 물리적인 네트워크 환경(LAN, WAN, MAN 등)에서 전기 신호, MAC 주소, 프레임 구조 등을 이용해 데이터를 전송하는 계층
- 물리적 매체와 이를 사용하는 규칙을 표준화한 단계

### 02 TCP/UDP
- TCP = 연결형(Connected) 프로토콜
  → 클라이언트마다 “연결 상태”를 따로 관리해야 해서 소켓이 여러 개 필요
  1. 서버는 먼저 listen 소켓(파일 디스크립터 하나)을 만들어 놓음
  2. 클라이언트가 접속하면, 서버는 새로운 소켓을 하나 더 생성(accept)
  3. 각 클라이언트마다 독립된 소켓이 생김
    
- UDP = 비연결형(Connectionless) 프로토콜
  → 연결을 유지할 필요가 없어서 하나의 소켓만으로도 여러 클라이언트와 통신 가능
  1. 클라이언트별로 별도의 연결 상태를 유지할 필요가 없음


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

### 05 UDP
- sendto/recvfrom: Unconnected UDP, The UDP socket does not have a fixed IP/Port. Each time sendto() is called, the destination address is provided, used for sending, and then discarded.
  - sendto: set destination address IP/Port number -> data sending -> delete destination address IP/Port number
- write/read: Connected UDP, By calling connect(), the socket is bound to a destination address IP/Port. This allows repeated communication with the same destination more efficiently, saving time and system resources.

### 06 Half-close
For example,
- even after the server sends an EOF and closes its output stream, 
- the client can still keep its input stream open.

This allows the client to notify the server that the file transfer is complete while still remaining able to receive data.
<img width="962" height="1024" alt="image" src="https://github.com/user-attachments/assets/7efa207d-128f-4a77-afd6-6c854d784b41" />



###Time-wait


### Nagle


### 07 Process

- fork()

<img width="1024" height="448" alt="image" src="https://github.com/user-attachments/assets/88bb6445-7def-4e19-ae23-3c6eb7578b96" />

- 멀티프로세스 서버에서 클라이언트 종료 시 accept() 에러가 발생한 이유는
자식 프로세스 종료로 SIGCHLD 시그널이 발생하며 accept()가 중단되기 때문

해결 방법은 sigaction()에서 SA_RESTART 옵션을 사용해 시스템 콜을 자동 재시작

또는 accept()가 -1을 반환하고 errno == EINTR인 경우 재시도하도록 처리

- 클라이언트에서 메시지 순서가 어긋난 이유는 fork() 후 읽기/쓰기 프로세스가 비동기적으로 실행되기 때문

서버 응답 출력과 사용자 입력 프롬프트가 서로 경쟁 상태(race condition)가 됨

전역 변수를 이용해 출력 완료 여부를 제어하려 했으나,

프로세스 간에는 메모리가 공유되지 않아 전역 변수 동기화가 불가능해 실패

해결 방법은 출력/입력을 한 프로세스에서 순차 처리하거나,

IPC(파이프, 시그널) 또는 select()/poll() 기반 단일 프로세스 I/O 모델을 사용하는 것


### Pipe & Multiplexing

- Multiplexing =
- Multiplexing using **select()** = 여러 fd를 모아놓고(=events) 동시에 관찰
  - 파일 디스크립터의 설정
  - 검사 범위 지정
  - 타임아웃의 설정
