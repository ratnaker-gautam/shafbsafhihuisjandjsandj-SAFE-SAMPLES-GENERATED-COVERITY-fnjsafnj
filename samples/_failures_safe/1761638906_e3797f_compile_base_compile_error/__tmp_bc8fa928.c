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
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_offset;
    uint32_t block_size;
    uint8_t block_type;
    uint8_t reserved[3];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
    uint8_t *input_data;
    uint32_t input_size;
    uint8_t *output_data;
    uint32_t output_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535455) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > 104857600) return 0;
    if (header->algorithm_id > 15) return 0;
    return 1;
}

int initialize_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL) return 0;
    if (max_blocks == 0 || max_blocks > 10000) return 0;
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return 0;
    
    ctx->max_blocks = max_blocks;
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm_id = 1;
    
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, uint32_t offset, uint32_t size, uint8_t type) {
    if (ctx == NULL) return 0;
    if (ctx->blocks == NULL) return 0;
    if (ctx->block_count >= ctx->max_blocks) return 0;
    if (size == 0 || size > 65536) return 0;
    if (type > 3) return 0;
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_offset = offset;
    block->block_size = size;
    block->block_type = type;
    memset(block->reserved, 0, sizeof(block->reserved));
    
    ctx->block_count++;
    return 1;
}

uint8_t calculate_checksum(const uint8_t *data, uint32_t size) {
    if (data == NULL || size == 0) return 0;
    
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (ctx == NULL || input == NULL) return 0;
    if (input_size == 0 || input_size > 1048576) return 0;
    
    ctx->input_data = malloc(input_size);
    if (ctx->input_data == NULL) return 0;
    memcpy(ctx->input_data, input, input_size);
    ctx->input_size = input_size;
    
    ctx->output_size = input_size / 2 + 64;
    if (ctx->output_size < input_size) {
        ctx->output_size = input_size;
    }
    
    ctx->output_data = malloc(ctx->output_size);
    if (ctx->output_data == NULL) {
        free(ctx->input_data);
        ctx->input_data = NULL;
        return 0;
    }
    
    for (uint32_t i = 0; i < input_size; i++) {
        ctx->output_data[i] = input[i] ^ 0xAA;
    }
    
    ctx->header.original_size = input_size;
    ctx->header.compressed_size = input_size;
    ctx->header.checksum = calculate_checksum(input, input_size);
    
    uint32_t block_size = 512;
    uint32_t blocks_needed = (input_size + block_size - 1) / block_size;
    
    for (uint32_t i = 0; i < blocks_needed && i < ctx->max_blocks; i++) {
        uint32_t offset = i * block_size;
        uint32_t size = (offset + block_size <= input_size) ? block_size : input_size - offset;
        add_compression_block(ctx, offset, size, 1);
    }
    
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
    
    if (ctx->input_data != NULL) {
        free(ctx->input_data);
        ctx->input_data = NULL;
    }
    
    if (ctx->output_data != NULL) {
        free(ctx->output_data);
        ctx->output_data = NULL;
    }
    
    ctx->block_count = 0;
    ctx->max_blocks = 0;
    ctx->input_size = 0;
    ctx->output_size = 0;
}

int main(void) {
    struct CompressionContext ctx;