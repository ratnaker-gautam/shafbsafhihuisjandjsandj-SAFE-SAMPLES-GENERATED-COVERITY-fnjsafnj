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
    uint32_t offset;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
    uint32_t max_block_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->original_size == 0) return 0;
    if (header->compressed_size > header->original_size) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

struct CompressionBlock *create_block(const uint8_t *data, size_t size, uint32_t offset) {
    if (data == NULL || size == 0) return NULL;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) return NULL;
    
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return NULL;
    }
    
    memcpy(block->data, data, size);
    block->size = size;
    block->offset = offset;
    block->next = NULL;
    return block;
}

void free_blocks(struct CompressionBlock *block) {
    while (block != NULL) {
        struct CompressionBlock *next = block->next;
        free(block->data);
        free(block);
        block = next;
    }
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) return 0;
    if (size > ctx->max_block_size) return 0;
    
    uint32_t offset = 0;
    if (ctx->last_block != NULL) {
        offset = ctx->last_block->offset + (uint32_t)ctx->last_block->size;
    }
    
    struct CompressionBlock *block = create_block(data, size, offset);
    if (block == NULL) return 0;
    
    if (ctx->first_block == NULL) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    ctx->header.compressed_size += (uint32_t)size;
    return 1;
}

void initialize_context(struct CompressionContext *ctx, uint32_t original_size, uint16_t algorithm) {
    if (ctx == NULL) return;
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = original_size;
    ctx->header.algorithm = algorithm;
    ctx->header.checksum = 0;
    ctx->max_block_size = 4096;
}

uint8_t calculate_checksum(const struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    
    uint8_t checksum = 0;
    const struct CompressionBlock *block = ctx->first_block;
    while (block != NULL) {
        for (size_t i = 0; i < block->size; i++) {
            checksum ^= block->data[i];
        }
        block = block->next;
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    if (input_size != ctx->header.original_size) return 0;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t block_size = input_size - processed;
        if (block_size > ctx->max_block_size) {
            block_size = ctx->max_block_size;
        }
        
        if (!add_block(ctx, input + processed, block_size)) {
            return 0;
        }
        
        processed += block_size;
    }
    
    ctx->header.checksum = calculate_checksum(ctx);
    return 1;
}

void display_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Information:\n");
    printf("  Original Size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Blocks: %zu\n", ctx->total_blocks);
    
    float ratio = 0.0f;
    if (ctx->header.original_size > 0) {
        ratio = (float)ctx->header.compressed_size / ctx->header.original_size;
    }
    printf("  Compression Ratio: %.2f\n", ratio);
}

int main(void) {
    struct CompressionContext ctx;
    
    uint8_t test_data[8192];
    for (size