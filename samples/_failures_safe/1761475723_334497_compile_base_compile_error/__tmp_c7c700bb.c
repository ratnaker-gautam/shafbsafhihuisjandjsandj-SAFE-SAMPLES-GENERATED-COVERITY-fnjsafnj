//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t compressed_size;
    uint32_t original_size;
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
    if (!block) return;
    block->capacity = initial_capacity > 0 ? initial_capacity : 1;
    block->data = malloc(block->capacity);
    block->size = 0;
}

void free_compression_block(struct CompressionBlock *block) {
    if (block && block->data) {
        free(block->data);
        block->data = NULL;
    }
    if (block) {
        block->size = 0;
        block->capacity = 0;
    }
}

int resize_compression_block(struct CompressionBlock *block, size_t new_capacity) {
    if (!block || new_capacity == 0) return 0;
    uint8_t *new_data = realloc(block->data, new_capacity);
    if (!new_data) return 0;
    block->data = new_data;
    block->capacity = new_capacity;
    if (block->size > new_capacity) block->size = new_capacity;
    return 1;
}

int append_to_compression_block(struct CompressionBlock *block, const uint8_t *data, size_t data_size) {
    if (!block || !data || data_size == 0) return 0;
    if (block->size + data_size < block->size) return 0;
    if (block->size + data_size > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        if (new_capacity < block->capacity) return 0;
        while (new_capacity < block->size + data_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
            if (new_capacity < block->capacity) return 0;
        }
        if (!resize_compression_block(block, new_capacity)) return 0;
    }
    memcpy(block->data + block->size, data, data_size);
    block->size += data_size;
    return 1;
}

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

void init_compression_context(struct CompressionContext *ctx) {
    if (!ctx) return;
    ctx->header.magic = 0x43535442;
    ctx->header.compressed_size = 0;
    ctx->header.original_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    init_compression_block(&ctx->input, 1024);
    init_compression_block(&ctx->output, 1024);
    ctx->status = 0;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (!ctx) return;
    free_compression_block(&ctx->input);
    free_compression_block(&ctx->output);
}

int compress_data(struct CompressionContext *ctx) {
    if (!ctx || ctx->input.size == 0) return 0;
    if (ctx->input.size > UINT32_MAX) return 0;
    ctx->header.original_size = (uint32_t)ctx->input.size;
    if (!resize_compression_block(&ctx->output, ctx->input.size)) return 0;
    for (size_t i = 0; i < ctx->input.size; i++) {
        ctx->output.data[i] = ctx->input.data[i] ^ 0x55;
    }
    ctx->output.size = ctx->input.size;
    ctx->header.compressed_size = (uint32_t)ctx->output.size;
    ctx->header.checksum = calculate_checksum(ctx->output.data, ctx->output.size);
    return 1;
}

int read_input_data(struct CompressionContext *ctx) {
    if (!ctx) return 0;
    printf("Enter data to compress (max 1024 bytes): ");
    uint8_t buffer[1024];
    if (!fgets((char*)buffer, sizeof(buffer), stdin)) return 0;
    size_t len = strlen((char*)buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[--len] = 0;
    if (len == 0) return 0;
    return append_to_compression_block(&ctx->input, buffer, len);
}

void display_compression_result(struct CompressionContext *ctx) {
    if (!ctx) return;
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
        (double)ctx->header.compressed_size / ctx->header.original_size * 100);
    printf("Checksum: