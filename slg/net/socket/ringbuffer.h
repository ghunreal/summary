/*
 * Copyright (c) 2014-2015, dennis wang
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RINGBUFFER_API_H
#define RINGBUFFER_API_H

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

typedef struct _ringbuffer_t kringbuffer_t;

#define verify(expr) assert(expr)

/* ������ */
typedef enum _error_e {
	error_ok = 0,
	error_fail,
	error_invalid_parameters,
	error_must_be_shared_channel_ref,
	error_invalid_channel,
	error_invalid_broadcast,
	error_no_memory,
	error_hash_not_found,
	error_recv_fail,
	error_send_fail,
	error_send_patial,
	error_recv_buffer_full,
	error_recv_nothing,
	error_connect_fail,
	error_connect_in_progress,
	error_channel_not_connect,
	error_accept_in_progress,
	error_bind_fail,
	error_listen_fail,
	error_ref_nonzero,
	error_loop_fail,
	error_loop_attached,
	error_loop_not_found,
	error_loop_impl_init_fail,
	error_thread_start_fail,
	error_already_close,
	error_impl_add_channel_ref_fail,
	error_broadcast_not_found,
	error_getpeername,
	error_getsockname,
	error_not_correct_domain,
	error_multiple_start,
	error_not_connected,
	error_logger_write,
	error_set_tls_fail,
	error_rpc_dup_id,
	error_rpc_unknown_id,
	error_rpc_unknown_type,
	error_rpc_cb_fail,
	error_rpc_cb_fail_close,
	error_rpc_cb_close,
	error_rpc_next,
	error_rpc_not_enough_bytes,
	error_rpc_vector_out_of_bound,
	error_rpc_marshal_fail,
	error_rpc_unmarshal_fail,
	error_rpc_map_error_key_or_value,
	error_recvbuffer_not_enough,
	error_recvbuffer_locked,
	error_stream_enable,
	error_stream_disable,
	error_stream_flush,
	error_stream_buffer_overflow,
	error_trie_not_found,
	error_trie_key_exist,
	error_trie_for_each_fail,
	error_ip_filter_open_fail,
	error_router_wire_not_found,
	error_router_wire_exist,
	error_node_not_found,
	error_node_exist,
	error_node_ip_filter,
	error_node_invalid_msg,
	error_node_timeout,
	error_ringbuffer_not_found,
	error_node_argv_invalid,
	error_getaddrinfo_fail,
} knet_error_e;

/**
 * ����һ��ringbuffer
 * @param size ��󳤶�
 * @return kringbuffer_tʵ��
 */
extern kringbuffer_t* ringbuffer_create(uint32_t size);

/**
 * ����ringbuffer
 * @param rb kringbuffer_tʵ��
 */
extern void ringbuffer_destroy(kringbuffer_t* rb);

/**
 * ��ȡ�����
 * @param rb kringbuffer_tʵ��
 * @param buffer д�뻺����ָ��
 * @param size д�뻺��������
 * @return ʵ�ʶ����ֽ���
 */
extern uint32_t ringbuffer_read(kringbuffer_t* rb, char* buffer, uint32_t size);

/**
* ���
* @param rb kringbuffer_tʵ��
* @param size ��Ҫ������ֽ���
* @return ʵ��������ֽ���
*/
extern uint32_t ringbuffer_remove(kringbuffer_t* rb, uint32_t size);

/**
 * д��
 * @param rb kringbuffer_tʵ��
 * @param buffer д�뻺����ָ��
 * @param size д�뻺��������
 * @return ʵ��д���ֽ���
 */
extern uint32_t ringbuffer_write(kringbuffer_t* rb, const char* buffer, uint32_t size);

/**
 * �滻
 * @param rb kringbuffer_tʵ��
 * @param pos �滻����ʼλ��
 * @param buffer д�뻺����ָ��
 * @param size д�뻺��������
 * @return ʵ��д���ֽ���
 */
extern uint32_t ringbuffer_replace(kringbuffer_t* rb, uint32_t pos, const char* buffer, uint32_t size);

/**
 * ��ȡ�������
 * @param rb kringbuffer_tʵ��
 * @param buffer д�뻺����ָ��
 * @param size д�뻺��������
 * @return ʵ�ʶ����ֽ���
 */
extern uint32_t ringbuffer_copy(kringbuffer_t* rb, char* buffer, uint32_t size);

/**
 * �����ȡ
 * @param rb kringbuffer_tʵ��
 * @param pos ��ȡ����ʼλ��
 * @param buffer д�뻺����ָ��
 * @param size д�뻺��������
 * @return ʵ��д���ֽ���
 */
extern uint32_t ringbuffer_copy_random(kringbuffer_t* rb, uint32_t pos, char* buffer, uint32_t size);

