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

/* ISO/IEC C standard: Any version. */

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* For `NULL` */
#include "queue.h"


cq_info_t cq_init(cq_t *ctx, cq_size_t size_max)
{
    if (ctx == NULL) {
        return CQ_ERR_NULL;
    }

    if (size_max >= 1U) {
        ctx->error = CQ_ERR_NONE;
        ctx->size_max = size_max;
        ctx->size_max_mn1_ = size_max - 1U;
        ctx->head = size_max;
        
    }
    else {
        /* Handle invalid size; size_max must be positive. */
        ctx->error = CQ_ERR_SIZE;
        ctx->size_max = 1U;
        ctx->size_max_mn1_ = 0U;
        ctx->head = 1U;
    }
    ctx->tail = 0U;
    ctx->size = 0U;

    return ctx->error;
}

cq_size_t cq_enqueue_index(cq_t *ctx)
{
    /* Maintain next new element index. */
    if (ctx->head < ctx->size_max_mn1_) {
        ++ctx->head;
    }
    else {
        ctx->head = 0U;
    }

    /* Maintain capacity usage and oldest element index. */
    if (ctx->size < ctx->size_max) {
        ++ctx->size;
    }
    else if (ctx->tail < ctx->size_max_mn1_) {
        ++ctx->tail; /* Queue is full, push the read index. */
    }
    else {
        ctx->tail = 0U; /* Queue is full, push the read index. */
    }

    return ctx->head;
}

cq_size_t cq_dequeue_index(cq_t *ctx)
{
    const cq_size_t index = ctx->tail;
    
    /* Maintain capacity usage and oldest element index. */
    if (ctx->size > 0U) {
        --ctx->size;
        if (ctx->tail < ctx->size_max_mn1_) {
            ++ctx->tail;
        }
        else {
            ctx->tail = 0U;
        }
    }
    else {
        /* Handle empty queue. */
        ctx->error = CQ_ERR_EMPTY;
    }
    
    return index;
}

void cq_reset(cq_t *ctx)
{
    ctx->error = CQ_ERR_NONE;
    ctx->head = ctx->size_max;
    ctx->tail = 0U;
    ctx->size = 0U;
}

void cq_reset_error(cq_t *ctx)
{
    ctx->error = CQ_ERR_NONE;
}


cq_size_t cq_info_next_index(const cq_t *ctx)
{
    if (ctx->head < ctx->size_max_mn1_) {
        return (ctx->head + 1U);
    }
    else {
        return 0U;
    }
}

cq_size_t cq_info_newest_index(const cq_t *ctx)
{
    return ctx->head;
}

cq_size_t cq_info_oldest_index(const cq_t *ctx)
{
    return ctx->tail;
}

cq_size_t cq_info_usage(const cq_t *ctx)
{
    return ctx->size;
}

cq_size_t cq_info_capacity(const cq_t *ctx)
{
    return ctx->size_max;
}

cq_info_t cq_info_error(const cq_t *ctx)
{
    if (ctx == NULL) {
        return CQ_ERR_NULL;
    }

    return ctx->error;
}


#ifdef __cplusplus
}
#endif
