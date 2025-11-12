//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t offset;
    uint16_t length;
    uint8_t data[256];
    uint8_t encoding_type;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint16_t block_count;
    uint16_t max_blocks;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->original_size > 1048576) return 0;
    if (header->compressed_size > 1048576) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int validate_block(const struct CompressionBlock *block, uint32_t max_offset) {
    if (block == NULL) return 0;
    if (block->offset > max_offset) return 0;
    if (block->length > 256) return 0;
    if (block->encoding_type > 7) return 0;
    return 1;
}

int init_compression_context(struct CompressionContext *ctx, uint16_t max_blocks) {
    if (ctx == NULL) return 0;
    if (max_blocks == 0 || max_blocks > 4096) return 0;
    
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 0;
    ctx->header.checksum = 0;
    ctx->header.flags = 0;
    
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return 0;
    
    ctx->block_count = 0;
    ctx->max_blocks = max_blocks;
    
    return 1;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx != NULL && ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
}

int add_compression_block(struct CompressionContext *ctx, uint32_t offset, uint16_t length, const uint8_t *data, uint8_t encoding_type) {
    if (ctx == NULL || ctx->blocks == NULL) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    if (data == NULL) return 0;
    if (length > 256) return 0;
    if (encoding_type > 7) return 0;
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->offset = offset;
    block->length = length;
    block->encoding_type = encoding_type;
    
    for (uint16_t i = 0; i < length; i++) {
        block->data[i] = data[i];
    }
    
    ctx->block_count++;
    ctx->header.compressed_size += length;
    
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    uint8_t checksum = 0;
    for (uint16_t i = 0; i < ctx->block_count; i++) {
        struct CompressionBlock *block = &ctx->blocks[i];
        checksum ^= block->encoding_type;
        for (uint16_t j = 0; j < block->length; j++) {
            checksum ^= block->data[j];
        }
    }
    ctx->header.checksum = checksum;
}

int compress_sample_data(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    
    uint8_t sample_data[] = {0x41, 0x41, 0x41, 0x41, 0x42, 0x42, 0x43, 0x44, 0x45, 0x46};
    uint32_t data_size = sizeof(sample_data);
    
    ctx->header.original_size = data_size;
    
    uint32_t processed = 0;
    while (processed < data_size) {
        uint16_t chunk_size = (data_size - processed) > 256 ? 256 : (uint16_t)(data_size - processed);
        
        if (!add_compression_block(ctx, processed, chunk_size, &sample_data[processed], 0)) {
            return 0;
        }
        
        processed += chunk_size;
    }
    
    calculate_checksum(ctx);
    return 1;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Header:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Original Size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Flags: