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
    uint32_t max_blocks;
    uint8_t *input_data;
    size_t input_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->version > 2) return 0;
    if (header->algorithm > 5) return 0;
    if (header->original_size > 1024 * 1024 * 1024) return 0;
    if (header->compressed_size > 1024 * 1024 * 1024) return 0;
    return 1;
}

int init_compression_context(struct CompressionContext *ctx, size_t input_size) {
    if (ctx == NULL || input_size == 0 || input_size > 1024 * 1024) return 0;
    
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->header.original_size = input_size;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    memset(ctx->header.checksum, 0, sizeof(ctx->header.checksum));
    
    ctx->max_blocks = 16;
    ctx->blocks = malloc(ctx->max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return 0;
    
    ctx->block_count = 0;
    ctx->input_data = malloc(input_size);
    if (ctx->input_data == NULL) {
        free(ctx->blocks);
        return 0;
    }
    ctx->input_size = input_size;
    
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, uint32_t offset, uint32_t size, uint8_t flags) {
    if (ctx == NULL || ctx->blocks == NULL) return 0;
    if (offset >= ctx->input_size) return 0;
    if (size == 0 || size > ctx->input_size - offset) return 0;
    if (flags > 3) return 0;
    
    if (ctx->block_count >= ctx->max_blocks) {
        uint32_t new_max = ctx->max_blocks * 2;
        if (new_max > 1024) return 0;
        
        struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_max * sizeof(struct CompressionBlock));
        if (new_blocks == NULL) return 0;
        
        ctx->blocks = new_blocks;
        ctx->max_blocks = new_max;
    }
    
    ctx->blocks[ctx->block_count].block_id = ctx->block_count;
    ctx->blocks[ctx->block_count].offset = offset;
    ctx->blocks[ctx->block_count].size = size;
    ctx->blocks[ctx->block_count].flags = flags;
    ctx->block_count++;
    
    return 1;
}

void process_compression(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->block_count == 0) return;
    
    uint64_t total_compressed = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        uint32_t compressed_size = ctx->blocks[i].size - (ctx->blocks[i].size / 4);
        if (compressed_size < ctx->blocks[i].size / 2) {
            compressed_size = ctx->blocks[i].size / 2;
        }
        total_compressed += compressed_size;
    }
    
    ctx->header.compressed_size = total_compressed;
    
    for (int i = 0; i < 16; i++) {
        ctx->header.checksum[i] = (uint8_t)((i * 7 + total_compressed) & 0xFF);
    }
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->blocks != NULL) free(ctx->blocks);
    if (ctx->input_data != NULL) free(ctx->input_data);
}

int main(void) {
    struct CompressionContext ctx;
    char input_buffer[1024];
    size_t input_len;
    
    printf("Enter text to compress (max 1023 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!init_compression_context(&ctx, input_len)) {
        printf("Failed to initialize compression context\n");
        return 1;
    }
    
    memcpy(ctx.input_data, input_buffer, input_len);