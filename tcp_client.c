/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/time.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "thread/include/kernerl.h"

// #if !defined(TEST_TCP_SERVER_IP)
// #error TEST_TCP_SERVER_IP not defined
// #endif

#define TCP_PORT 4242
#define DEBUG_printf printf
#define BUF_SIZE 2048

#define TEST_ITERATIONS 2
#define POLL_TIME_S 5



#if 0
static void dump_bytes(const uint8_t *bptr, uint32_t len) {
    unsigned int i = 0;

    printf("dump_bytes %d", len);
    for (i = 0; i < len;) {
        if ((i & 0x0f) == 0) {
            printf("\n");
        } else if ((i & 0x07) == 0) {
            printf(" ");
        }
        printf("%02x ", bptr[i++]);
    }
    printf("\n");
}
#define DUMP_BYTES dump_bytes
#else
#define DUMP_BYTES(A,B)
#endif

typedef struct TCP_CLIENT_T_ {
    struct tcp_pcb *tcp_pcb;
    ip_addr_t remote_addr;
    uint8_t buffer[BUF_SIZE];
    int buffer_len;
    int sent_len;
    bool complete;
    int run_count;
    bool connected;
} TCP_CLIENT_T;

static err_t tcp_client_close(void *arg) {
    printf("Start tcp_client_close\n");
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    err_t err = ERR_OK;
    if (state->tcp_pcb != NULL) {
        tcp_arg(state->tcp_pcb, NULL);
        tcp_poll(state->tcp_pcb, NULL, 0);
        tcp_sent(state->tcp_pcb, NULL);
        tcp_recv(state->tcp_pcb, NULL);
        tcp_err(state->tcp_pcb, NULL);
        err = tcp_close(state->tcp_pcb);
        if (err != ERR_OK) {
            DEBUG_printf("close failed %d, calling abort\n", err);
            tcp_abort(state->tcp_pcb);
            err = ERR_ABRT;
        }
        state->tcp_pcb = NULL;
    }
     printf("End tcp_client_close\n");
    return err;
}

// Called with results of operation
static err_t tcp_result(void *arg, int status) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    if (status == 0) {
        DEBUG_printf("****************test success****************************\n");
    } else {
        DEBUG_printf("test failed %d\n", status);
    }
    return tcp_client_close(arg);
}

static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    state->sent_len += len;

    if (state->sent_len >= BUF_SIZE) {

        state->run_count++;
        DEBUG_printf("****************[%d]****************************\n", state->run_count);
        if (state->run_count >= TEST_ITERATIONS) {
            tcp_result(arg, 0);
            return ERR_OK;
        }

        // We should receive a new buffer from the server
        state->buffer_len = 0;
        state->sent_len = 0;
        DEBUG_printf("Waiting for buffer from server(tcp_client_sent)\n");
    }
    return ERR_OK;
}

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    if (err != ERR_OK) {
        printf("connect failed %d\n", err);
        return tcp_result(arg, err);
    }
    state->connected = true;
    return ERR_OK;
}

static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb) {
    return tcp_result(arg, -1); // no response is an error?
}

static void tcp_client_err(void *arg, err_t err) {
    if (err != ERR_ABRT) {
        tcp_result(arg, err);
    }
}

err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    if (!p) {
        return tcp_result(arg, -1);
    }
    // this method is callback from lwIP, so cyw43_arch_lwip_begin is not required, however you
    // can use this method to cause an assertion in debug mode, if this method is called when
    // cyw43_arch_lwip_begin IS needed
    cyw43_arch_lwip_check();
    if (p->tot_len > 0) {
        DEBUG_printf("****************recv %d err %d****************\n", p->tot_len, err);
        for (struct pbuf *q = p; q != NULL; q = q->next) {
            DUMP_BYTES(q->payload, q->len);
        }
        // Receive the buffer
        const uint16_t buffer_left = BUF_SIZE - state->buffer_len;
        state->buffer_len += pbuf_copy_partial(p, state->buffer + state->buffer_len,
                                               p->tot_len > buffer_left ? buffer_left : p->tot_len, 0);
        tcp_recved(tpcb, p->tot_len);
    }
    pbuf_free(p);

    // If we have received the whole buffer, send it back to the server
    if (state->buffer_len == BUF_SIZE) {
        DEBUG_printf("****************Writing %d bytes to server****************\n", state->buffer_len);
        err_t err = tcp_write(tpcb, state->buffer, state->buffer_len, TCP_WRITE_FLAG_COPY);
        if (err != ERR_OK) {
            DEBUG_printf("Failed to write data %d\n", err);
            return tcp_result(arg, -1);
        }
    }
    return ERR_OK;
}

