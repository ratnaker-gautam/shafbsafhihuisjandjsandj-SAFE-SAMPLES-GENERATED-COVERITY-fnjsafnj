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
    struct CompressionBlock input_block;
    struct CompressionBlock output_block;
    int compression_level;
};

void init_compression_block(struct CompressionBlock *block, size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = 64;
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
        block->data = NULL;
    }
    block->size = 0;
    block->capacity = 0;
}

int resize_compression_block(struct CompressionBlock *block, size_t new_capacity) {
    if (new_capacity <= block->capacity) {
        return 0;
    }
    uint8_t *new_data = realloc(block->data, new_capacity);
    if (new_data == NULL) {
        return -1;
    }
    block->data = new_data;
    block->capacity = new_capacity;
    return 0;
}

int append_to_compression_block(struct CompressionBlock *block, const uint8_t *data, size_t data_size) {
    if (data == NULL || data_size == 0) {
        return 0;
    }
    size_t new_size = block->size + data_size;
    if (new_size < block->size) {
        return -1;
    }
    if (new_size > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        if (new_capacity < new_size) {
            new_capacity = new_size;
        }
        if (resize_compression_block(block, new_capacity) != 0) {
            return -1;
        }
    }
    memcpy(block->data + block->size, data, data_size);
    block->size = new_size;
    return 0;
}

void init_compression_context(struct CompressionContext *ctx, int level) {
    if (level < 0 || level > 9) {
        level = 6;
    }
    ctx->compression_level = level;
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
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

int perform_compression(struct CompressionContext *ctx) {
    if (ctx->input_block.size == 0) {
        return -1;
    }
    ctx->header.original_size = ctx->input_block.size;
    ctx->header.checksum = calculate_checksum(ctx->input_block.data, ctx->input_block.size);
    for (size_t i = 0; i < ctx->input_block.size; i++) {
        uint8_t byte = ctx->input_block.data[i];
        if (append_to_compression_block(&ctx->output_block, &byte, 1) != 0) {
            return -1;
        }
        if (ctx->compression_level > 5 && i > 0 && byte == ctx->input_block.data[i-1]) {
            uint8_t repeat_count = 1;
            while (i + 1 < ctx->input_block.size && ctx->input_block.data[i+1] == byte && repeat_count < 255) {
                repeat_count++;
                i++;
            }
            if (append_to_compression_block(&ctx->output_block, &repeat_count, 1) != 0) {
                return -1;
            }
        }
    }
    ctx->header.compressed_size = ctx->output_block.size;
    return 0;
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx, 7);
    
    printf("Enter data to compress (max 1024 bytes): ");
    char input_buffer[1025];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        free_compression_context(&ctx);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len-1] == '\n') {
        input_buffer[input_len-1] = '\0';
        input_len--;
    }
    
    if (input_len ==