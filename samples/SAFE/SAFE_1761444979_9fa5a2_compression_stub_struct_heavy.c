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
    if (initial_capacity == 0) initial_capacity = 64;
    block->data = malloc(initial_capacity);
    if (block->data == NULL) {
        exit(EXIT_FAILURE);
    }
    block->size = 0;
    block->capacity = initial_capacity;
}

void free_compression_block(struct CompressionBlock *block) {
    if (block->data != NULL) {
        free(block->data);
        block->data = NULL;
    }
    block->size = 0;
    block->capacity = 0;
}

int resize_compression_block(struct CompressionBlock *block, size_t new_capacity) {
    if (new_capacity <= block->capacity) return 1;
    uint8_t *new_data = realloc(block->data, new_capacity);
    if (new_data == NULL) return 0;
    block->data = new_data;
    block->capacity = new_capacity;
    return 1;
}

int append_to_compression_block(struct CompressionBlock *block, const uint8_t *data, size_t data_size) {
    if (data_size == 0) return 1;
    if (block->size > SIZE_MAX - data_size) return 0;
    size_t required = block->size + data_size;
    if (required > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        if (new_capacity < required) new_capacity = required;
        if (!resize_compression_block(block, new_capacity)) return 0;
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
    if (ctx->input.size == 0) return 0;
    if (ctx->input.size > UINT32_MAX) return 0;
    ctx->header.original_size = (uint32_t)ctx->input.size;
    size_t output_needed = ctx->input.size + sizeof(struct CompressionHeader);
    if (output_needed > ctx->output.capacity) {
        if (!resize_compression_block(&ctx->output, output_needed)) return 0;
    }
    ctx->header.compressed_size = (uint32_t)ctx->input.size;
    ctx->header.checksum = calculate_checksum(ctx->input.data, ctx->input.size);
    memcpy(ctx->output.data, &ctx->header, sizeof(struct CompressionHeader));
    memcpy(ctx->output.data + sizeof(struct CompressionHeader), ctx->input.data, ctx->input.size);
    ctx->output.size = sizeof(struct CompressionHeader) + ctx->input.size;
    return 1;
}

int read_input_data(struct CompressionContext *ctx) {
    uint8_t buffer[256];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), stdin)) > 0) {
        if (!append_to_compression_block(&ctx->input, buffer, bytes_read)) {
            return 0;
        }
    }
    if (ferror(stdin)) return 0;
    return 1;
}

int write_output_data(struct CompressionContext *ctx) {
    size_t written = fwrite(ctx->output.data, 1, ctx->output.size, stdout);
    if (written != ctx->output.size) return 0;
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);
    if (!read_input_data(&ctx)) {
        free_compression_context(&ctx);
        return EXIT_FAILURE;
    }
    if (!compress_data(&ctx)) {
        free_compression_context(&ctx);
        return EXIT_FAILURE;
    }
    if (!write_output_data(&ctx)) {
        free_compression_context(&ctx);
        return EXIT_FAILURE;
    }
    free_compression_context(&ctx);
    return EXIT_SUCCESS;
}