//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
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
    uint16_t encoded_length;
    uint8_t data[256];
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
    ctx->header.magic = 0x43535442;
    ctx->header.algorithm = 1;
    ctx->state = 1;
}

int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->length > 256) return 0;
    if (block->encoded_length > 256) return 0;
    if (block->offset > UINT32_MAX - 256) return 0;
    return 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_len) {
    if (ctx == NULL || input == NULL || input_len == 0) return 0;
    if (input_len > 256) input_len = 256;
    
    if (!validate_block(&ctx->current_block)) return 0;
    
    ctx->current_block.offset = ctx->total_processed;
    ctx->current_block.length = (uint16_t)input_len;
    
    for (size_t i = 0; i < input_len; i++) {
        ctx->current_block.data[i] = input[i] ^ 0x55;
    }
    
    ctx->current_block.encoded_length = (uint16_t)input_len;
    ctx->total_processed += (uint32_t)input_len;
    
    return 1;
}

int finalize_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->state == 0) return 0;
    
    ctx->header.original_size = ctx->total_processed;
    ctx->header.compressed_size = ctx->total_processed;
    
    uint8_t sum = 0;
    for (uint32_t i = 0; i < ctx->total_processed; i++) {
        if (i < 256) {
            sum ^= ctx->current_block.data[i];
        }
    }
    ctx->header.checksum = sum;
    
    ctx->state = 2;
    return 1;
}

void display_results(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression completed:\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (ctx->header.original_size == 0) ? 0.0 :
           (100.0 * (double)ctx->header.compressed_size / (double)ctx->header.original_size));
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data[] = {
        'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!',
        'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e',
        's', 't', ' ', 'd', 'a', 't', 'a', ' ', 's', 't', 'r', 'i',
        'n', 'g', ' ', 'f', 'o', 'r', ' ', 'c', 'o', 'm', 'p', 'r',
        'e', 's', 's', 'i', 'o', 'n', ' ', 'd', 'e', 'm', 'o', '.'
    };
    
    size_t data_len = sizeof(test_data);
    size_t processed = 0;
    
    while (processed < data_len) {
        size_t chunk_size = (data_len - processed > 32) ? 32 : data_len - processed;
        
        if (!process_block(&ctx, &test_data[processed], chunk_size)) {
            printf("Error processing block at offset %zu\n", processed);
            return 1;
        }
        
        processed += chunk_size;
    }
    
    if (!finalize_compression(&ctx)) {
        printf("Error finalizing compression\n");
        return 1;
    }
    
    display_results(&ctx);
    
    return 0;
}