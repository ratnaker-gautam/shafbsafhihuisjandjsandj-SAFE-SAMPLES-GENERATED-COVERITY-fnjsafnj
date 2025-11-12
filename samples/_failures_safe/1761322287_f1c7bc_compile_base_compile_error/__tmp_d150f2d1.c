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
    uint8_t data[4096];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    size_t block_count;
    size_t max_blocks;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535455) return 0;
    if (header->version > 1) return 0;
    if (header->original_size > UINT64_MAX / 2) return 0;
    if (header->compressed_size > UINT64_MAX / 2) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int init_context(struct CompressionContext *ctx, size_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > SIZE_MAX / sizeof(struct CompressionBlock)) {
        return 0;
    }
    
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        return 0;
    }
    
    ctx->block_count = 0;
    ctx->max_blocks = max_blocks;
    
    memset(&ctx->header, 0, sizeof(struct CompressionHeader));
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    
    return 1;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 4096) {
        return 0;
    }
    
    if (ctx->block_count >= ctx->max_blocks) {
        return 0;
    }
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_id = ctx->block_count;
    block->offset = ctx->header.compressed_size;
    block->size = size;
    
    memcpy(block->data, data, size);
    
    ctx->header.original_size += size;
    ctx->header.compressed_size += size;
    ctx->block_count++;
    
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
        ctx->block_count = 0;
        ctx->max_blocks = 0;
    }
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) {
        return 0;
    }
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t chunk_size = input_size - processed;
        if (chunk_size > 4096) {
            chunk_size = 4096;
        }
        
        if (!add_block(ctx, input + processed, chunk_size)) {
            return 0;
        }
        
        processed += chunk_size;
    }
    
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, size_t output_size) {
    if (ctx == NULL || output == NULL || output_size < ctx->header.original_size) {
        return 0;
    }
    
    if (!validate_header(&ctx->header)) {
        return 0;
    }
    
    size_t offset = 0;
    for (size_t i = 0; i < ctx->block_count; i++) {
        const struct CompressionBlock *block = &ctx->blocks[i];
        if (block->size > 4096 || offset + block->size > output_size) {
            return 0;
        }
        
        memcpy(output + offset, block->data, block->size);
        offset += block->size;
    }
    
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    
    if (!init_context(&ctx, 10)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    uint8_t test_data[] = "This is a test string for compression demonstration. It contains multiple sentences to ensure we have enough data to fill multiple compression blocks for testing purposes.";
    size_t data_size = sizeof(test_data) - 1;
    
    printf("Original data size: %zu bytes\n", data_size);
    
    if (!compress_data(&ctx, test_data, data_size)) {
        fprintf(stderr, "Compression failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    printf("Compressed into %zu blocks\n", ctx.block_count);
    printf("Total compressed size: %lu bytes\n", ctx.header.compressed_size);
    
    uint8_t decompressed[1024];
    if (!decompress_data(&ctx, decompressed, sizeof(decompressed))) {
        fprintf(stderr, "Decompression failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    if (memcmp(test_data, decompressed, data_size) == 0) {
        printf("Decomp