/**
 * ����ָ����Ŀ�꣬������λ��
 * @param rb kringbuffer_tʵ��
 * @param target Ŀ���ַ���
 * @param size λ��
 * @retval error_ok �ҵ�
 * @retval ���� δ�ҵ�
 */
extern uint32_t ringbuffer_find(kringbuffer_t* rb, const char* target, uint32_t* size);

/**
 * ȡ�ÿɶ��ֽ���
 * @param rb kringbuffer_tʵ��
 * @return �ɶ��ֽ���
 */
extern uint32_t ringbuffer_available(kringbuffer_t* rb);

/**
 * ������пɶ��ֽ�
 * @param rb kringbuffer_tʵ��
 * @retval error_ok �ɹ�
 * @retval ���� ʧ��
 */
extern int ringbuffer_eat_all(kringbuffer_t* rb);

/**
 * ���ָ�����ȵĿɶ��ֽ�
 * @param rb kringbuffer_tʵ��
 * @param size ��Ҫ����ĳ���
 * @retval error_ok �ɹ�
 * @retval ���� ʧ��
 */
extern int ringbuffer_eat(kringbuffer_t* rb, uint32_t size);

/**
 * ȡ�÷��ƻ�������ַ�����ɶ��ֽ���
 * @param rb kringbuffer_tʵ��
 * @return ���ƻ�������ַ�����ɶ��ֽ���
 */
extern uint32_t ringbuffer_read_lock_size(kringbuffer_t* rb);

/**
 * ȡ�ÿɶ�������ָֹ��
 * @param rb kringbuffer_tʵ��
 * @return �ɶ�������ָֹ��
 */
extern char* ringbuffer_read_lock_ptr(kringbuffer_t* rb);

/**
 * �ύ������Ѿ��������ֽ�
 * @param rb kringbuffer_tʵ��
 * @param size �Ѿ��������ֽ���
 */
extern void ringbuffer_read_commit(kringbuffer_t* rb, uint32_t size);

/**
 * ���������
 * @param rb kringbuffer_tʵ��
 */
extern void ringbuffer_read_unlock(kringbuffer_t* rb);

/**
 * ���ⴰ�� - ȡ�÷��ƻ�������ַ�����ɶ��ֽ���
 * @param rb kringbuffer_tʵ��
 * @return ���ƻ�������ַ�����ɶ��ֽ���
 */
extern uint32_t ringbuffer_window_read_lock_size(kringbuffer_t* rb);

/**
 * ���ⴰ�� - ȡ�ÿɶ�������ָֹ��
 * @param rb kringbuffer_tʵ��
 * @return �ɶ�������ָֹ��
 */
extern char* ringbuffer_window_read_lock_ptr(kringbuffer_t* rb);

/**
 * ���ⴰ�� - �ύ�Ѿ��������ֽڣ��������
 * @param rb kringbuffer_tʵ��
 * @param size �Ѿ��������ֽ���
 */
void ringbuffer_window_read_commit(kringbuffer_t* rb, uint32_t size);

/**
 * ȡ�÷��ƻؿ�����д�����󳤶�
 * @param rb kringbuffer_tʵ��
 * @return ���ƻؿ�����д�����󳤶�
 */
extern uint32_t ringbuffer_write_lock_size(kringbuffer_t* rb);

/**
 * ȡ�ÿ�д��ʼָ��
 * @param rb kringbuffer_tʵ��
 * @return ��д��ָֹ��
 */
extern char* ringbuffer_write_lock_ptr(kringbuffer_t* rb);

/**
 * �ύ�ɹ�д����ֽ���
 * @param rb kringbuffer_tʵ��
 * @param size �ɹ�д����ֽ���
 */
extern void ringbuffer_write_commit(kringbuffer_t* rb, uint32_t size);

/**
 * ���д����
 * @param rb kringbuffer_tʵ��
 */
extern void ringbuffer_write_unlock(kringbuffer_t* rb);

/**
 * ��
 * @param rb kringbuffer_tʵ��
 * @retval 0 δ��
 * @retval ���� ��
 */
extern int ringbuffer_full(kringbuffer_t* rb);

/**
 * ��
 * @param rb kringbuffer_tʵ��
 * @retval 0 �ǿ�
 * @retval ���� ��
 */
extern int ringbuffer_empty(kringbuffer_t* rb);

/**
 * ȡ����󳤶�
 * @param rb kringbuffer_tʵ��
 * @return ��󳤶�
 */
extern uint32_t ringbuffer_get_max_size(kringbuffer_t* rb);

/**
 * �����ݴ�ӡ����Ļ
 * @param rb kringbuffer_tʵ��
 */
extern void ringbuffer_print_stdout(kringbuffer_t* rb);

#endif /* RINGBUFFER_API_H */
