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
    if (initial_capacity == 0) {
        initial_capacity = 1;
    }
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
    }
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
    if (data_size == 0 || data == NULL) {
        return 0;
    }
    if (block->size + data_size > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        if (new_capacity < block->size + data_size) {
            new_capacity = block->size + data_size;
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

void init_compression_context(struct CompressionContext *ctx, int level) {
    if (level < 0 || level > 9) {
        level = 6;
    }
    ctx->compression_level = level;
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    init_compression_block(&ctx->input, 1024);
    init_compression_block(&ctx->output, 1024);
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
    size_t output_needed = ctx->input.size + sizeof(struct CompressionHeader);
    if (!resize_compression_block(&ctx->output, output_needed)) {
        return 0;
    }
    for (size_t i = 0; i < ctx->input.size; i++) {
        ctx->output.data[i] = ctx->input.data[i] ^ 0x55;
    }
    ctx->output.size = ctx->input.size;
    ctx->header.compressed_size = ctx->output.size;
    ctx->header.checksum = calculate_checksum(ctx->output.data, ctx->output.size);
    return 1;
}

int write_compressed_file(struct CompressionContext *ctx, const char *filename) {
    if (filename == NULL || ctx->output.size == 0) {
        return 0;
    }
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    if (fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    if (fwrite(ctx->output.data, 1, ctx->output.size, file) != ctx->output.size) {
        fclose(file);
        return 0;
    }
    if (fclose(file) != 0) {
        return 0;
    }
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx, 6);
    
    printf("Enter data to compress (max 1024 bytes): ");
    char input_buffer[1025];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        free