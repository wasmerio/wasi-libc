#include <wasi/wasio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

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

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    __wasi_errno_t err = wasio_socket_bind(sockfd, addr, addrlen);
    if(err) {
        errno = err;
        return -1;
    }
    return 0;
}

int listen(int sockfd, int backlog) {
    __wasi_errno_t err = wasio_socket_listen(sockfd);
    if(err) {
        errno = err;
        return -1;
    }
    return 0;
}

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    __wasi_errno_t err;
    cancellation_token_t ct;
    usercontext_t uctx;
    __wasi_fd_t conn;

    err = wasio_socket_pre_accept(sockfd, 0, &ct);
    if(err == 0) wasio_wait(&err, &uctx);
    if(err == 0) err = wasio_socket_accept(&conn, addr, *addrlen);
    if(err) {
        errno = err;
        return -1;
    }
    return conn;
}

int accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags) {
    return accept(sockfd, addr, addrlen);
}

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
    return 0;
}

int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int err = wasio_socket_local_addr(sockfd, addr, addrlen);
    if(err) {
        errno = err;
        return -1;
    }
    return 0;
}

int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int err = wasio_socket_remote_addr(sockfd, addr, addrlen);
    if(err) {
        errno = err;
        return -1;
    }
    return 0;
}