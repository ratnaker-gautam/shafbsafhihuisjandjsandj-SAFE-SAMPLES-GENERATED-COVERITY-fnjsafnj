//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t compressed_size;
    uint32_t original_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t offset;
    uint32_t size;
    uint8_t *data;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    uint32_t total_blocks;
    uint32_t max_block_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->compressed_size == 0) return 0;
    if (header->original_size == 0) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

struct CompressionBlock *create_block(uint32_t offset, uint32_t size, const uint8_t *data) {
    if (size == 0 || data == NULL) return NULL;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) return NULL;
    
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return NULL;
    }
    
    memcpy(block->data, data, size);
    block->offset = offset;
    block->size = size;
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

int add_block(struct CompressionContext *ctx, uint32_t offset, uint32_t size, const uint8_t *data) {
    if (ctx == NULL || data == NULL || size == 0) return 0;
    if (ctx->total_blocks >= 1000) return 0;
    
    struct CompressionBlock *block = create_block(offset, size, data);
    if (block == NULL) return 0;
    
    if (ctx->first_block == NULL) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    return 1;
}

struct CompressionContext *create_context(uint32_t max_block_size) {
    if (max_block_size == 0 || max_block_size > 65536) return NULL;
    
    struct CompressionContext *ctx = malloc(sizeof(struct CompressionContext));
    if (ctx == NULL) return NULL;
    
    ctx->header.magic = 0x43535442;
    ctx->header.compressed_size = 0;
    ctx->header.original_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    ctx->header.flags = 0;
    
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
    ctx->max_block_size = max_block_size;
    
    return ctx;
}

void free_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    free_blocks(ctx->first_block);
    free(ctx);
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    
    uint32_t processed = 0;
    
    while (processed < input_size) {
        uint32_t remaining = input_size - processed;
        uint32_t block_size = (remaining > ctx->max_block_size) ? ctx->max_block_size : remaining;
        
        if (!add_block(ctx, processed, block_size, input + processed)) {
            return 0;
        }
        
        processed += block_size;
    }
    
    ctx->header.original_size = input_size;
    ctx->header.compressed_size = processed;
    
    return 1;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Header:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Compressed Size: %u\n", ctx->header.compressed_size);
    printf("  Original Size: %u\n", ctx->header.original_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Flags: 0x%02X\n", ctx->header.flags);
    printf("Total Blocks: %u\n", ctx->total_blocks);
    printf("Max Block Size: %u\n", ctx->max_block_size);
    
    struct CompressionBlock *block = ctx->first_block;
    uint32_t block_count = 0;
    while (block != NULL && block_count < 5) {
        printf("Block %u: offset=%u, size=%u\n", block_count, block->offset, block->size);
        block = block->next;
        block_count++;
    }
}

int main