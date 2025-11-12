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
    uint32_t total_size = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current) {
        for (uint32_t i = 0; i < current->block_size; i++) {
            current->data[i] = (uint8_t)(i % 256);
        }
        total_size += current->block_size;
        current = current->next;
    }
    
    ctx->header.original_size = total_size;
    ctx->header.compressed_size = total_size / 2;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0xAB;
}

void display_compression_info(const struct CompressionContext *ctx) {
    printf("Compression Results:\n");
    printf("Magic: 0x%08X\n", ctx->header.magic);
    printf("Original Size: %u bytes\n", ctx->header.original_size);
    printf("Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression Ratio: %.2f%%\n", 
           (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100);
    printf("Algorithm: %u\n", ctx->header.algorithm);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
    printf("Total Blocks: %u\n", ctx->total_blocks);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    printf("Enter number of compression blocks to create (1-10): ");
    int num_blocks;
    if (scanf("%d", &num_blocks) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (num_blocks < 1 || num_blocks > 10) {
        printf("Number of blocks must be between 1 and 10\n");
        return 1;
    }
    
    for (int i = 0; i < num_blocks; i++) {
        printf("Enter size for block %d (1-65536): ", i + 1);
        uint32_t block_size;
        if (scanf("%u", &block_size) != 1) {
            printf("Invalid block size\n");
            free_blocks(ctx.first_block);
            return 1;
        }
        
        if (!add_block(&ctx, block_size)) {
            printf("Failed to create block %d\n", i + 1);
            free_blocks(ctx.first_block);
            return 1;
        }
    }
    
    simulate_compression(&ctx);
    
    if (!validate_header(&ctx.header)) {
        printf("Invalid compression header generated\n");
        free_blocks(ctx.first_block);
        return 1;
    }
    
    display_compression_info(&ctx);
    
    free_blocks(ctx.first_block);
    return 0;
}