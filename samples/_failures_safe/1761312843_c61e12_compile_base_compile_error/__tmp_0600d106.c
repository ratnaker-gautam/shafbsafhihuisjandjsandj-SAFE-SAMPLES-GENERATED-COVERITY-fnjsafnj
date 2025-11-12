//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t data_offset;
    uint8_t block_type;
    uint8_t reserved[3];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint8_t *input_data;
    uint32_t input_size;
    uint8_t *output_data;
    uint32_t output_size;
};

int validate_input(const uint8_t *data, uint32_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > UINT32_MAX / 2) return 0;
    return 1;
}

int initialize_context(struct CompressionContext *ctx, const uint8_t *input, uint32_t size) {
    if (ctx == NULL || !validate_input(input, size)) return 0;
    
    ctx->input_data = malloc(size);
    if (ctx->input_data == NULL) return 0;
    memcpy(ctx->input_data, input, size);
    ctx->input_size = size;
    
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = size;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    ctx->header.flags = 0;
    
    ctx->block_count = (size + 4095) / 4096;
    if (ctx->block_count > 0) {
        ctx->blocks = calloc(ctx->block_count, sizeof(struct CompressionBlock));
        if (ctx->blocks == NULL) {
            free(ctx->input_data);
            return 0;
        }
    } else {
        ctx->blocks = NULL;
    }
    
    ctx->output_data = NULL;
    ctx->output_size = 0;
    
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->input_data != NULL) free(ctx->input_data);
    if (ctx->blocks != NULL) free(ctx->blocks);
    if (ctx->output_data != NULL) free(ctx->output_data);
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->input_data == NULL) return 0;
    
    uint32_t output_size = ctx->input_size + sizeof(struct CompressionHeader) + 
                          ctx->block_count * sizeof(struct CompressionBlock);
    
    if (output_size < ctx->input_size) return 0;
    
    ctx->output_data = malloc(output_size);
    if (ctx->output_data == NULL) return 0;
    
    uint8_t *output_ptr = ctx->output_data;
    memcpy(output_ptr, &ctx->header, sizeof(struct CompressionHeader));
    output_ptr += sizeof(struct CompressionHeader);
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        uint32_t block_start = i * 4096;
        uint32_t block_size = (i == ctx->block_count - 1) ? 
                             ctx->input_size - block_start : 4096;
        
        if (block_start + block_size > ctx->input_size) {
            free(ctx->output_data);
            ctx->output_data = NULL;
            return 0;
        }
        
        ctx->blocks[i].block_size = block_size;
        ctx->blocks[i].data_offset = output_ptr - ctx->output_data;
        ctx->blocks[i].block_type = 0;
        
        memcpy(output_ptr, ctx->input_data + block_start, block_size);
        output_ptr += block_size;
        
        memcpy(ctx->output_data + sizeof(struct CompressionHeader) + 
               i * sizeof(struct CompressionBlock), &ctx->blocks[i], sizeof(struct CompressionBlock));
    }
    
    ctx->output_size = output_ptr - ctx->output_data;
    ctx->header.compressed_size = ctx->output_size;
    
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < ctx->output_size; i++) {
        checksum ^= ctx->output_data[i];
    }
    ctx->header.checksum = checksum;
    memcpy(ctx->output_data, &ctx->header, sizeof(struct CompressionHeader));
    
    return 1;
}

int main(void) {
    uint8_t test_data[8192];
    for (int i = 0; i < 8192; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    struct CompressionContext ctx;
    if (!initialize_context(&ctx, test_data, 8192)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    if (!compress_data(&ctx)) {
        fprintf(stderr, "Compression failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    printf("Original size: %u bytes\n", ctx.header.original_size);
    printf("Compressed size: %u bytes\n", ctx.header.compressed_size);
    float ratio = (float)ctx.header.compressed_size / ctx.header.original_size * 100.0f;
    printf("Compression