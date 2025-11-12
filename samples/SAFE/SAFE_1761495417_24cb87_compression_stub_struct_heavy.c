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

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

int validate_header(const struct CompressionHeader *header) {
    if (header->magic != 0x43535455) return 0;
    if (header->original_size > 1000000) return 0;
    if (header->compressed_size > 1000000) return 0;
    if (header->algorithm > 5) return 0;
    return 1;
}

struct CompressionBlock *create_block(uint32_t size) {
    if (size == 0 || size > 8192) return NULL;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (!block) return NULL;
    
    block->data = malloc(size);
    if (!block->data) {
        free(block);
        return NULL;
    }
    
    block->block_size = size;
    block->next = NULL;
    return block;
}

void free_blocks(struct CompressionBlock *block) {
    while (block) {
        struct CompressionBlock *next = block->next;
        free(block->data);
        free(block);
        block = next;
    }
}

int add_block(struct CompressionContext *ctx, uint32_t size) {
    struct CompressionBlock *block = create_block(size);
    if (!block) return 0;
    
    if (!ctx->first_block) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    return 1;
}

void generate_sample_data(struct CompressionContext *ctx) {
    for (uint32_t i = 0; i < 3; i++) {
        uint32_t size = 64 + (i * 32);
        if (!add_block(ctx, size)) {
            return;
        }
        
        for (uint32_t j = 0; j < size; j++) {
            ctx->last_block->data[j] = (uint8_t)((i + j) % 256);
        }
    }
    
    ctx->header.original_size = 256;
    ctx->header.compressed_size = 192;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0xAB;
}

void display_compression_info(const struct CompressionContext *ctx) {
    printf("Compression Header:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Original Size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("Total Blocks: %u\n", ctx->total_blocks);
    
    uint32_t total_data = 0;
    struct CompressionBlock *current = ctx->first_block;
    uint32_t block_num = 0;
    
    while (current) {
        printf("Block %u: %u bytes\n", block_num, current->block_size);
        total_data += current->block_size;
        current = current->next;
        block_num++;
    }
    
    printf("Total Data Size: %u bytes\n", total_data);
    printf("Compression Ratio: %.2f%%\n", 
           (1.0 - (float)ctx->header.compressed_size / ctx->header.original_size) * 100.0);
}

int main(void) {
    struct CompressionContext ctx;
    
    initialize_context(&ctx);
    
    if (!validate_header(&ctx.header)) {
        printf("Invalid compression header\n");
        return 1;
    }
    
    generate_sample_data(&ctx);
    
    if (ctx.total_blocks == 0) {
        printf("No compression blocks generated\n");
        return 1;
    }
    
    display_compression_info(&ctx);
    
    free_blocks(ctx.first_block);
    
    return 0;
}