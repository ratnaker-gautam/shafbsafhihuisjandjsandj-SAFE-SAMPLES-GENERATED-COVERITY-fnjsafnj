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
    ctx->header.original_size = ctx->input.size;
    ctx->output.size = 0;
    for (size_t i = 0; i < ctx->input.size; i++) {
        uint8_t byte = ctx->input.data[i];
        if (byte == 0) {
            uint8_t zero_seq[] = {0, 1};
            if (!append_to_compression_block(&ctx->output, zero_seq, 2)) {
                return 0;
            }
        } else {
            if (!append_to_compression_block(&ctx->output, &byte, 1)) {
                return 0;
            }
        }
    }
    ctx->header.compressed_size = ctx->output.size;
    ctx->header.checksum = calculate_checksum(ctx->output.data, ctx->output.size);
    return 1;
}

int decompress_data(struct CompressionContext *ctx) {
    if (!ctx || !ctx->output.data || ctx->output.size == 0) {
        return 0;
    }
    ctx->input.size = 0;
    for (size_t i = 0; i < ctx->output.size; i++) {
        uint8_t byte = ctx->output.data[i];
        if (byte == 0 && i + 1 < ctx->output.size && ctx->output.data[i + 1] == 1) {
            uint8_t zero_byte[] = {0};
            if (!append_to_compression_block(&ctx->input, zero_byte, 1)) {
                return 0;
            }
            i++;
        } else {
            if (!append_to_compression_block(&ctx->input, &byte, 1)) {
                return 0;
            }
        }
    }
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);
    
    const char *test_string = "Hello, world! This is a test string with null\0byte.";
    size_t