//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
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
};

struct CompressionBlock {
    uint8_t *data;
    size_t size;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 1024) return 0;
    
    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) return 0;
    
    new_block->data = malloc(size);
    if (new_block->data == NULL) {
        free(new_block);
        return 0;
    }
    
    memcpy(new_block->data, data, size);
    new_block->size = size;
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

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->first_block == NULL) return;
    
    uint8_t checksum = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current != NULL) {
        for (size_t i = 0; i < current->size; i++) {
            checksum ^= current->data[i];
        }
        current = current->next;
    }
    
    ctx->header.checksum = checksum;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    
    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t block_size = (input_size - processed > 16) ? 16 : input_size - processed;
        
        uint8_t compressed_block[16];
        for (size_t i = 0; i < block_size; i++) {
            compressed_block[i] = input[processed + i] ^ 0xAA;
        }
        
        if (!add_block(ctx, compressed_block, block_size)) {
            return 0;
        }
        
        processed += block_size;
    }
    
    calculate_checksum(ctx);
    return 1;
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

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Header:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Original Size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Total Blocks: %zu\n", ctx->total_blocks);
    
    struct CompressionBlock *current = ctx->first_block;
    size_t block_num = 0;
    
    while (current != NULL) {
        printf("  Block %zu: %zu bytes\n", block_num, current->size);
        current = current->next;
        block_num++;
    }
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data[] = {
        'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!',
        'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 't', 'e', 's', 't'
    };
    
    size_t data_size = sizeof(test_data);
    
    printf("Original data size: %zu bytes\n", data_size);
    
    if (compress_data(&ctx, test_data, data_size)) {
        printf("Compression successful!\n");
        print_compression_info(&ctx);
        
        float ratio = (float)ctx.header.compressed_size / ctx.header.original_size;
        printf("Compression ratio: %.2f\n", ratio);
    } else {
        printf("Compression failed!\n");
    }