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
    if (size == 0 || size > 65536) return NULL;
    
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

void simulate_compression(struct CompressionContext *ctx) {
    uint32_t total_compressed = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current) {
        for (uint32_t i = 0; i < current->block_size; i++) {
            current->data[i] = (uint8_t)(i % 256);
        }
        total_compressed += current->block_size;
        current = current->next;
    }
    
    ctx->header.compressed_size = total_compressed;
}

void display_compression_info(const struct CompressionContext *ctx) {
    printf("Compression Results:\n");
    printf("Original Size: %u bytes\n", ctx->header.original_size);
    printf("Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression Ratio: %.2f%%\n", 
           (1.0 - (float)ctx->header.compressed_size / ctx->header.original_size) * 100.0);
    printf("Blocks Processed: %u\n", ctx->total_blocks);
    printf("Algorithm: %u\n", ctx->header.algorithm);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    printf("Enter original data size (bytes): ");
    if (scanf("%u", &ctx.header.original_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (ctx.header.original_size == 0 || ctx.header.original_size > 1000000) {
        printf("Invalid size\n");
        return 1;
    }
    
    printf("Enter compression algorithm (0-5): ");
    if (scanf("%hu", &ctx.header.algorithm) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (ctx.header.algorithm > 5) {
        printf("Invalid algorithm\n");
        return 1;
    }
    
    uint32_t remaining = ctx.header.original_size;
    while (remaining > 0) {
        uint32_t block_size = (remaining > 4096) ? 4096 : remaining;
        if (!add_block(&ctx, block_size)) {
            printf("Failed to create block\n");
            free_blocks(ctx.first_block);
            return 1;
        }
        remaining -= block_size;
    }
    
    simulate_compression(&ctx);
    display_compression_info(&ctx);
    
    free_blocks(ctx.first_block);
    return 0;
}