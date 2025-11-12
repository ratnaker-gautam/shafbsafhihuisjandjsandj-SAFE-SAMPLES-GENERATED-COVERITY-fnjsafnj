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

struct CompressionConfig {
    uint16_t max_block_size;
    uint8_t compression_level;
    int enable_checksum;
};

void initialize_compression_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        if (current->data != NULL) {
            free(current->data);
        }
        free(current);
        current = next;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
}

int validate_compression_config(const struct CompressionConfig *config) {
    if (config == NULL) return 0;
    if (config->max_block_size == 0 || config->max_block_size > 65535) return 0;
    if (config->compression_level > 9) return 0;
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0) return 0;
    
    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) return 0;
    
    new_block->data = malloc(size);
    if (new_block->data == NULL) {
        free(new_block);
        return 0;
    }
    
    memcpy(new_block->data, data, size);
    new_block->block_size = size;
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

uint8_t calculate_checksum(const struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    
    uint8_t checksum = 0;
    const struct CompressionBlock *current = ctx->first_block;
    
    while (current != NULL) {
        for (uint32_t i = 0; i < current->block_size; i++) {
            checksum ^= current->data[i];
        }
        current = current->next;
    }
    
    return checksum;
}

int compress_data(struct CompressionContext *ctx, const struct CompressionConfig *config, const uint8_t *input, uint32_t input_size) {
    if (ctx == NULL || config == NULL || input == NULL || input_size == 0) return 0;
    if (!validate_compression_config(config)) return 0;
    
    initialize_compression_context(ctx);
    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;
    
    uint32_t remaining = input_size;
    const uint8_t *current_pos = input;
    
    while (remaining > 0) {
        uint32_t block_size = (remaining > config->max_block_size) ? config->max_block_size : remaining;
        
        if (!add_compression_block(ctx, current_pos, block_size)) {
            cleanup_compression_context(ctx);
            return 0;
        }
        
        current_pos += block_size;
        remaining -= block_size;
    }
    
    if (config->enable_checksum) {
        ctx->header.checksum = calculate_checksum(ctx);
    }
    
    return 1;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Results:\n");
    printf("Original Size: %u bytes\n", ctx->header.original_size);
    printf("Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression Ratio: %.2f%%\n", 
           (ctx->header.original_size > 0) ? 
           (100.0 * ctx->header.compressed_size / ctx->header.original_size) : 0.0);
    printf("Total Blocks: %u\n", ctx->total_blocks);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
}

int main() {
    struct CompressionContext ctx;
    struct CompressionConfig config;
    
    config.max_block_size = 1024;
    config.compression_level = 6;
    config.enable_checksum = 1;
    
    uint8_t test_data[2048];
    for (int i = 0; i < 2048; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    printf("Starting compression test...\n");
    
    if (compress_data(&ctx, &config, test_data, sizeof(test_data))) {
        print_