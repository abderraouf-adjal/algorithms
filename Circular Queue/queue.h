/* SPDX-License-Identifier: ISC */
/**
 * Copyright (c) 2020 Abderraouf Adjal
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * Implementation of a circular queue to use with any type of buffer/array.
 * This code is made for embedded environments with code safety in mind.
 * If multi-threading is needed, the OS/environment threading API
 * should be used to handle locking.
 * 
 * Customization: `cq_size_t` type and its `CQ_SIZE_MAX` can be modified
 * for the queue maximum capacity and/or performance.
 */

/* Semantic versioning: 0.1.0 */
/* Version date (ISO-8601): 2020-08-01 */
/* ISO/IEC C standard: Any version. */

#ifndef CQ_H
#define CQ_H 1


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#define CQ_SIZE_MAX UINT_FAST8_MAX /* Type `cq_size_t` maximum value. */
typedef uint_fast8_t cq_size_t; /* An unsigned type for size and indices. */
typedef uint_fast8_t cq_info_t; /* An unsigned type for errors flag. */

/* API and behavior semantic versioning. */
#define CQ_VERSION "0.1.0"

/* For errors management; Type: cq_info_t; Order: 2^n */
#define CQ_ERR_NULL ((cq_info_t)0U)  /* A try to initialize NULL pointer. */
#define CQ_ERR_NONE ((cq_info_t)1U)  /* No error detected. */
#define CQ_ERR_SIZE ((cq_info_t)2U)  /* Initialize with invalid size. */
#define CQ_ERR_EMPTY ((cq_info_t)4U) /* Attempt to dequeue empty queue. */

typedef struct {
    cq_size_t tail; /* Index of the oldest element. */
    cq_size_t head; /* Index of the newest element. */
    cq_size_t size; /* Current number of elements. */
    cq_size_t size_max; /* Maximum number of elements. */
    cq_size_t size_max_mn1_; /* (size_max - 1) for optimization. */
    cq_info_t error; /* Errors flag. */
} cq_t;


/**
 * Initialize a `cq_t`.
 * 
 * It will rise the error `CQ_ERR_INIT` if `size_max` was not positive,
 * or `ctx` was NULL.
 * 
 * Return error flag:
 *     - CQ_ERR_NONE: Successful initialization, no error.
 *     - CQ_ERR_NULL: `ctx` is NULL pointer.
 *     - CQ_ERR_SIZE: Invalid size (non-positive).
 */
cq_info_t cq_init(cq_t *ctx, cq_size_t size_max);

/**
 * Return the array index to insert new element.
 */
cq_size_t cq_enqueue_index(cq_t *ctx);

/**
 * Return the array index to get oldest element.
 * 
 * It will rise the error `CQ_ERR_EMPTY` if the queue is empty.
 */
cq_size_t cq_dequeue_index(cq_t *ctx);

/**
 * Reset `cq_t` to abandon any existing element.
 */
void cq_reset(cq_t *ctx);

/**
 * Clear `cq_t` errors flag.
 */
void cq_reset_error(cq_t *ctx);


/**
 * Return array index of the next element to enqueue
 * without counting an enqueue.
 */
cq_size_t cq_info_next_index(const cq_t *ctx);

/**
 * Return array index of the newest element.
 */
cq_size_t cq_info_newest_index(const cq_t *ctx);

/**
 * Return array index of the oldest element.
 */
cq_size_t cq_info_oldest_index(const cq_t *ctx);

/**
 * Return current number of elements of `cq_t`.
 */
cq_size_t cq_info_usage(const cq_t *ctx);

/**
 * Return maximum number of elements of `cq_t`.
 */
cq_size_t cq_info_capacity(const cq_t *ctx);

/**
 * Return error flag:
 *     - CQ_ERR_NONE: Successful initialization, no error.
 *     - CQ_ERR_NULL: `ctx` is NULL pointer.
 *     - CQ_ERR_SIZE: Invalid size (non-positive).
 *     - CQ_ERR_EMPTY: Attempt to dequeue empty queue.
 */
cq_info_t cq_info_error(const cq_t *ctx);


#ifdef __cplusplus
}
#endif

#endif /* CQ_H */
