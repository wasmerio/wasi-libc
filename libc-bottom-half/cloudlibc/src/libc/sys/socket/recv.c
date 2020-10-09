// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/errno.h>

#include <sys/socket.h>

#include <assert.h>
#include <wasi/api.h>
#include <wasi/wasio.h>
#include <errno.h>
#include <stdint.h>

#ifdef __wasilibc_unmodified_upstream
static_assert(MSG_PEEK == __WASI_SOCK_RECV_PEEK, "Value mismatch");
static_assert(MSG_WAITALL == __WASI_SOCK_RECV_WAITALL, "Value mismatch");
#else
static_assert(MSG_PEEK == __WASI_RIFLAGS_RECV_PEEK, "Value mismatch");
static_assert(MSG_WAITALL == __WASI_RIFLAGS_RECV_WAITALL, "Value mismatch");
#endif

ssize_t recv(int socket, void *restrict buffer, size_t length, int flags) {
  // Validate flags.
  if ((flags & ~(MSG_PEEK | MSG_WAITALL)) != 0) {
    errno = EOPNOTSUPP;
    return -1;
  }

  // Prepare input parameters.
  __wasi_iovec_t iov = {.buf = buffer, .buf_len = length};
#ifdef __wasilibc_unmodified_upstream // send/recv
  __wasi_recv_in_t ri = {
      .ri_data = &iov,
      .ri_data_len = 1,
      .ri_flags = flags,
  };
#else
  __wasi_iovec_t *ri_data = &iov;
  size_t ri_data_len = 1;
  __wasi_riflags_t ri_flags = flags;
#endif

  // Perform system call.
#ifdef __wasilibc_unmodified_upstream // send/recv
  __wasi_recv_out_t ro;
  __wasi_errno_t error = __wasi_sock_recv(socket, &ri, &ro);
#else
  size_t ro_datalen;
  __wasi_roflags_t ro_flags;

  cancellation_token_t ct;
  usercontext_t uctx;
  __wasi_errno_t error = wasio_socket_recv(socket,
                                          ri_data, ri_data_len, ri_flags,
                                          &ro_datalen,
                                          &ro_flags,
                                          0, &ct);
  if(error == 0) wasio_wait(&error, &uctx);
#endif
  if (error != 0) {
    errno = errno_fixup_socket(socket, error);
    return -1;
  }
#ifdef __wasilibc_unmodified_upstream // send/recv
  return ro.ro_datalen;
#else
  return ro_datalen;
#endif
}
