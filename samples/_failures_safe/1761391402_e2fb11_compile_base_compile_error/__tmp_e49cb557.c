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
    uint32_t offset;
    uint16_t length;
    uint8_t flags;
    uint8_t data[256];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint16_t block_count;
    uint16_t max_blocks;
};

struct FileData {
    uint8_t *data;
    size_t size;
    size_t capacity;
};

int init_compression_context(struct CompressionContext *ctx, uint16_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) {
        return 0;
    }
    
    ctx->blocks = malloc(sizeof(struct CompressionBlock) * max_blocks);
    if (ctx->blocks == NULL) {
        return 0;
    }
    
    ctx->max_blocks = max_blocks;
    ctx->block_count = 0;
    memset(&ctx->header, 0, sizeof(struct CompressionHeader));
    ctx->header.magic = 0x43535455;
    
    return 1;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx != NULL && ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
}

int init_file_data(struct FileData *fd, size_t initial_capacity) {
    if (fd == NULL || initial_capacity == 0 || initial_capacity > 1000000) {
        return 0;
    }
    
    fd->data = malloc(initial_capacity);
    if (fd->data == NULL) {
        return 0;
    }
    
    fd->capacity = initial_capacity;
    fd->size = 0;
    return 1;
}

void cleanup_file_data(struct FileData *fd) {
    if (fd != NULL && fd->data != NULL) {
        free(fd->data);
        fd->data = NULL;
    }
}

int add_compression_block(struct CompressionContext *ctx, uint32_t offset, uint16_t length, const uint8_t *data) {
    if (ctx == NULL || ctx->blocks == NULL || data == NULL) {
        return 0;
    }
    
    if (ctx->block_count >= ctx->max_blocks) {
        return 0;
    }
    
    if (length > 256) {
        return 0;
    }
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->offset = offset;
    block->length = length;
    block->flags = 0;
    
    memcpy(block->data, data, length);
    ctx->block_count++;
    
    return 1;
}

int compress_data(struct CompressionContext *ctx, const struct FileData *input) {
    if (ctx == NULL || input == NULL || input->data == NULL || input->size == 0) {
        return 0;
    }
    
    ctx->header.original_size = (uint32_t)input->size;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    
    size_t remaining = input->size;
    size_t processed = 0;
    
    while (remaining > 0 && ctx->block_count < ctx->max_blocks) {
        uint16_t chunk_size = (remaining > 256) ? 256 : (uint16_t)remaining;
        
        if (!add_compression_block(ctx, (uint32_t)processed, chunk_size, &input->data[processed])) {
            return 0;
        }
        
        ctx->header.compressed_size += chunk_size;
        processed += chunk_size;
        remaining -= chunk_size;
    }
    
    for (uint32_t i = 0; i < input->size; i++) {
        ctx->header.checksum ^= input->data[i];
    }
    
    return (remaining == 0);
}

int decompress_data(const struct CompressionContext *ctx, struct FileData *output) {
    if (ctx == NULL || output == NULL || output->data == NULL) {
        return 0;
    }
    
    if (ctx->header.magic != 0x43535455) {
        return 0;
    }
    
    size_t required_size = ctx->header.original_size;
    if (required_size > output->capacity) {
        return 0;
    }
    
    output->size = 0;
    uint8_t computed_checksum = 0;
    
    for (uint16_t i = 0; i < ctx->block_count; i++) {
        const struct CompressionBlock *block = &ctx->blocks[i];
        
        if (block->offset + block->length > required_size) {
            return 0;
        }
        
        if (output->size + block->length > output->capacity) {
            return 0;
        }
        
        memcpy(&output->data[block->offset], block->data, block->length);
        output->size = block->offset + block->length;
    }
    
    for (size_t i = 0; i < output->size; i++) {
        computed_checksum ^= output->data[i];
    }
    
    return (computed_checksum == ctx->header.checksum && output->size == required