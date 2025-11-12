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
    uint16_t algorithm_id;
    uint8_t checksum;
};

struct CompressionBlock {
    uint8_t data[256];
    uint16_t size;
    uint32_t offset;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint8_t state;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm_id = 1;
    ctx->state = 1;
}

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535455) return 0;
    if (header->algorithm_id == 0 || header->algorithm_id > 5) return 0;
    if (header->original_size > 1000000) return 0;
    if (header->compressed_size > header->original_size) return 0;
    return 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    if (input_size > 256) return 0;
    if (ctx->state != 1) return 0;
    
    memcpy(ctx->current_block.data, input, input_size);
    ctx->current_block.size = (uint16_t)input_size;
    ctx->current_block.offset = ctx->total_processed;
    ctx->total_processed += input_size;
    
    if (ctx->total_processed > ctx->header.original_size) {
        return 0;
    }
    
    uint8_t checksum = 0;
    for (size_t i = 0; i < input_size; i++) {
        checksum ^= input[i];
    }
    ctx->header.checksum ^= checksum;
    
    return 1;
}

int finalize_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->state != 1) return 0;
    if (ctx->total_processed != ctx->header.original_size) return 0;
    
    ctx->header.compressed_size = ctx->total_processed;
    ctx->state = 2;
    return 1;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->state != 2) return;
    
    printf("Compression completed successfully\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Algorithm ID: %u\n", ctx->header.algorithm_id);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
    printf("Compression ratio: %.2f%%\n", 
           (float)ctx->header.compressed_size / ctx->header.original_size * 100.0f);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    printf("Enter original data size (1-1000): ");
    unsigned int size_input;
    if (scanf("%u", &size_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (size_input < 1 || size_input > 1000) {
        printf("Size out of range\n");
        return 1;
    }
    
    ctx.header.original_size = (uint32_t)size_input;
    
    printf("Processing %u bytes of data...\n", ctx.header.original_size);
    
    uint32_t remaining = ctx.header.original_size;
    uint8_t test_data[256];
    
    for (int i = 0; i < 256; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    while (remaining > 0) {
        size_t block_size = (remaining > 256) ? 256 : remaining;
        
        if (!process_block(&ctx, test_data, block_size)) {
            printf("Error processing block\n");
            return 1;
        }
        
        remaining -= block_size;
        printf("Processed block at offset %u, size %zu\n", 
               ctx.current_block.offset, block_size);
    }
    
    if (!finalize_compression(&ctx)) {
        printf("Error finalizing compression\n");
        return 1;
    }
    
    print_compression_info(&ctx);
    
    return 0;
}