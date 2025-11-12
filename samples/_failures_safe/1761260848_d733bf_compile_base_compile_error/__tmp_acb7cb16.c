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
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
    int compression_level;
};

struct CompressionResult {
    int success;
    size_t input_size;
    size_t output_size;
    double compression_ratio;
};

void init_compression_context(struct CompressionContext *ctx, int level) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    if (level < 0) level = 0;
    if (level > 9) level = 9;
    ctx->compression_level = level;
}

int add_block_data(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) return 0;
    if (size > 1024 * 1024) return 0;

    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) return 0;

    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return 0;
    }

    memcpy(block->data, data, size);
    block->size = size;
    block->capacity = size;
    block->next = NULL;

    if (ctx->last_block == NULL) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }

    ctx->total_blocks++;
    if (ctx->header.original_size + size < ctx->header.original_size) {
        free(block->data);
        free(block);
        return 0;
    }
    ctx->header.original_size += size;
    return 1;
}

struct CompressionResult perform_compression(struct CompressionContext *ctx) {
    struct CompressionResult result = {0};
    if (ctx == NULL || ctx->first_block == NULL) return result;

    size_t compressed_size = 0;
    struct CompressionBlock *current = ctx->first_block;

    while (current != NULL) {
        compressed_size += current->size;
        current = current->next;
    }

    if (compressed_size > UINT32_MAX / 85) {
        return result;
    }
    compressed_size = (compressed_size * 85) / 100;

    if (compressed_size == 0 && ctx->header.original_size > 0) {
        compressed_size = 1;
    }

    ctx->header.compressed_size = compressed_size;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 42;

    result.success = 1;
    result.input_size = ctx->header.original_size;
    result.output_size = compressed_size;
    
    if (result.input_size > 0) {
        result.compression_ratio = (double)result.output_size / result.input_size;
    } else {
        result.compression_ratio = 0.0;
    }

    return result;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;

    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        if (current->data != NULL) {
            free(current->data);
        }
        free(current);
        current = next;
    }

    memset(ctx, 0, sizeof(struct CompressionContext));
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx, 6);

    uint8_t test_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
    uint8_t test_data2[] = {0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74};
    uint8_t test_data3[] = {0x43, 0x6F, 0x6D, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6F, 0x6E, 0x20, 0x44, 0x65, 0x6D, 0x6F};

    if (!add_block_data(&ctx, test_data1, sizeof(test_data1))) {
        printf("Failed to add block 1\n");
        cleanup_compression_context(&ctx);
        return 1;
    }

    if (!add_block_data(&ctx, test_data2, sizeof(test_data2))) {
        printf("Failed to add block