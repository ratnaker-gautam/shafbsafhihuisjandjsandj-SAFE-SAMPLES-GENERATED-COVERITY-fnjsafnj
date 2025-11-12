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
    if (header->magic != 0x43535442) return 0;
    if (header->version > 1) return 0;
    if (header->original_size > SIZE_MAX) return 0;
    if (header->compressed_size > SIZE_MAX) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int initialize_context(struct CompressionContext *ctx, uint64_t original_size) {
    if (ctx == NULL) return 0;
    if (original_size == 0 || original_size > SIZE_MAX) return 0;
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    
    ctx->header.magic = 0x43535442;
    ctx->header.version = 1;
    ctx->header.original_size = original_size;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    memset(ctx->header.checksum, 0, sizeof(ctx->header.checksum));
    
    uint32_t block_count = (original_size + 4095) / 4096;
    if (block_count > 1000) return 0;
    
    ctx->blocks = malloc(block_count * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return 0;
    
    ctx->buffer_size = original_size;
    ctx->data_buffer = malloc(ctx->buffer_size);
    if (ctx->data_buffer == NULL) {
        free(ctx->blocks);
        return 0;
    }
    
    ctx->block_count = block_count;
    
    for (uint32_t i = 0; i < block_count; i++) {
        ctx->blocks[i].block_id = i;
        ctx->blocks[i].offset = i * 4096;
        ctx->blocks[i].size = (i == block_count - 1) ? 
            (original_size % 4096) : 4096;
        if (ctx->blocks[i].size == 0) ctx->blocks[i].size = 4096;
        ctx->blocks[i].flags = 0;
    }
    
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    free(ctx->blocks);
    free(ctx->data_buffer);
    memset(ctx, 0, sizeof(struct CompressionContext));
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL) return 0;
    if (input_size != ctx->header.original_size) return 0;
    if (input_size > ctx->buffer_size) return 0;
    
    memcpy(ctx->data_buffer, input, input_size);
    
    uint64_t compressed_size = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].offset + ctx->blocks[i].size > input_size) return 0;
        compressed_size += ctx->blocks[i].size;
        ctx->blocks[i].flags = 1;
    }
    
    ctx->header.compressed_size = compressed_size;
    
    for (int i = 0; i < 16; i++) {
        ctx->header.checksum[i] = (uint8_t)((input_size + i) % 256);
    }
    
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, size_t output_size) {
    if (ctx == NULL || output == NULL) return 0;
    if (!validate_header(&ctx->header)) return 0;
    if (output_size < ctx->header.original_size) return 0;
    
    memcpy(output, ctx->data_buffer, ctx->header.original_size);
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    const char test_data[] = "This is a test string for compression demonstration.";
    size_t data_size = strlen(test_data) + 1;
    
    if (!initialize_context(&ctx, data_size)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    if (!compress_data(&ctx, (const uint8_t*)test_data, data_size)) {
        fprintf(stderr, "Compression failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    printf("Original size: %lu bytes\n", ctx.header.original_size);
    printf("Compressed size: %lu bytes\n", ctx.header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
        (double)ctx.header.compressed_size / ctx.header.original_size * 100);
    
    char decompressed[256];
    if (!decompress_data(&ctx, (uint8