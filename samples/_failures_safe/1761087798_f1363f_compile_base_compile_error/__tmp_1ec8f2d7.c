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
    int compression_level;
};

void init_compression_block(struct CompressionBlock *block, size_t initial_capacity) {
    if (block == NULL || initial_capacity == 0) {
        exit(EXIT_FAILURE);
    }
    block->data = malloc(initial_capacity);
    if (block->data == NULL) {
        exit(EXIT_FAILURE);
    }
    block->size = 0;
    block->capacity = initial_capacity;
}

void free_compression_block(struct CompressionBlock *block) {
    if (block != NULL) {
        free(block->data);
        block->data = NULL;
        block->size = 0;
        block->capacity = 0;
    }
}

int resize_compression_block(struct CompressionBlock *block, size_t new_capacity) {
    if (block == NULL || new_capacity == 0) {
        return 0;
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
    if (block == NULL || data == NULL || data_size == 0) {
        return 0;
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

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->input.data == NULL || ctx->input.size == 0) {
        return 0;
    }
    ctx->header.original_size = (uint32_t)ctx->input.size;
    ctx->header.magic = 0x43535442;
    ctx->header.algorithm = 1;
    if (!append_to_compression_block(&ctx->output, (uint8_t*)&ctx->header, sizeof(struct CompressionHeader))) {
        return 0;
    }
    size_t compressed_offset = ctx->output.size;
    for (size_t i = 0; i < ctx->input.size; i++) {
        uint8_t byte = ctx->input.data[i];
        if (ctx->compression_level > 0 && i > 0 && byte == ctx->input.data[i-1]) {
            uint8_t count = 1;
            while (i < ctx->input.size && byte == ctx->input.data[i] && count < 255) {
                count++;
                i++;
            }
            i--;
            uint8_t run_length[2] = {byte, count};
            if (!append_to_compression_block(&ctx->output, run_length, 2)) {
                return 0;
            }
        } else {
            if (!append_to_compression_block(&ctx->output, &byte, 1)) {
                return 0;
            }
        }
    }
    ctx->header.compressed_size = (uint32_t)(ctx->output.size - compressed_offset);
    ctx->header.checksum = calculate_checksum(ctx->output.data + compressed_offset, ctx->header.compressed_size);
    memcpy(ctx->output.data, &ctx->header, sizeof(struct CompressionHeader));
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.compression_level = 1;
    
    init_compression_block(&ctx.input, 1024);
    init_compression_block(&ctx.output, 2048);
    
    printf("Enter text to compress (max 1023 characters): ");
    char input_buffer[1024];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        free_compression_block(&ctx.input);
        free_compression_block(&ctx.output);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len-1] == '\n') {
        input_buffer[input_len-1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided