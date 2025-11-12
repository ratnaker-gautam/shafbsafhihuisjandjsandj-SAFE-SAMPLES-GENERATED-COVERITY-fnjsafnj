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
    block->data = malloc(initial_capacity);
    if (block->data == NULL) {
        exit(EXIT_FAILURE);
    }
    block->size = 0;
    block->capacity = initial_capacity;
}

void free_compression_block(struct CompressionBlock *block) {
    free(block->data);
    block->data = NULL;
    block->size = 0;
    block->capacity = 0;
}

int resize_compression_block(struct CompressionBlock *block, size_t new_capacity) {
    if (new_capacity == 0) {
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
    if (data == NULL || data_size == 0) {
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
    if (!resize_compression_block(&ctx->output, ctx->input.size)) {
        return 0;
    }
    for (size_t i = 0; i < ctx->input.size; i++) {
        ctx->output.data[i] = ctx->input.data[i] ^ 0x55;
    }
    ctx->output.size = ctx->input.size;
    ctx->header.original_size = ctx->input.size;
    ctx->header.compressed_size = ctx->output.size;
    ctx->header.checksum = calculate_checksum(ctx->output.data, ctx->output.size);
    return 1;
}

int decompress_data(struct CompressionContext *ctx) {
    if (ctx->output.size == 0) {
        return 0;
    }
    uint8_t expected_checksum = calculate_checksum(ctx->output.data, ctx->output.size);
    if (expected_checksum != ctx->header.checksum) {
        return 0;
    }
    if (!resize_compression_block(&ctx->input, ctx->output.size)) {
        return 0;
    }
    for (size_t i = 0; i < ctx->output.size; i++) {
        ctx->input.data[i] = ctx->output.data[i] ^ 0x55;
    }
    ctx->input.size = ctx->output.size;
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);

    printf("Enter data to compress (max 1024 bytes): ");
    char input_buffer[1025];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        free_compression_context(&ctx);
        return EXIT_FAILURE;
    }

    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len == 0) {
        printf("No input provided\n");
        free_compression_context(&ctx);
        return