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
    uint8_t flags;
    uint8_t reserved;
};

struct CompressionBlock {
    uint32_t offset;
    uint32_t size;
    uint8_t checksum[16];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint8_t *input_data;
    uint8_t *output_data;
    size_t input_size;
    size_t output_size;
};

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0 || size > 1048576) {
        return 0;
    }
    return 1;
}

int initialize_context(struct CompressionContext *ctx, const uint8_t *input, size_t size) {
    if (ctx == NULL || !validate_input(input, size)) {
        return 0;
    }
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    
    ctx->input_data = malloc(size);
    if (ctx->input_data == NULL) {
        return 0;
    }
    memcpy(ctx->input_data, input, size);
    ctx->input_size = size;
    
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = (uint32_t)size;
    ctx->header.algorithm = 1;
    
    return 1;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->input_data == NULL) {
        return 0;
    }
    
    size_t output_needed = ctx->input_size + sizeof(struct CompressionHeader) + 64;
    if (output_needed < ctx->input_size) {
        return 0;
    }
    
    ctx->output_data = malloc(output_needed);
    if (ctx->output_data == NULL) {
        return 0;
    }
    
    uint8_t *output_ptr = ctx->output_data;
    memcpy(output_ptr, &ctx->header, sizeof(struct CompressionHeader));
    output_ptr += sizeof(struct CompressionHeader);
    
    size_t compressed_size = 0;
    for (size_t i = 0; i < ctx->input_size; i++) {
        if (compressed_size >= output_needed - sizeof(struct CompressionHeader)) {
            free(ctx->output_data);
            ctx->output_data = NULL;
            return 0;
        }
        output_ptr[compressed_size++] = ctx->input_data[i];
    }
    
    ctx->header.compressed_size = (uint32_t)compressed_size;
    memcpy(ctx->output_data, &ctx->header, sizeof(struct CompressionHeader));
    ctx->output_size = sizeof(struct CompressionHeader) + compressed_size;
    
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->input_data != NULL) {
        free(ctx->input_data);
        ctx->input_data = NULL;
    }
    
    if (ctx->output_data != NULL) {
        free(ctx->output_data);
        ctx->output_data = NULL;
    }
    
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
    
    memset(ctx, 0, sizeof(struct CompressionContext));
}

int main(void) {
    uint8_t test_data[256];
    for (int i = 0; i < 256; i++) {
        test_data[i] = (uint8_t)(i & 0xFF);
    }
    
    struct CompressionContext ctx;
    
    if (!initialize_context(&ctx, test_data, sizeof(test_data))) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    printf("Original size: %u bytes\n", ctx.header.original_size);
    printf("Starting compression...\n");
    
    if (!compress_data(&ctx)) {
        fprintf(stderr, "Compression failed\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    printf("Compressed size: %u bytes\n", ctx.header.compressed_size);
    printf("Total output size: %zu bytes\n", ctx.output_size);
    
    if (ctx.output_size > 0 && ctx.output_data != NULL) {
        printf("First 16 bytes of output: ");
        for (size_t i = 0; i < 16 && i < ctx.output_size; i++) {
            printf("%02X ", ctx.output_data[i]);
        }
        printf("\n");
    }
    
    double ratio = (double)ctx.header.compressed_size / ctx.header.original_size * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    cleanup_context(&ctx);
    return 0;
}