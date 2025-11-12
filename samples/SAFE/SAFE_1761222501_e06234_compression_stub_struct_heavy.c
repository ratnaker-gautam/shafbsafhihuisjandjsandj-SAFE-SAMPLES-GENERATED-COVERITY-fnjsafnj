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
    if (header->original_size > 10485760) return 0;
    if (header->compressed_size > 10485760) return 0;
    if (header->algorithm_id == 0 || header->algorithm_id > 10) return 0;
    return 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    if (input_size > 256) return 0;
    if (ctx->state != 1) return 0;
    
    memcpy(ctx->current_block.data, input, input_size);
    ctx->current_block.size = (uint16_t)input_size;
    ctx->current_block.offset = ctx->total_processed;
    ctx->total_processed += (uint32_t)input_size;
    
    uint8_t sum = 0;
    for (size_t i = 0; i < input_size; i++) {
        sum ^= input[i];
    }
    ctx->header.checksum ^= sum;
    
    return 1;
}

int finalize_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->state != 1) return 0;
    
    ctx->header.original_size = ctx->total_processed;
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
    printf("Total blocks processed: %u\n", (ctx->total_processed + 255) / 256);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data[3][256] = {0};
    size_t sizes[3] = {100, 200, 50};
    
    for (int i = 0; i < 3; i++) {
        for (size_t j = 0; j < sizes[i]; j++) {
            test_data[i][j] = (uint8_t)((i * 50 + j) % 256);
        }
    }
    
    for (int i = 0; i < 3; i++) {
        if (!process_block(&ctx, test_data[i], sizes[i])) {
            printf("Error processing block %d\n", i);
            return 1;
        }
    }
    
    if (!finalize_compression(&ctx)) {
        printf("Error finalizing compression\n");
        return 1;
    }
    
    if (!validate_header(&ctx.header)) {
        printf("Invalid compression header\n");
        return 1;
    }
    
    print_compression_info(&ctx);
    
    return 0;
}