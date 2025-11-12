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
    uint32_t data_offset;
    uint8_t flags;
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

struct CompressionStats {
    uint32_t input_bytes;
    uint32_t output_bytes;
    double compression_ratio;
    uint32_t processing_time_ms;
};

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535442;
    ctx->header.algorithm = 1;
}

int validate_input(const uint8_t *data, uint32_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > 1048576) return 0;
    return 1;
}

int allocate_blocks(struct CompressionContext *ctx, uint32_t count) {
    if (count == 0 || count > 256) return 0;
    ctx->blocks = malloc(count * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return 0;
    ctx->block_count = count;
    return 1;
}

void simple_compress(struct CompressionContext *ctx) {
    if (ctx->input_size == 0) return;
    
    uint32_t output_needed = ctx->input_size + sizeof(struct CompressionHeader);
    if (output_needed < ctx->input_size) return;
    
    ctx->output_data = malloc(output_needed);
    if (ctx->output_data == NULL) return;
    
    ctx->header.original_size = ctx->input_size;
    ctx->header.compressed_size = ctx->input_size;
    
    memcpy(ctx->output_data, &ctx->header, sizeof(struct CompressionHeader));
    memcpy(ctx->output_data + sizeof(struct CompressionHeader), ctx->input_data, ctx->input_size);
    
    ctx->output_size = output_needed;
    
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < ctx->input_size; i++) {
        checksum ^= ctx->input_data[i];
    }
    ctx->header.checksum = checksum;
    memcpy(ctx->output_data, &ctx->header, sizeof(struct CompressionHeader));
}

void calculate_stats(const struct CompressionContext *ctx, struct CompressionStats *stats) {
    if (ctx == NULL || stats == NULL) return;
    
    stats->input_bytes = ctx->input_size;
    stats->output_bytes = ctx->output_size;
    
    if (ctx->input_size > 0) {
        stats->compression_ratio = (double)ctx->output_size / (double)ctx->input_size;
    } else {
        stats->compression_ratio = 0.0;
    }
    stats->processing_time_ms = 10;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
    if (ctx->input_data != NULL) {
        free(ctx->input_data);
        ctx->input_data = NULL;
    }
    if (ctx->output_data != NULL) {
        free(ctx->output_data);
        ctx->output_data = NULL;
    }
    ctx->block_count = 0;
    ctx->input_size = 0;
    ctx->output_size = 0;
}

int main(void) {
    struct CompressionContext ctx;
    struct CompressionStats stats;
    
    initialize_context(&ctx);
    
    printf("Enter data size (1-1024 bytes): ");
    uint32_t input_size;
    if (scanf("%u", &input_size) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_size == 0 || input_size > 1024) {
        printf("Invalid size range\n");
        return 1;
    }
    
    ctx.input_data = malloc(input_size);
    if (ctx.input_data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %u bytes of data: ", input_size);
    for (uint32_t i = 0; i < input_size; i++) {
        int byte;
        if (scanf("%2x", &byte) != 1) {
            printf("Invalid byte input\n");
            free(ctx.input_data);
            return 1;
        }
        ctx.input_data[i] = (uint8_t)byte;
    }
    
    ctx.input_size = input_size;
    
    if (!allocate_blocks(&ctx, 1)) {
        printf("Failed to allocate compression blocks\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    simple_compress(&ctx);
    calculate_stats(&ctx, &stats);
    
    printf("Compression completed:\n");
    printf("Input size: %u bytes\n", stats.input_bytes);
    printf("Output size: %u bytes\n", stats.output_bytes);
    printf("Compression ratio: %.2f\n", stats.compression_ratio);
    printf("Processing time: %u ms\n", stats.processing_time_ms);