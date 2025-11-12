//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
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
    size_t block_count;
    size_t block_capacity;
    uint8_t *buffer;
    size_t buffer_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->version > 1) return 0;
    if (header->original_size > SIZE_MAX) return 0;
    if (header->compressed_size > SIZE_MAX) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int add_block(struct CompressionContext *ctx, uint32_t block_id, uint32_t offset, uint32_t size, uint8_t flags) {
    if (ctx == NULL) return 0;
    if (size == 0 || size > UINT32_MAX / 2) return 0;
    
    if (ctx->block_count >= ctx->block_capacity) {
        size_t new_capacity = ctx->block_capacity == 0 ? 4 : ctx->block_capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(struct CompressionBlock)) return 0;
        struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_capacity * sizeof(struct CompressionBlock));
        if (new_blocks == NULL) return 0;
        ctx->blocks = new_blocks;
        ctx->block_capacity = new_capacity;
    }
    
    ctx->blocks[ctx->block_count].block_id = block_id;
    ctx->blocks[ctx->block_count].offset = offset;
    ctx->blocks[ctx->block_count].size = size;
    ctx->blocks[ctx->block_count].flags = flags;
    ctx->block_count++;
    return 1;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *data, size_t data_size) {
    if (ctx == NULL || data == NULL || data_size == 0 || data_size > SIZE_MAX / 2) return 0;
    
    ctx->buffer_size = data_size + 1024;
    if (ctx->buffer_size < data_size) return 0;
    ctx->buffer = malloc(ctx->buffer_size);
    if (ctx->buffer == NULL) return 0;
    
    memcpy(ctx->buffer, data, data_size);
    
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->header.original_size = data_size;
    ctx->header.compressed_size = data_size;
    ctx->header.algorithm = 1;
    memset(ctx->header.checksum, 0, sizeof(ctx->header.checksum));
    
    if (!add_block(ctx, 0, 0, data_size, 1)) {
        free(ctx->buffer);
        ctx->buffer = NULL;
        return 0;
    }
    
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, size_t output_size) {
    if (ctx == NULL || output == NULL || output_size == 0) return 0;
    if (!validate_header(&ctx->header)) return 0;
    if (ctx->header.original_size > output_size) return 0;
    if (ctx->buffer == NULL) return 0;
    
    memcpy(output, ctx->buffer, ctx->header.original_size);
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    free(ctx->blocks);
    free(ctx->buffer);
    ctx->blocks = NULL;
    ctx->buffer = NULL;
    ctx->block_count = 0;
    ctx->block_capacity = 0;
    ctx->buffer_size = 0;
}

int main(void) {
    struct CompressionContext ctx = {0};
    const char test_data[] = "This is a test string for compression demonstration.";
    size_t data_size = strlen(test_data);
    
    printf("Original data: %s\n", test_data);
    printf("Original size: %zu bytes\n", data_size);
    
    if (!compress_data(&ctx, (const uint8_t*)test_data, data_size)) {
        printf("Compression failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    printf("Compressed size: %lu bytes\n", ctx.header.compressed_size);
    printf("Using algorithm: %u\n", ctx.header.algorithm);
    
    char decompressed[256] = {0};
    if (!decompress_data(&ctx, (uint8_t*)decompressed, sizeof(decompressed))) {
        printf("Decompression failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    printf("Decompressed data: %s\n", decompressed);
    
    if (strcmp(test_data, decompressed) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }