/* ISO/IEC C standard: C11 (ISO/IEC 9899:2011) or later. */
/* gcc -std=c11 -Wall -Wextra main_mt.c -o main_mt.bin -lpthread */

#include <stdio.h>
#include <stdint.h>

#include <threads.h>

#include "../queue.h"
#include "../queue.c"


#define DATA_SIZE 8U

#if (DATA_SIZE > CQ_SIZE_MAX)
# error "DATA_SIZE > CQ_SIZE_MAX"
#endif

uint8_t data_buf[DATA_SIZE];
cq_t sensor_queue;

#define THREADS_NUM DATA_SIZE
mtx_t enqueue_mtx;


int enqueue_th(__attribute__((unused)) void *arg)
{
    if (mtx_lock(&enqueue_mtx) != thrd_success) {
        fprintf(stderr, "enqueue_th mtx_lock(&enqueue_mtx) error.\n");
        return thrd_error;
    }

    /* Put element in queue */
    uint8_t elm = (uint8_t)cq_info_usage(&sensor_queue);
    data_buf[cq_enqueue_index(&sensor_queue)] = elm;

    printf("Put [%ju/%ju]: %u\n",
        (uintmax_t)cq_info_usage(&sensor_queue),
        (uintmax_t)cq_info_capacity(&sensor_queue),
        data_buf[cq_info_newest_index(&sensor_queue)]);

    if (mtx_unlock(&enqueue_mtx) != thrd_success) {
        fprintf(stderr, "enqueue_th mtx_unlock(&enqueue_mtx) error.\n");
        return thrd_error;
    }

    return (int)cq_info_error(&sensor_queue);
}

int main()
{
    /* Initialize threading */
    thrd_t enq_th[THREADS_NUM];
    int enq_th_res[THREADS_NUM];
    if (mtx_init(&enqueue_mtx, mtx_plain) != thrd_success) {
        return -2;
    }

    /* Initialize queue */
    if (cq_init(&sensor_queue, (cq_size_t)sizeof(data_buf)) != CQ_ERR_NONE) {
        /* Error eg. invalid size (CQ_ERR_INIT) */
        fprintf(stderr, "cq_init() error.\n");
        return -1;
    }

    /* Run threads. */
    for (uint8_t i = 0U; i < THREADS_NUM; ++i) {
        if (thrd_create(&enq_th[i], enqueue_th, NULL) != thrd_success) {
            fprintf(stderr, "thrd_create(&enq_th[i], enqueue_th, &i) error.\n");
            return -6;
        }
    }
    /* Wait for all threads to finish. */
    for (uint8_t i = 0U; i < THREADS_NUM; ++i) {
        if (thrd_join(enq_th[i], &enq_th_res[i]) != thrd_success) {
            fprintf(stderr, "thrd_join(enq_th[i], &enq_th_res[i]) error.\n");
            return -7;
        }
    }
    
    /* Flush elements from the queue. */
    while (cq_info_usage(&sensor_queue) > 0U) {
        /* There are element(s) in the queue. */
        printf("<< Get [%ju/%ju]: ",
            (uintmax_t)cq_info_usage(&sensor_queue),
            (uintmax_t)cq_info_capacity(&sensor_queue));
        printf("%u\n", data_buf[cq_dequeue_index(&sensor_queue)]);
        
    }
    if (cq_info_error(&sensor_queue) != CQ_ERR_NONE) {
        fprintf(stderr, "dequeue_th_cnd(): cq_info_error(&sensor_queue) != CQ_ERR_NONE error.\n");
    }

    return (int)cq_info_error(&sensor_queue);
}
