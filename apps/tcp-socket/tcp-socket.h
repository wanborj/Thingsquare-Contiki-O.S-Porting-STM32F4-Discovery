/*
 * Copyright (c) 2012, Thingsquare, http://www.thingsquare.com/.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

struct tcp_socket;

typedef enum {
  TCP_SOCKET_CONNECTED,
  TCP_SOCKET_CLOSED,
  TCP_SOCKET_TIMEDOUT,
  TCP_SOCKET_ABORTED
} tcp_socket_event_t;

/**
 * \brief      TCP input callback function
 * \param s    A pointer to a TCP socket
 * \param ptr  A user-defined pointer
 * \param input_data_ptr A pointer to the incoming data
 * \param input_data_len The length of the incoming data
 * \return     The function should return the number of bytes to leave in the input buffer
 *
 *             The TCP socket input callback function gets
 *             called whenever there is new data on the socket. The
 *             function can choose to either consume the data
 *             directly, or leave it in the buffer for later. The
 *             function must return the amount of data to leave in the
 *             buffer. I.e., if the callback function consumes all
 *             incoming data, it should return 0.
 */

typedef int (* tcp_socket_input_callback_t)(struct tcp_socket *s,
					    void *ptr,
					    const uint8_t *input_data_ptr,
					    int input_data_len);


/**
 * \brief      TCP event callback function
 * \param s    A pointer to a TCP socket
 * \param ptr  A user-defined pointer
 * \param event The event number
 *
 *             The TCP socket event callback function gets
 *             called whenever there is an event on a socket, such as
 *             the socket getting connected or closed.
 */
typedef void (* tcp_socket_event_callback_t)(struct tcp_socket *s,
					    void *ptr,
					    tcp_socket_event_t event);

struct tcp_socket {
  struct tcp_socket *next;

  tcp_socket_input_callback_t input_callback;
  tcp_socket_event_callback_t event_callback;
  void *ptr;

  struct process *p;

  uint8_t *input_data_ptr;
  uint8_t *output_data_ptr;

  uint16_t input_data_maxlen;
  uint16_t input_data_len;
  uint16_t output_data_maxlen;
  uint16_t output_data_len;
  uint16_t output_data_send_nxt;

  uint8_t is_listening;
  uint16_t listen_port;
};


void tcp_socket_register(struct tcp_socket *s, void *ptr,
                  uint8_t *input_databuf, int input_databuf_len,
                  uint8_t *output_databuf, int output_databuf_len,
                  tcp_socket_input_callback_t input_callback,
                  tcp_socket_event_callback_t event_callback);

int tcp_socket_connect(struct tcp_socket *s,
                uip_ipaddr_t *ipaddr,
                uint16_t port);

int tcp_socket_listen(struct tcp_socket *s,
               uint16_t port);

int tcp_socket_send(struct tcp_socket *s,
             const uint8_t *dataptr,
             int datalen);

int tcp_socket_send_str(struct tcp_socket *s,
                 const char *strptr);

int tcp_socket_close(struct tcp_socket *s);

#endif /* TCP_SOCKET_H */