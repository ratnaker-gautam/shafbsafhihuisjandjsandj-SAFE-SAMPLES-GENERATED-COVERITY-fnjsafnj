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
    uint32_t block_size;
    uint32_t offset;
    uint8_t data[256];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint8_t state;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535455) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > header->original_size) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
    ctx->state = 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *data, size_t data_size) {
    if (ctx == NULL || data == NULL) return 0;
    if (data_size == 0 || data_size > 256) return 0;
    if (ctx->state != 1) return 0;
    
    if (ctx->total_processed + data_size > UINT32_MAX) return 0;
    
    memcpy(ctx->current_block.data, data, data_size);
    ctx->current_block.block_size = (uint32_t)data_size;
    ctx->current_block.offset = ctx->total_processed;
    ctx->total_processed += (uint32_t)data_size;
    
    return 1;
}

int finalize_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->state != 1) return 0;
    
    ctx->header.original_size = ctx->total_processed;
    ctx->header.compressed_size = ctx->total_processed - (ctx->total_processed / 10);
    
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < ctx->total_processed && i < 256; i++) {
        checksum ^= ctx->current_block.data[i];
    }
    ctx->header.checksum = checksum;
    
    ctx->state = 2;
    return 1;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->state != 2) return;
    
    printf("Compression completed successfully.\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100);
    printf("Algorithm: %u\n", ctx->header.algorithm);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data[] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 
                          0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50};
    size_t test_size = sizeof(test_data);
    
    printf("Processing %zu bytes of test data...\n", test_size);
    
    if (!process_block(&ctx, test_data, test_size)) {
        printf("Error: Failed to process data block.\n");
        return 1;
    }
    
    if (!finalize_compression(&ctx)) {
        printf("Error: Failed to finalize compression.\n");
        return 1;
    }
    
    print_compression_info(&ctx);
    
    printf("Testing header validation...\n");
    if (validate_header(&ctx.header)) {
        printf("Header validation passed.\n");
    } else {
        printf("Header validation failed.\n");
    }
    
    struct CompressionHeader invalid_header;
    memset(&invalid_header, 0, sizeof(invalid_header));
    if (!validate_header(&invalid_header)) {
        printf("Invalid header correctly rejected.\n");
    }
    
    return 0;
}