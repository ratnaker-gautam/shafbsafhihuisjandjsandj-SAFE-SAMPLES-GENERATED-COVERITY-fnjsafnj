//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t algorithm;
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

struct CompressionResult {
    int success;
    size_t output_size;
    uint8_t *output_data;
};

void init_compression_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535442;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    memset(ctx, 0, sizeof(*ctx));
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) {
        return 0;
    }
    
    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) {
        return 0;
    }
    
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

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

struct CompressionResult compress_data(const uint8_t *input, size_t input_size) {
    struct CompressionResult result = {0};
    
    if (input == NULL || input_size == 0 || input_size > 1048576) {
        return result;
    }
    
    struct CompressionContext ctx;
    init_compression_context(&ctx);
    ctx.header.original_size = (uint32_t)input_size;
    
    const uint8_t *current_pos = input;
    size_t remaining = input_size;
    
    while (remaining > 0) {
        uint32_t block_size = (remaining > 4096) ? 4096 : (uint32_t)remaining;
        
        if (!add_compression_block(&ctx, current_pos, block_size)) {
            cleanup_compression_context(&ctx);
            return result;
        }
        
        current_pos += block_size;
        remaining -= block_size;
    }
    
    size_t output_size = sizeof(struct CompressionHeader) + ctx.header.compressed_size;
    uint8_t *output = malloc(output_size);
    if (output == NULL) {
        cleanup_compression_context(&ctx);
        return result;
    }
    
    memcpy(output, &ctx.header, sizeof(struct CompressionHeader));
    uint8_t *output_pos = output + sizeof(struct CompressionHeader);
    
    struct CompressionBlock *current = ctx.first_block;
    while (current != NULL) {
        memcpy(output_pos, current->data, current->block_size);
        output_pos += current->block_size;
        current = current->next;
    }
    
    struct CompressionHeader *output_header = (struct CompressionHeader*)output;
    output_header->checksum = calculate_checksum(input, input_size);
    
    result.success = 1;
    result.output_size = output_size;
    result.output_data = output;
    
    cleanup_compression_context(&ctx);
    return result;
}

struct CompressionResult decompress_data(const uint8_t *compressed, size_t compressed_size) {
    struct CompressionResult result = {0};
    
    if (compressed == NULL || compressed_size < sizeof(struct CompressionHeader)) {
        return result;
    }
    
    const struct CompressionHeader *header = (const struct CompressionHeader*)compressed;
    if (header->magic != 0x43535442) {
        return result;
    }
    
    if (header->original_size > 1048576 || header->compressed_size > 1048576) {
        return result;
    }
    
    size_t expected_size = sizeof(struct CompressionHeader) + header->compressed_size;
    if (compressed_size != expected_size) {
        return result;
    }
    
    uint8_t *output = malloc(header->original_size);
    if (output == NULL) {
        return result;
    }
    
    const uint8_t *compressed_data = compressed + sizeof(struct CompressionHeader);
    size_t data_remaining = header->compressed_size;
    uint8_t *output_pos = output;
    size_t output_remaining = header->original_size;
    
    while (data_remaining > 0 && output