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
    uint8_t flags;
    uint8_t reserved;
};

struct CompressionBlock {
    uint32_t offset;
    uint32_t size;
    uint8_t data[256];
    uint8_t checksum;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint8_t state;
};

uint8_t calculate_checksum(const uint8_t *data, size_t len) {
    uint8_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum ^= data[i];
    }
    return sum;
}

int validate_header(const struct CompressionHeader *header) {
    if (header->magic != 0x43535442) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > 104857600) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535442;
    ctx->header.algorithm = 1;
    ctx->state = 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_len) {
    if (ctx == NULL || input == NULL) return 0;
    if (input_len == 0 || input_len > 256) return 0;
    
    ctx->current_block.offset = ctx->total_processed;
    ctx->current_block.size = (uint32_t)input_len;
    
    if (input_len > sizeof(ctx->current_block.data)) return 0;
    memcpy(ctx->current_block.data, input, input_len);
    
    ctx->current_block.checksum = calculate_checksum(input, input_len);
    ctx->total_processed += (uint32_t)input_len;
    
    return 1;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_len) {
    if (ctx == NULL || input == NULL) return 0;
    if (input_len == 0) return 0;
    
    initialize_context(ctx);
    ctx->header.original_size = (uint32_t)input_len;
    
    size_t processed = 0;
    while (processed < input_len) {
        size_t block_size = input_len - processed;
        if (block_size > 256) block_size = 256;
        
        if (!process_block(ctx, input + processed, block_size)) {
            return 0;
        }
        
        processed += block_size;
    }
    
    ctx->header.compressed_size = ctx->total_processed;
    return 1;
}

void print_compression_info(const struct CompressionContext *ctx) {
    printf("Compression completed successfully\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100);
    printf("Algorithm: %u\n", ctx->header.algorithm);
    printf("Total blocks processed: %u\n", (ctx->total_processed + 255) / 256);
}

int main(void) {
    struct CompressionContext ctx;
    uint8_t test_data[512];
    
    for (size_t i = 0; i < sizeof(test_data); i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    printf("Starting compression test...\n");
    
    if (!compress_data(&ctx, test_data, sizeof(test_data))) {
        printf("Compression failed\n");
        return 1;
    }
    
    if (!validate_header(&ctx.header)) {
        printf("Invalid compression header\n");
        return 1;
    }
    
    print_compression_info(&ctx);
    
    printf("Verifying block checksums...\n");
    struct CompressionContext verify_ctx;
    initialize_context(&verify_ctx);
    
    size_t processed = 0;
    while (processed < sizeof(test_data)) {
        size_t block_size = sizeof(test_data) - processed;
        if (block_size > 256) block_size = 256;
        
        if (!process_block(&verify_ctx, test_data + processed, block_size)) {
            printf("Block verification failed\n");
            return 1;
        }
        
        uint8_t expected_checksum = calculate_checksum(test_data + processed, block_size);
        if (verify_ctx.current_block.checksum != expected_checksum) {
            printf("Checksum mismatch at block %zu\n", processed / 256);
            return 1;
        }
        
        processed += block_size;
    }
    
    printf("All checksums verified successfully\n");
    return 0;
}