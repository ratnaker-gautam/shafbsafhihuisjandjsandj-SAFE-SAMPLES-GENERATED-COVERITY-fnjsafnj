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
    uint8_t *data;
    size_t size;
    size_t capacity;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
    int compression_level;
};

struct CompressionResult {
    int success;
    size_t original_size;
    size_t compressed_size;
    double ratio;
};

void init_compression_context(struct CompressionContext *ctx, int level) {
    if (level < 0 || level > 9) level = 6;
    memset(ctx, 0, sizeof(*ctx));
    ctx->compression_level = level;
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
}

int add_data_to_context(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (!block) return 0;
    
    block->data = malloc(size);
    if (!block->data) {
        free(block);
        return 0;
    }
    
    memcpy(block->data, data, size);
    block->size = size;
    block->capacity = size;
    block->next = NULL;
    
    if (!ctx->first_block) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    ctx->header.original_size += size;
    return 1;
}

struct CompressionResult perform_compression(struct CompressionContext *ctx) {
    struct CompressionResult result = {0};
    if (!ctx || !ctx->first_block) return result;
    
    size_t compressed = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current) {
        compressed += current->size;
        current = current->next;
    }
    
    ctx->header.compressed_size = compressed;
    ctx->header.checksum = 0;
    
    for (int i = 0; i < 8; i++) {
        ctx->header.checksum ^= (ctx->header.magic >> (i * 4)) & 0xF;
    }
    
    result.success = 1;
    result.original_size = ctx->header.original_size;
    result.compressed_size = compressed;
    result.ratio = (compressed > 0) ? (double)ctx->header.original_size / compressed : 0.0;
    
    return result;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (!ctx) return;
    
    struct CompressionBlock *current = ctx->first_block;
    while (current) {
        struct CompressionBlock *next = current->next;
        if (current->data) free(current->data);
        free(current);
        current = next;
    }
    
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
}

int main() {
    struct CompressionContext ctx;
    init_compression_context(&ctx, 6);
    
    printf("Compression stub demonstration\n");
    printf("Enter up to 3 data chunks (empty line to finish):\n");
    
    char buffer[256];
    int chunk_count = 0;
    
    while (chunk_count < 3) {
        printf("Chunk %d: ", chunk_count + 1);
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[--len] = 0;
        if (len == 0) break;
        
        if (!add_data_to_context(&ctx, (uint8_t*)buffer, len)) {
            printf("Failed to add chunk\n");
            break;
        }
        
        chunk_count++;
    }
    
    if (ctx.header.original_size == 0) {
        printf("No data to compress\n");
        cleanup_compression_context(&ctx);
        return 1;
    }
    
    struct CompressionResult result = perform_compression(&ctx);
    
    printf("\nCompression results:\n");
    printf("Original size: %zu bytes\n", result.original_size);
    printf("Compressed size: %zu bytes\n", result.compressed_size);
    printf("Compression ratio: %.2f:1\n", result.ratio);
    printf("Magic: 0x%08X\n", ctx.header.magic);
    printf("Algorithm: %u\n", ctx.header.algorithm);
    printf("Checksum: 0x%02X\n", ctx.header.checksum);
    
    cleanup_compression_context(&ctx);
    return 0;
}