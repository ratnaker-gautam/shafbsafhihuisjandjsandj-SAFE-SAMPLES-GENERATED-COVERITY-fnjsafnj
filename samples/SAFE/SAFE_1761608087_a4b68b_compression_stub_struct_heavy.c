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
    size_t original_size;
    size_t compressed_size;
    double ratio;
};

void init_compression_context(struct CompressionContext *ctx, int level) {
    if (level < 0 || level > 9) level = 6;
    memset(ctx, 0, sizeof(*ctx));
    ctx->compression_level = level;
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
}

int add_data_to_context(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (!block) return 0;
    
    block->data = malloc(size);
    if (!block->data) {
        free(block);
        return 0;
    }
    
    memcpy(block->data, data, size);
    block->size = size;
    block->capacity = size;
    block->next = NULL;
    
    if (!ctx->first_block) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    ctx->header.original_size += size;
    return 1;
}

struct CompressionResult perform_compression(struct CompressionContext *ctx) {
    struct CompressionResult result = {0};
    if (!ctx || !ctx->first_block) return result;
    
    size_t compressed = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current) {
        compressed += current->size;
        current = current->next;
    }
    
    compressed = compressed * 85 / 100;
    if (compressed == 0 && ctx->header.original_size > 0) compressed = 1;
    
    ctx->header.compressed_size = compressed;
    ctx->header.checksum = (ctx->header.original_size + ctx->header.compressed_size) % 256;
    
    result.success = 1;
    result.original_size = ctx->header.original_size;
    result.compressed_size = compressed;
    result.ratio = (double)compressed / ctx->header.original_size;
    
    return result;
}

void free_compression_context(struct CompressionContext *ctx) {
    if (!ctx) return;
    
    struct CompressionBlock *current = ctx->first_block;
    while (current) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    
    memset(ctx, 0, sizeof(*ctx));
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx, 6);
    
    printf("Enter data to compress (max 1000 characters): ");
    char input[1001];
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No data provided\n");
        return 1;
    }
    
    if (!add_data_to_context(&ctx, (uint8_t*)input, input_len)) {
        printf("Failed to add data\n");
        return 1;
    }
    
    struct CompressionResult result = perform_compression(&ctx);
    
    if (result.success) {
        printf("Compression completed successfully\n");
        printf("Original size: %zu bytes\n", result.original_size);
        printf("Compressed size: %zu bytes\n", result.compressed_size);
        printf("Compression ratio: %.2f\n", result.ratio);
        printf("Magic: 0x%08X\n", ctx.header.magic);
        printf("Algorithm: %u\n", ctx.header.algorithm);
        printf("Checksum: 0x%02X\n", ctx.header.checksum);
    } else {
        printf("Compression failed\n");
    }
    
    free_compression_context(&ctx);
    return 0;
}