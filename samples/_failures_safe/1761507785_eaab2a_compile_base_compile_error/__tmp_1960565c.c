//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm_id;
    uint8_t checksum;
};

struct CompressionBlock {
    uint8_t *data;
    size_t size;
    size_t capacity;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock input_block;
    struct CompressionBlock output_block;
    int compression_level;
};

void init_compression_block(struct CompressionBlock *block, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 64;
    block->data = malloc(initial_capacity);
    if (block->data == NULL) exit(EXIT_FAILURE);
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
    if (data == NULL || data_size == 0) return 1;
    if (block->size + data_size > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        while (new_capacity < block->size + data_size) {
            if (new_capacity > SIZE_MAX / 2) return 0;
            new_capacity *= 2;
        }
        if (!resize_compression_block(block, new_capacity)) return 0;
    }
    memcpy(block->data + block->size, data, data_size);
    block->size += data_size;
    return 1;
}

void init_compression_context(struct CompressionContext *ctx, int level) {
    if (level < 0) level = 0;
    if (level > 9) level = 9;
    ctx->compression_level = level;
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm_id = 1;
    ctx->header.checksum = 0;
    init_compression_block(&ctx->input_block, 1024);
    init_compression_block(&ctx->output_block, 1024);
}

void free_compression_context(struct CompressionContext *ctx) {
    free_compression_block(&ctx->input_block);
    free_compression_block(&ctx->output_block);
}

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx->input_block.size == 0) return 0;
    ctx->header.original_size = ctx->input_block.size;
    size_t output_size = ctx->input_block.size;
    if (ctx->compression_level > 0) {
        output_size = ctx->input_block.size * 85 / 100;
        if (output_size < 16) output_size = 16;
    }
    if (!resize_compression_block(&ctx->output_block, output_size)) return 0;
    for (size_t i = 0; i < output_size; i++) {
        ctx->output_block.data[i] = (uint8_t)(i % 256);
    }
    ctx->output_block.size = output_size;
    ctx->header.compressed_size = output_size;
    ctx->header.checksum = calculate_checksum(ctx->output_block.data, output_size);
    return 1;
}

int write_compressed_file(struct CompressionContext *ctx, const char *filename) {
    if (filename == NULL || strlen(filename) == 0) return 0;
    FILE *file = fopen(filename, "wb");
    if (file == NULL) return 0;
    if (fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    if (fwrite(ctx->output_block.data, 1, ctx->output_block.size, file) != ctx->output_block.size) {
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

int read_input_data(struct CompressionContext *ctx, const char *input) {
    if (input == NULL) return 0;
    size_t input_len = strlen(input);
    if (input_len == 0) return 0;
    if (!append_to_compression_block(&ctx->input_block, (const uint8_t*)input, input_len)) return 0;
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    init_