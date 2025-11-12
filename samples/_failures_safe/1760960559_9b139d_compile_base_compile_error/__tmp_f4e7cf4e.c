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
    if (required < block->size) {
        return 0;
    }
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
    ctx->header.magic = 0;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 0;
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
    ctx->output.size = 0;
    for (size_t i = 0; i < ctx->input.size; i++) {
        uint8_t current = ctx->input.data[i];
        if (i + 1 < ctx->input.size && current == ctx->input.data[i + 1]) {
            size_t count = 1;
            while (i + count < ctx->input.size && count < 255 && ctx->input.data[i + count] == current) {
                count++;
            }
            uint8_t run_length = count;
            uint8_t pattern[2] = {current, run_length};
            if (!append_to_compression_block(&ctx->output, pattern, 2)) {
                return 0;
            }
            i += count - 1;
        } else {
            uint8_t single[1] = {current};
            if (!append_to_compression_block(&ctx->output, single, 1)) {
                return 0;
            }
        }
    }
    ctx->header.original_size = ctx->input.size;
    ctx->header.compressed_size = ctx->output.size;
    ctx->header.algorithm = 1;
    ctx->header.checksum = calculate_checksum(ctx->output.data, ctx->output.size);
    ctx->header.magic = 0x43535442;
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);
    printf("Enter data to compress (max 1024 bytes): ");
    char input_buffer[1025];
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        printf("Input error\n");
        free_compression_context(&ctx);
        return 1;
    }
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len -