//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
};

struct CompressionBlock {
    uint8_t *data;
    size_t size;
    size_t capacity;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock input;
    struct CompressionBlock output;
    int status;
};

void init_compression_block(struct CompressionBlock *block, size_t initial_capacity) {
    block->data = NULL;
    block->size = 0;
    block->capacity = 0;
    if (initial_capacity > 0) {
        block->data = malloc(initial_capacity);
        if (block->data) {
            block->capacity = initial_capacity;
        }
    }
}

void free_compression_block(struct CompressionBlock *block) {
    if (block->data) {
        free(block->data);
        block->data = NULL;
    }
    block->size = 0;
    block->capacity = 0;
}

int resize_compression_block(struct CompressionBlock *block, size_t new_capacity) {
    if (new_capacity == 0) {
        free_compression_block(block);
        return 1;
    }
    uint8_t *new_data = realloc(block->data, new_capacity);
    if (!new_data) {
        return 0;
    }
    block->data = new_data;
    block->capacity = new_capacity;
    if (block->size > new_capacity) {
        block->size = new_capacity;
    }
    return 1;
}

int append_to_compression_block(struct CompressionBlock *block, const uint8_t *data, size_t data_size) {
    if (!data || data_size == 0) {
        return 1;
    }
    if (block->size + data_size > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        if (new_capacity < block->size + data_size) {
            new_capacity = block->size + data_size;
        }
        if (!resize_compression_block(block, new_capacity)) {
            return 0;
        }
    }
    memcpy(block->data + block->size, data, data_size);
    block->size += data_size;
    return 1;
}

void init_compression_context(struct CompressionContext *ctx) {
    memset(&ctx->header, 0, sizeof(ctx->header));
    init_compression_block(&ctx->input, 1024);
    init_compression_block(&ctx->output, 1024);
    ctx->status = 0;
}

void free_compression_context(struct CompressionContext *ctx) {
    free_compression_block(&ctx->input);
    free_compression_block(&ctx->output);
}

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx) {
    if (!ctx || !ctx->input.data || ctx->input.size == 0) {
        return 0;
    }
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = (uint32_t)ctx->input.size;
    if (ctx->input.size > UINT32_MAX) {
        return 0;
    }
    size_t output_needed = ctx->input.size + sizeof(struct CompressionHeader);
    if (!resize_compression_block(&ctx->output, output_needed)) {
        return 0;
    }
    memcpy(ctx->output.data, &ctx->header, sizeof(struct CompressionHeader));
    for (size_t i = 0; i < ctx->input.size; i++) {
        ctx->output.data[sizeof(struct CompressionHeader) + i] = ctx->input.data[i] ^ 0x55;
    }
    ctx->output.size = sizeof(struct CompressionHeader) + ctx->input.size;
    ctx->header.compressed_size = (uint32_t)ctx->output.size;
    ctx->header.algorithm = 1;
    ctx->header.checksum = calculate_checksum(ctx->output.data + sizeof(struct CompressionHeader), ctx->input.size);
    memcpy(ctx->output.data, &ctx->header, sizeof(struct CompressionHeader));
    return 1;
}

int decompress_data(struct CompressionContext *ctx) {
    if (!ctx || !ctx->output.data || ctx->output.size < sizeof(struct CompressionHeader)) {
        return 0;
    }
    memcpy(&ctx->header, ctx->output.data, sizeof(struct CompressionHeader));
    if (ctx->header.magic != 0x43535442) {
        return 0;
    }
    if (ctx->header.compressed_size != ctx->output.size) {
        return 0;
    }
    size_t data_size = ctx->output.size - sizeof(struct CompressionHeader);
    if (data_size != ctx->header.original_size) {
        return 0;
    }
    uint8_t checksum = calculate_checksum(ctx->output.data + sizeof(struct CompressionHeader), data_size);
    if (checksum != ctx->header.checksum) {
        return 0;
    }
    if (!resize_compression_block(&ctx->input, data_size)) {