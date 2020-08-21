/* ISO/IEC C standard: C99 (ISO/IEC 9899:1999) or later. */
/* gcc -std=c99 -Wall -Wextra main.c -o main.bin */

#include <stdio.h>
#include <stdint.h>

#include "../queue.h"
#include "../queue.c"

#define DATA_SIZE 8U

#if (DATA_SIZE > CQ_SIZE_MAX)
# error "DATA_SIZE > CQ_SIZE_MAX"
#endif

uint8_t data_buf[DATA_SIZE];
cq_t sensor_queue;

int main()
{
    /* Initialize */
    if (cq_init(&sensor_queue, (cq_size_t)sizeof(data_buf)) != CQ_ERR_NONE) {
        /* Error eg. invalid size (CQ_ERR_INIT) */
        fprintf(stderr, "cq_init() error.\n");
        return (int)cq_info_error(&sensor_queue);
    }

    /* Put elements in queue */
    for(size_t i = 0; i < (cq_info_capacity(&sensor_queue) + 2U); ++i) {
        data_buf[cq_enqueue_index(&sensor_queue)] = (uint8_t)i;
        printf("Put: %u\n", data_buf[cq_info_newest_index(&sensor_queue)]);
        printf(" Usage: %ju/%ju\n",
            (uintmax_t)cq_info_usage(&sensor_queue),
            (uintmax_t)cq_info_capacity(&sensor_queue));
    }

    /* Get all elements out */
    printf("\n\n");
    while (cq_info_usage(&sensor_queue) > 0U) {
        printf("Get: %u\n", data_buf[cq_dequeue_index(&sensor_queue)]);
        printf(" Usage: %ju/%ju\n",
            (uintmax_t)cq_info_usage(&sensor_queue),
            (uintmax_t)cq_info_capacity(&sensor_queue));
    }

    return (int)cq_info_error(&sensor_queue);
}
