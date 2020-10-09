#include <wasi/wasio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol) {
    __wasi_fd_t fd;
    __wasi_errno_t err;
    err = wasio_socket_create(&fd, domain, type, protocol);
    if(err) {
        errno = err;
        return -1;
    }
    return fd;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    __wasi_errno_t err;
    cancellation_token_t ct;
    usercontext_t uctx;
    err = wasio_socket_connect(sockfd, addr, addrlen, 0, &ct);
    if(err == 0) wasio_wait(&err, &uctx);
    if(err) {
        errno = err;
        return -1;
    }
    return 0;
}