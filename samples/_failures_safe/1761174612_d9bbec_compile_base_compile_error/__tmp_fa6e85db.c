//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t algorithm;
    uint8_t checksum;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t offset;
    uint8_t *data;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

struct InputBuffer {
    uint8_t *data;
    uint32_t size;
    uint32_t capacity;
};

struct OutputBuffer {
    uint8_t *data;
    uint32_t size;
    uint32_t capacity;
};

void init_input_buffer(struct InputBuffer *buf, uint32_t capacity) {
    if (capacity == 0 || capacity > 1024 * 1024) {
        capacity = 4096;
    }
    buf->data = malloc(capacity);
    if (buf->data == NULL) {
        exit(1);
    }
    buf->size = 0;
    buf->capacity = capacity;
}

void free_input_buffer(struct InputBuffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

void init_output_buffer(struct OutputBuffer *buf, uint32_t capacity) {
    if (capacity == 0 || capacity > 1024 * 1024) {
        capacity = 4096;
    }
    buf->data = malloc(capacity);
    if (buf->data == NULL) {
        exit(1);
    }
    buf->size = 0;
    buf->capacity = capacity;
}

void free_output_buffer(struct OutputBuffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

void init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (max_blocks == 0 || max_blocks > 1000) {
        max_blocks = 10;
    }
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        exit(1);
    }
    ctx->block_count = 0;
    ctx->max_blocks = max_blocks;
    memset(&ctx->header, 0, sizeof(struct CompressionHeader));
}

void free_compression_context(struct CompressionContext *ctx) {
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].data != NULL) {
            free(ctx->blocks[i].data);
        }
    }
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
    ctx->block_count = 0;
    ctx->max_blocks = 0;
}

uint8_t calculate_checksum(const uint8_t *data, uint32_t size) {
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx, const struct InputBuffer *input) {
    if (ctx == NULL || input == NULL || input->size == 0) {
        return 0;
    }
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = input->size;
    ctx->header.algorithm = 1;
    uint32_t block_size = 64;
    uint32_t remaining = input->size;
    uint32_t offset = 0;
    while (remaining > 0 && ctx->block_count < ctx->max_blocks) {
        uint32_t current_block_size = (remaining < block_size) ? remaining : block_size;
        struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
        block->block_size = current_block_size;
        block->offset = offset;
        block->data = malloc(current_block_size);
        if (block->data == NULL) {
            return 0;
        }
        for (uint32_t i = 0; i < current_block_size; i++) {
            block->data[i] = input->data[offset + i];
        }
        offset += current_block_size;
        remaining -= current_block_size;
        ctx->block_count++;
    }
    ctx->header.compressed_size = offset;
    ctx->header.checksum = calculate_checksum(input->data, input->size);
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, struct OutputBuffer *output) {
    if (ctx == NULL || output == NULL || ctx->block_count == 0) {
        return 0;
    }
    if (output->capacity < ctx->header.original_size) {
        uint8_t *new_data = realloc(output->data, ctx->header.original_size);
        if (new_data == NULL) {
            return 0;
        }
        output->data = new_data;
        output->capacity = ctx->header.original_size;
    }
    output->size = ctx->header.original_size;
    uint32_t total_copied = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        const struct