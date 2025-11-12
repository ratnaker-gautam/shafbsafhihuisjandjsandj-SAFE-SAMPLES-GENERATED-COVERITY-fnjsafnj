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
    uint16_t flags;
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
    if (header->algorithm_id > 10) return 0;
    return 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    if (input_size > 256) return 0;
    if (ctx->state != 1) return 0;
    
    memcpy(ctx->current_block.data, input, input_size);
    ctx->current_block.size = (uint16_t)input_size;
    ctx->current_block.flags = 0;
    
    uint8_t sum = 0;
    for (size_t i = 0; i < input_size; i++) {
        sum ^= input[i];
    }
    ctx->current_block.flags = sum;
    
    ctx->total_processed += (uint32_t)input_size;
    return 1;
}

int finalize_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->state != 1) return 0;
    
    ctx->header.original_size = ctx->total_processed;
    ctx->header.compressed_size = ctx->total_processed;
    
    uint8_t checksum = 0;
    uint8_t *data = (uint8_t*)&ctx->header;
    for (size_t i = 0; i < sizeof(struct CompressionHeader) - 1; i++) {
        checksum ^= data[i];
    }
    ctx->header.checksum = checksum;
    
    ctx->state = 2;
    return 1;
}

void print_compression_result(const struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->state != 2) return;
    
    printf("Compression completed successfully\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Algorithm ID: %u\n", ctx->header.algorithm_id);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
    
    if (ctx->header.original_size > 0) {
        double ratio = (double)ctx->header.compressed_size / ctx->header.original_size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
    size_t data_size = sizeof(test_data);
    
    printf("Processing %zu bytes of test data\n", data_size);
    
    if (!process_block(&ctx, test_data, data_size)) {
        printf("Error processing data block\n");
        return 1;
    }
    
    if (!finalize_compression(&ctx)) {
        printf("Error finalizing compression\n");
        return 1;
    }
    
    if (!validate_header(&ctx.header)) {
        printf("Invalid compression header\n");
        return 1;
    }
    
    print_compression_result(&ctx);
    
    return 0;
}