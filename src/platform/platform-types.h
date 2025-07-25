/** Copyright (c) 2025, Wu Jin <wujin.developer@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

_Pragma("once")

#include "deprecated/c11-threads.h"
#include <errno.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__linux__) || defined(__APPLE__)
#include <arpa/inet.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

#if defined(__linux__)
#include <linux/filter.h>
#include <sys/epoll.h>
#include <sys/syscall.h>
#endif

#if defined(__APPLE__)
#include <sys/event.h>
#endif

#define PLATFORM_SO_ERROR_EAGAIN          EAGAIN
#define PLATFORM_SO_ERROR_EWOULDBLOCK     EWOULDBLOCK
#define PLATFORM_SO_ERROR_ECONNRESET      ECONNRESET
#define PLATFORM_SO_ERROR_ETIMEDOUT       ETIMEDOUT
#define PLATFORM_SO_ERROR_INVALID_SOCKET  -1
#define PLATFORM_SO_ERROR_SOCKET_ERROR    -1

#define PLATFORM_UA_ERROR_INVALID_UART    -1
#define PLATFORM_UA_ERROR_UART_ERROR      -1

#define PLATFORM_PATH_SEPARATOR '/'
#endif

#if defined(_WIN32)
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#include <mstcpip.h>
#include <process.h>
#include <ws2ipdef.h>

#define PLATFORM_SO_ERROR_EAGAIN          WSAEWOULDBLOCK
#define PLATFORM_SO_ERROR_EWOULDBLOCK     WSAEWOULDBLOCK
#define PLATFORM_SO_ERROR_ECONNRESET      WSAECONNRESET
#define PLATFORM_SO_ERROR_ETIMEDOUT       WSAETIMEDOUT
#define PLATFORM_SO_ERROR_INVALID_SOCKET  INVALID_SOCKET
#define PLATFORM_SO_ERROR_SOCKET_ERROR    SOCKET_ERROR

#define PLATFORM_UA_ERROR_INVALID_UART    INVALID_HANDLE_VALUE
#define PLATFORM_UA_ERROR_UART_ERROR      -1

#define PLATFORM_PATH_SEPARATOR '\\'

#pragma comment(lib, "ws2_32.lib")
#endif

#define PLATFORM_POLLER_CQE_NUM 64

#if defined(__linux__) || defined(__APPLE__)
#if defined(__APPLE__)
typedef uint64_t platform_tid_t;
#endif

#if defined(__linux__)
typedef pid_t platform_tid_t;
#endif

typedef int                      platform_sock_t;
typedef pid_t                    platform_pid_t;
typedef int                      platform_poller_sq_t;
typedef int                      platform_uart_t;
#endif

#if defined(_WIN32)
typedef DWORD   platform_tid_t;
typedef DWORD   platform_pid_t;
typedef HANDLE  platform_poller_sq_t;
typedef SOCKET  platform_sock_t;
typedef HANDLE  platform_uart_t;
typedef SSIZE_T ssize_t;
#endif

typedef platform_sock_t                platform_poller_fd_t;
typedef enum platform_poller_op_e      platform_poller_op_t;
typedef struct platform_poller_cqe_s   platform_poller_cqe_t;
typedef struct platform_poller_sqe_s   platform_poller_sqe_t;
typedef struct platform_uart_config_s  platform_uart_config_t;
typedef enum platform_uart_baudrate_e  platform_uart_baudrate_t;
typedef enum platform_uart_parity_e    platform_uart_parity_t;
typedef enum platform_uart_databits_e  platform_uart_databits_t;
typedef enum platform_uart_stopbits_e  platform_uart_stopbits_t;

struct platform_poller_cqe_s {
    platform_poller_op_t op;
    void*               ud;
};

struct platform_poller_sqe_s {
    platform_poller_op_t op;
    platform_poller_fd_t fd;
    void*                ud;
};

enum platform_poller_op_e {
    PLATFORM_POLLER_NO_OP = 0,
    PLATFORM_POLLER_RD_OP = 1,
    PLATFORM_POLLER_WR_OP = 2,
    PLATFORM_POLLER_RW_OP = 3,
};

enum platform_uart_baudrate_e {
    PLATFORM_UART_BAUDRATE_9600,
    PLATFORM_UART_BAUDRATE_19200,
    PLATFORM_UART_BAUDRATE_38400,
    PLATFORM_UART_BAUDRATE_57600,
    PLATFORM_UART_BAUDRATE_115200,
};

enum platform_uart_parity_e { 
    PLATFORM_UART_PARITY_NO,
    PLATFORM_UART_PARITY_ODD,
    PLATFORM_UART_PARITY_EVEN,
};

enum platform_uart_databits_e { 
    PLATFORM_UART_DATABITS_CS7,
    PLATFORM_UART_DATABITS_CS8,
};

enum platform_uart_stopbits_e { 
    PLATFORM_UART_STOPBITS_ONE,
    PLATFORM_UART_STOPBITS_TWO,
};

struct platform_uart_config_s {
    const char* restrict     device;
    platform_uart_baudrate_t baudrate;
    platform_uart_parity_t   parity;
    platform_uart_databits_t databits;
    platform_uart_stopbits_t stopbits;
    size_t                   timeout_ms;
};