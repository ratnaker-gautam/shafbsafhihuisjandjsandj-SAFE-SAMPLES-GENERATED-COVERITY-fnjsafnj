//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t algorithm;
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

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 1024) return 0;
    
    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) return 0;
    
    new_block->data = malloc(size);
    if (new_block->data == NULL) {
        free(new_block);
        return 0;
    }
    
    memcpy(new_block->data, data, size);
    new_block->block_size = size;
    new_block->next = NULL;
    
    if (ctx->first_block == NULL) {
        ctx->first_block = new_block;
        ctx->last_block = new_block;
    } else {
        ctx->last_block->next = new_block;
        ctx->last_block = new_block;
    }
    
    ctx->total_blocks++;
    ctx->header.compressed_size += size;
    return 1;
}

void free_blocks(struct CompressionBlock *block) {
    while (block != NULL) {
        struct CompressionBlock *next = block->next;
        free(block->data);
        free(block);
        block = next;
    }
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    free_blocks(ctx->first_block);
    ctx->first_block = NULL;
    ctx->last_block = NULL;
}

uint8_t calculate_checksum(const struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    
    uint8_t checksum = 0;
    const struct CompressionBlock *block = ctx->first_block;
    
    while (block != NULL) {
        for (uint32_t i = 0; i < block->block_size; i++) {
            checksum ^= block->data[i];
        }
        block = block->next;
    }
    
    return checksum;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    
    initialize_context(ctx);
    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;
    
    uint32_t processed = 0;
    while (processed < input_size) {
        uint32_t block_size = (input_size - processed > 16) ? 16 : (input_size - processed);
        
        if (!add_block(ctx, input + processed, block_size)) {
            cleanup_context(ctx);
            return 0;
        }
        
        processed += block_size;
    }
    
    ctx->header.checksum = calculate_checksum(ctx);
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, uint32_t output_size) {
    if (ctx == NULL || output == NULL || output_size < ctx->header.original_size) return 0;
    
    uint32_t written = 0;
    const struct CompressionBlock *block = ctx->first_block;
    
    while (block != NULL) {
        if (written + block->block_size > output_size) return 0;
        
        memcpy(output + written, block->data, block->block_size);
        written += block->block_size;
        block = block->next;
    }
    
    return (written == ctx->header.original_size);
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Information:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Original size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Total blocks: %u\n", ctx->total_blocks);
}

int main() {
    struct CompressionContext ctx;
    uint8_t test_data[] = "This is a test string for compression demonstration.";
    uint32_t data_size = sizeof(test_data) - 1;
    
    printf("Original data: %s\n", test_data);
    printf("Original size: %u bytes\n\n", data_size);
    
    if (!compress_data(&ctx, test_data, data_size)) {
        printf("Compression failed!\n");
        return 1;
    }
    
    print_compression_info(&ctx);
    
    uint8_t decompressed[100];
    if (!decompress_data(&ctx, decompressed, sizeof(decompressed))) {
        printf("