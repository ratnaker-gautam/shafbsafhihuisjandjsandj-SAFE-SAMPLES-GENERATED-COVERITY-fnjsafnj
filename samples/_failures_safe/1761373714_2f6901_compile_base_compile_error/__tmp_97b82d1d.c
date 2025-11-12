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
    uint32_t block_size;
    uint8_t *data;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    uint32_t total_blocks;
};

struct CompressionResult {
    int success;
    size_t input_size;
    size_t output_size;
    struct CompressionContext *context;
};

void initialize_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
}

struct CompressionBlock *create_compression_block(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0 || size > UINT32_MAX) return NULL;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) return NULL;
    
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return NULL;
    }
    
    memcpy(block->data, data, size);
    block->block_size = (uint32_t)size;
    block->next = NULL;
    return block;
}

void free_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    
    memset(ctx, 0, sizeof(struct CompressionContext));
}

int add_block_to_context(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > UINT32_MAX) return 0;
    
    struct CompressionBlock *block = create_compression_block(data, size);
    if (block == NULL) return 0;
    
    if (ctx->first_block == NULL) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    ctx->header.original_size += block->block_size;
    ctx->header.compressed_size += block->block_size;
    
    return 1;
}

struct CompressionResult compress_data(const uint8_t *input, size_t input_size) {
    struct CompressionResult result = {0};
    
    if (input == NULL || input_size == 0 || input_size > SIZE_MAX / 2) {
        return result;
    }
    
    struct CompressionContext *ctx = malloc(sizeof(struct CompressionContext));
    if (ctx == NULL) return result;
    
    initialize_compression_context(ctx);
    
    size_t processed = 0;
    const size_t block_size = 1024;
    
    while (processed < input_size) {
        size_t remaining = input_size - processed;
        size_t current_block_size = (remaining < block_size) ? remaining : block_size;
        
        if (!add_block_to_context(ctx, input + processed, current_block_size)) {
            free_compression_context(ctx);
            free(ctx);
            return result;
        }
        
        processed += current_block_size;
    }
    
    ctx->header.checksum = 0;
    for (size_t i = 0; i < input_size && i < 256; i++) {
        ctx->header.checksum ^= input[i];
    }
    
    result.success = 1;
    result.input_size = input_size;
    result.output_size = ctx->header.compressed_size;
    result.context = ctx;
    
    return result;
}

void display_compression_info(const struct CompressionResult *result) {
    if (result == NULL || !result->success || result->context == NULL) {
        printf("Compression failed or invalid result.\n");
        return;
    }
    
    const struct CompressionContext *ctx = result->context;
    printf("Compression completed successfully.\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (float)ctx->header.compressed_size / ctx->header.original_size * 100.0f);
    printf("Number of blocks: %u\n", ctx->total_blocks);
    printf("Algorithm: %u\n", ctx->header.algorithm);
}

int main() {
    uint8_t test_data[2048];
    for (size_t i = 0; i < sizeof(test_data); i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    printf("Starting compression test...\n");
    printf("Input data size: %zu bytes\n", sizeof(test_data));
    
    struct CompressionResult result = compress_data(test_data, sizeof(test_data));
    
    if (result.success) {
        display_compression_info(&result);
        free_compression_context(result.context);
        free(result.context);
        printf("Compression test completed.\n");