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

int add_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (size == 0 || size > 65536) return 0;
    if (ctx->total_blocks >= 100) return 0;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (!block) return 0;
    
    block->data = malloc(size);
    if (!block->data) {
        free(block);
        return 0;
    }
    
    memcpy(block->data, data, size);
    block->block_size = size;
    block->next = NULL;
    
    if (ctx->last_block) {
        ctx->last_block->next = block;
    } else {
        ctx->first_block = block;
    }
    ctx->last_block = block;
    ctx->total_blocks++;
    
    return 1;
}

void free_blocks(struct CompressionContext *ctx) {
    struct CompressionBlock *current = ctx->first_block;
    while (current) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (!input || input_size == 0 || input_size > 1000000) return 0;
    
    initialize_context(ctx);
    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;
    
    uint32_t processed = 0;
    while (processed < input_size) {
        uint32_t block_size = (input_size - processed > 4096) ? 4096 : input_size - processed;
        
        uint8_t compressed_block[4096];
        uint32_t compressed_size = 0;
        
        for (uint32_t i = 0; i < block_size; i++) {
            if (compressed_size < sizeof(compressed_block)) {
                compressed_block[compressed_size++] = input[processed + i];
            }
        }
        
        if (!add_block(ctx, compressed_block, compressed_size)) {
            free_blocks(ctx);
            return 0;
        }
        
        processed += block_size;
        ctx->header.compressed_size += compressed_size;
    }
    
    ctx->header.checksum = 0;
    for (uint32_t i = 0; i < input_size && i < 256; i++) {
        ctx->header.checksum ^= input[i];
    }
    
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, uint32_t output_size) {
    if (!ctx || !output || output_size < ctx->header.original_size) return 0;
    if (!validate_header(&ctx->header)) return 0;
    
    uint32_t decompressed = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current && decompressed < ctx->header.original_size) {
        uint32_t copy_size = current->block_size;
        if (decompressed + copy_size > ctx->header.original_size) {
            copy_size = ctx->header.original_size - decompressed;
        }
        
        if (decompressed + copy_size > output_size) return 0;
        
        memcpy(output + decompressed, current->data, copy_size);
        decompressed += copy_size;
        current = current->next;
    }
    
    return decompressed == ctx->header.original_size;
}

int main() {
    struct CompressionContext ctx;
    uint8_t test_data[1000];
    
    for (int i = 0; i < 1000; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    printf("Original data size: 1000 bytes\n");
    
    if (!compress_data(&ctx, test_data, 1000)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed to %u bytes using %u blocks\n", 
           ctx.header.compressed_size, ctx.total_blocks);
    
    uint8_t decompressed[1000];
    if (!decompress_data(&ctx, decompressed, 1000)) {
        printf("Decompression failed\n");
        free_blocks(&