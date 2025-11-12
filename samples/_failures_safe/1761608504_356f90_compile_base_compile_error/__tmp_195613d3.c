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
    }
    block->data = NULL;
    block->size = 0;
    block->capacity = 0;
}

int resize_compression_block(struct CompressionBlock *block, size_t new_capacity) {
    if (new_capacity == 0) {
        free_compression_block(block);
        return 1;
    }
    uint8_t *new_data = realloc(block->data, new_capacity);
    if (new_data == NULL) {
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
    if (data == NULL || data_size == 0) {
        return 1;
    }
    size_t required = block->size + data_size;
    if (required > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        if (new_capacity < required) {
            new_capacity = required;
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
    ctx->header.magic = 0x43535455;
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

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx->input.size == 0) {
        return 0;
    }
    ctx->header.original_size = ctx->input.size;
    ctx->header.checksum = calculate_checksum(ctx->input.data, ctx->input.size);
    size_t output_needed = ctx->input.size + sizeof(struct CompressionHeader);
    if (!resize_compression_block(&ctx->output, output_needed)) {
        return 0;
    }
    memcpy(ctx->output.data, &ctx->header, sizeof(struct CompressionHeader));
    for (size_t i = 0; i < ctx->input.size; i++) {
        ctx->output.data[sizeof(struct CompressionHeader) + i] = ctx->input.data[i] ^ 0x55;
    }
    ctx->output.size = output_needed;
    ctx->header.compressed_size = ctx->output.size;
    return 1;
}

int read_input_data(struct CompressionContext *ctx) {
    printf("Enter data to compress (max 1024 bytes): ");
    char buffer[1025];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    if (len == 0) {
        return 0;
    }
    if (!append_to_compression_block(&ctx->input, (uint8_t*)buffer, len)) {
        return 0;
    }
    return 1;
}

void display_compression_result(struct CompressionContext *ctx) {
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (float)ctx->header.compressed_size / ctx->header.original_size) * 100);
    printf("Checksum: 0x%02