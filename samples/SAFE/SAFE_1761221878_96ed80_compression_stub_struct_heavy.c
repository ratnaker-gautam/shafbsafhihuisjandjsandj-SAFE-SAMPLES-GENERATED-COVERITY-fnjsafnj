//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint64_t original_size;
    uint64_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum[16];
};

struct CompressionBlock {
    uint32_t block_id;
    uint32_t offset;
    uint32_t size;
    uint8_t flags;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint8_t *data_buffer;
    size_t buffer_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->version > 2) return 0;
    if (header->algorithm > 5) return 0;
    if (header->original_size > SIZE_MAX) return 0;
    if (header->compressed_size > SIZE_MAX) return 0;
    return 1;
}

int validate_block(const struct CompressionBlock *block, size_t max_offset) {
    if (block == NULL) return 0;
    if (block->offset > max_offset) return 0;
    if (block->size > SIZE_MAX) return 0;
    if (block->flags > 7) return 0;
    return 1;
}

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
}

int add_block(struct CompressionContext *ctx, uint32_t offset, uint32_t size, uint8_t flags) {
    if (ctx == NULL) return 0;
    if (offset > SIZE_MAX || size > SIZE_MAX) return 0;
    
    uint32_t new_count = ctx->block_count + 1;
    if (new_count < ctx->block_count) return 0;
    
    struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_count * sizeof(struct CompressionBlock));
    if (new_blocks == NULL) return 0;
    
    ctx->blocks = new_blocks;
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_id = ctx->block_count;
    block->offset = offset;
    block->size = size;
    block->flags = flags & 7;
    
    ctx->block_count = new_count;
    return 1;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL) return 0;
    if (input_size > SIZE_MAX / 2) return 0;
    
    size_t buffer_needed = input_size + 1024;
    if (buffer_needed < input_size) return 0;
    
    uint8_t *new_buffer = realloc(ctx->data_buffer, buffer_needed);
    if (new_buffer == NULL) return 0;
    
    ctx->data_buffer = new_buffer;
    ctx->buffer_size = buffer_needed;
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < input_size; i += 64) {
        size_t block_size = (input_size - i) < 64 ? (input_size - i) : 64;
        if (!add_block(ctx, compressed_size, block_size, 1)) return 0;
        
        if (compressed_size + block_size > ctx->buffer_size) return 0;
        memcpy(ctx->data_buffer + compressed_size, input + i, block_size);
        compressed_size += block_size;
    }
    
    ctx->header.original_size = input_size;
    ctx->header.compressed_size = compressed_size;
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    free(ctx->blocks);
    free(ctx->data_buffer);
    memset(ctx, 0, sizeof(*ctx));
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data[256];
    for (size_t i = 0; i < sizeof(test_data); i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    if (!compress_data(&ctx, test_data, sizeof(test_data))) {
        printf("Compression failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    printf("Compression successful\n");
    printf("Original size: %lu\n", (unsigned long)ctx.header.original_size);
    printf("Compressed size: %lu\n", (unsigned long)ctx.header.compressed_size);
    printf("Block count: %u\n", ctx.block_count);
    
    size_t total_data = 0;
    for (uint32_t i = 0; i < ctx.block_count; i++) {
        if (!validate_block(&ctx.blocks[i], ctx.buffer_size)) {
            printf("Invalid block detected\n");
            cleanup_context(&ctx);
            return 1;
        }
        total_data += ctx.blocks[i].size;
    }
    
    printf("Total data in blocks: %lu\n", (unsigned long)total_data);
    
    cleanup_context(&ctx);
    return 0;
}