static bool tcp_client_open(void *arg) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    DEBUG_printf("Connecting to %s port %u\n", ip4addr_ntoa(&state->remote_addr), TCP_PORT);
    state->tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(&state->remote_addr));
    if (!state->tcp_pcb) {
        DEBUG_printf("failed to create pcb\n");
        return false;
    }

    tcp_arg(state->tcp_pcb, state);
    tcp_poll(state->tcp_pcb, tcp_client_poll, POLL_TIME_S * 2);
    tcp_sent(state->tcp_pcb, tcp_client_sent);
    tcp_recv(state->tcp_pcb, tcp_client_recv);
    tcp_err(state->tcp_pcb, tcp_client_err);

    state->buffer_len = 0;

    // cyw43_arch_lwip_begin/end should be used around calls into lwIP to ensure correct locking.
    // You can omit them if you are in a callback from lwIP. Note that when using pico_cyw_arch_poll
    // these calls are a no-op and can be omitted, but it is a good practice to use them in
    // case you switch the cyw43_arch type later.
    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(state->tcp_pcb, &state->remote_addr, TCP_PORT, tcp_client_connected);
    cyw43_arch_lwip_end();
    return err == ERR_OK;
}

// Perform initialisation
static TCP_CLIENT_T* tcp_client_init(void) {
    printf("Start tcp_client_init \n");
    TCP_CLIENT_T *state = calloc(1, sizeof(TCP_CLIENT_T));
    if (!state) {
        DEBUG_printf("failed to allocate state\n");
        return NULL;
    }
    ip4addr_aton("192.168.1.18", &state->remote_addr);
    return state;
}

void run_tcp_client_test(void) {
    printf("Start run_tcp_client_test\n");
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");
    int err = cyw43_arch_wifi_connect_timeout_ms("Buffalo-2G-CF00", "nefucaab7b63a", CYW43_AUTH_WPA2_AES_PSK, 30000);
    if (err) {
        printf("failed to connect.%d\n",err);
        //return 1;
    } else {
        printf("Connected.\n");
    }
    
    
    TCP_CLIENT_T *state = tcp_client_init();
    
    if (!state) {
        return;
    }
    if (!tcp_client_open(state)) {
        tcp_result(state, -1);
        cyw43_arch_deinit();
        return;
    }
    while(!state->complete) {
        printf("While\n");
        // the following #ifdef is only here so this same example can be used in multiple modes;
        // you do not need it in your code
#if PICO_CYW43_ARCH_POLL
        // if you are using pico_cyw43_arch_poll, then you must poll periodically from your
        // main loop (not from a timer) to check for Wi-Fi driver or lwIP work that needs to be done.
        cyw43_arch_poll();
        // you can poll as often as you like, however if you have nothing else to do you can
        // choose to sleep until either a specified time, or cyw43_arch_poll() has work to do:
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
#else
        // if you are not using pico_cyw43_arch_poll, then WiFI driver and lwIP work
        // is done via interrupt in the background. This sleep is just an example of some (blocking)
        // work you might be doing.
        sleep_ms(1000);
#endif
    }
    free(state);
    printf("End run_tcp_client_test\n");
    cyw43_arch_deinit();
}


 //uint32_t *stack_0[256];
#define STACK_SIZE 2160
uint32_t *stack_1[STACK_SIZE/sizeof(uint32_t)];
uint32_t *stack_2[STACK_SIZE/sizeof(uint32_t)];
uint32_t *stack_3[STACK_SIZE/sizeof(uint32_t)];
#include <stdint.h>

void test1(void){
    int i = 0;  
    for(;;)
    {      
        printf("test1[%d]\n",i);
        tsk_sleep(100);
        i++;
    } 
    return;
}
void test2(void){
    int i = 0;
    for(;;)
    {
        printf("test2[%d]\n",i);
        tsk_sleep(100);
        i++;
    }
    return;
 }
void test3(void){
    int i = 0;
    for(;;)
    {
        printf("test3[%d]\n",i);
        tsk_sleep(100);
        i++;
    }
    return;
}
void End(){
    printf("End\n");
}
int main(){
    
    if (cyw43_arch_init()) {
        DEBUG_printf("failed to initialise\n");
        return 1;
    }
    stdio_init_all();
    sleep_ms(4000);
    init();
    create_tsk(stack_1, sizeof(stack_1), test1,End);
    //  create_tsk(stack_2, sizeof(stack_2), test2,End);
    create_tsk(stack_3, sizeof(stack_3), test3,End);
    tsk_run();
    // id = create_tsk(stack_2, sizeof(stack_2), test2,End);
    // printf ("%d\n",id);
    // tsk_run(id);
    // create_tsk(stack_2, sizeof(stack_2), test2,End);
    //run_tcp_client_test();
    while (1);
    
    return 0;

}

void isr_hardfault(){
    printf("hardFalut\n");

}

// int usermain() {
//     // int id = create_tsk(test1, stack_1, sizeof(stack_1),10, 1);
//     // printf("%d\n",id);
//     // tsk_run(id);
//     // create_tsk(test2, stack_2, sizeof(stack_2),10, 2);
//     // printf("%d\n",id);
//     // tsk_run(id);
//     // printf("End main\n");
//     // run_tcp_client_test();
//     // cyw43_arch_deinit();
//     // return 0;
// }