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
    int compression_level;
};

void init_compression_block(struct CompressionBlock *block, size_t initial_capacity) {
    block->data = NULL;
    block->capacity = 0;
    block->size = 0;
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
    if (new_capacity == block->capacity) {
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
    if (!block || !data || data_size == 0) {
        return 1;
    }
    if (block->size + data_size < block->size) {
        return 0;
    }
    if (block->size + data_size > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        if (new_capacity < block->size + data_size) {
            new_capacity = block->size + data_size;
        }
        if (new_capacity < block->size) {
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
    if (!ctx) return;
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    init_compression_block(&ctx->input, 1024);
    init_compression_block(&ctx->output, 1024);
    ctx->compression_level = 6;
}

void free_compression_context(struct CompressionContext *ctx) {
    if (ctx) {
        free_compression_block(&ctx->input);
        free_compression_block(&ctx->output);
    }
}

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    if (data) {
        for (size_t i = 0; i < size; i++) {
            checksum ^= data[i];
        }
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx) {
    if (!ctx || !ctx->input.data || ctx->input.size == 0) {
        return 0;
    }
    ctx->output.size = 0;
    for (size_t i = 0; i < ctx->input.size; i++) {
        uint8_t byte = ctx->input.data[i];
        if (byte == 0) {
            uint8_t zero_seq[] = {0x00, 0x01};
            if (!append_to_compression_block(&ctx->output, zero_seq, 2)) {
                return 0;
            }
        } else {
            if (!append_to_compression_block(&ctx->output, &byte, 1)) {
                return 0;
            }
        }
    }
    ctx->header.original_size = (uint32_t)ctx->input.size;
    ctx->header.compressed_size = (uint32_t)ctx->output.size;
    ctx->header.checksum = calculate_checksum(ctx->output.data, ctx->output.size);
    return 1;
}

int read_input_data(struct CompressionContext *ctx) {
    if (!ctx) {
        return 0;
    }
    printf("Enter data to compress (max 4096 bytes): ");
    char buffer[4097];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
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
    if (!append_to_compression_block(&ctx->input