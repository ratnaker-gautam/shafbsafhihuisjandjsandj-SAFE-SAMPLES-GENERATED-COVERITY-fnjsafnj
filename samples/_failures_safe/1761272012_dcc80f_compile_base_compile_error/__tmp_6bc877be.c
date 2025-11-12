//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
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
    if (block->size > SIZE_MAX - data_size) {
        return 0;
    }
    size_t required = block->size + data_size;
    if (required > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        if (new_capacity < required) {
            new_capacity = required;
        }
        if (new_capacity < block->capacity) {
            return 0;
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
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    init_compression_block(&ctx->input, 1024);
    init_compression_block(&ctx->output, 1024);
    ctx->status = 0;
}

void free_compression_context(struct CompressionContext *ctx) {
    free_compression_block(&ctx->input);
    free_compression_block(&ctx->output);
}

int compress_data(struct CompressionContext *ctx) {
    if (!ctx || !ctx->input.data || ctx->input.size == 0) {
        return 0;
    }
    if (!resize_compression_block(&ctx->output, ctx->input.size)) {
        return 0;
    }
    ctx->output.size = 0;
    for (size_t i = 0; i < ctx->input.size; i++) {
        if (i > 0 && ctx->input.data[i] == ctx->input.data[i-1]) {
            continue;
        }
        if (!append_to_compression_block(&ctx->output, &ctx->input.data[i], 1)) {
            return 0;
        }
    }
    ctx->header.original_size = ctx->input.size;
    ctx->header.compressed_size = ctx->output.size;
    uint8_t checksum = 0;
    for (size_t i = 0; i < ctx->output.size; i++) {
        checksum ^= ctx->output.data[i];
    }
    ctx->header.checksum = checksum;
    return 1;
}

int read_input_data(struct CompressionContext *ctx) {
    printf("Enter data to compress (max 4096 bytes): ");
    char buffer[4097];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    if (len == 0) {
        return 0;
    }
    if (!resize_compression_block(&ctx->input, len)) {
        return 0;
    }
    ctx->input.size = 0;
    if (!append_to_compression_block(&ctx->input, (uint8_t*)buffer, len)) {
        return 0;
    }
    return 1;
}

void display_results(struct CompressionContext *ctx) {
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    if (ctx->header.original_size > 0) {
        double