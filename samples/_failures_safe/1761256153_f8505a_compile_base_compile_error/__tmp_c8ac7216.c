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

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
    ctx->max_block_size = 4096;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    if (input_size > UINT32_MAX) return 0;
    
    ctx->header.original_size = (uint32_t)input_size;
    
    size_t remaining = input_size;
    const uint8_t *current = input;
    
    while (remaining > 0) {
        size_t block_size = (remaining < ctx->max_block_size) ? remaining : ctx->max_block_size;
        
        if (!add_block(ctx, current, block_size)) {
            return 0;
        }
        
        current += block_size;
        remaining -= block_size;
    }
    
    uint8_t checksum = 0;
    for (size_t i = 0; i < input_size; i++) {
        checksum ^= input[i];
    }
    ctx->header.checksum = checksum;
    
    return 1;
}

void display_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Results:\n");
    printf("  Original size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed size: %u bytes\n", ctx->header.compressed_size);
    if (ctx->header.original_size > 0) {
        double ratio = (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100;
        printf("  Compression ratio: %.2f%%\n", ratio);
    } else {
        printf("  Compression ratio: 0.00%%\n");
    }
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Total blocks: %zu\n", ctx->total_blocks);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data[1024];
    for (int i = 0; i <