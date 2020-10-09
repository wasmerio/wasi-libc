#ifndef __wasio_api_h
#define __wasio_api_h

#include <wasi/api.h>

#define WASIO_IMPORT(name) __attribute__((__import_module__("wasio_unstable"), __import_name__(name)))

typedef uint64_t usercontext_t;
typedef uint64_t cancellation_token_t;
typedef int32_t __wasio_socket_domain_t;
typedef int32_t __wasio_socket_type_t;
typedef int32_t __wasio_socket_protocol_t;

WASIO_IMPORT("wait") __wasi_errno_t wasio_wait(__wasi_errno_t *error_out, usercontext_t *user_context_out);
WASIO_IMPORT("cancel") __wasi_errno_t wasio_cancel(cancellation_token_t token);

WASIO_IMPORT("socket_create") __wasi_errno_t wasio_socket_create(
    __wasi_fd_t *fd_out,
    __wasio_socket_domain_t domain,
    __wasio_socket_type_t ty,
    __wasio_socket_protocol_t protocol
);

WASIO_IMPORT("socket_connect") __wasi_errno_t wasio_socket_connect(
    __wasi_fd_t fd,
    const void *sockaddr,
    const uint32_t sockaddr_size,
    usercontext_t uctx,
    cancellation_token_t *ct_out
);

WASIO_IMPORT("socket_send") __wasi_errno_t wasio_socket_send(
    __wasi_fd_t fd,

    /**
     * List of scatter/gather vectors to which to retrieve data
     */
    const __wasi_ciovec_t *si_data,

    /**
     * The length of the array pointed to by `si_data`.
     */
    size_t si_data_len,

    /**
     * Message flags.
     */
    __wasi_siflags_t si_flags,

    /**
     * Number of bytes transmitted.
     */
    __wasi_size_t *so_datalen,

    usercontext_t uctx,
    cancellation_token_t *ct_out
);

WASIO_IMPORT("socket_recv") __wasi_errno_t wasio_socket_recv(
    __wasi_fd_t fd,

    /**
     * List of scatter/gather vectors to which to store data.
     */
    const __wasi_iovec_t *ri_data,

    /**
     * The length of the array pointed to by `ri_data`.
     */
    size_t ri_data_len,

    /**
     * Message flags.
     */
    __wasi_riflags_t ri_flags,

    /**
     * Number of bytes stored in ri_data.
     */
    __wasi_size_t *ro_datalen,
    /**
     * Message flags.
     */
    __wasi_roflags_t *ro_flags,

    usercontext_t uctx,
    cancellation_token_t *ct_out
);


#undef WASIO_IMPORT

#endif
