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
    uint32_t block_size;
    uint32_t checksum;
    uint8_t data[1024];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint32_t blocks_processed;
};

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
}

int validate_header(const struct CompressionHeader *header) {
    if (header->magic != 0x43535455) return 0;
    if (header->algorithm > 3) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > 104857600) return 0;
    return 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL) return 0;
    if (size > sizeof(ctx->current_block.data)) return 0;
    if (ctx->total_processed + size > ctx->header.original_size) return 0;

    memcpy(ctx->current_block.data, data, size);
    ctx->current_block.block_size = size;

    uint32_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum = (checksum << 3) ^ data[i];
    }
    ctx->current_block.checksum = checksum;

    ctx->total_processed += size;
    ctx->blocks_processed++;
    return 1;
}

void simulate_compression(struct CompressionContext *ctx) {
    if (ctx->total_processed == 0) return;
    
    ctx->header.compressed_size = ctx->total_processed * 3 / 4;
    if (ctx->header.compressed_size < ctx->total_processed / 2) {
        ctx->header.compressed_size = ctx->total_processed / 2;
    }
}

void print_compression_info(const struct CompressionContext *ctx) {
    printf("Compression completed:\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Blocks processed: %u\n", ctx->blocks_processed);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (float)ctx->header.compressed_size / ctx->header.original_size) * 100.0);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    printf("Enter original data size (bytes, max 104857600): ");
    if (scanf("%u", &ctx.header.original_size) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    if (ctx.header.original_size == 0 || ctx.header.original_size > 104857600) {
        printf("Invalid size\n");
        return 1;
    }

    printf("Select algorithm (1-3): ");
    if (scanf("%hu", &ctx.header.algorithm) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    if (ctx.header.algorithm < 1 || ctx.header.algorithm > 3) {
        printf("Invalid algorithm\n");
        return 1;
    }

    uint32_t remaining = ctx.header.original_size;
    uint8_t buffer[1024];

    while (remaining > 0) {
        size_t block_size = (remaining < sizeof(buffer)) ? remaining : sizeof(buffer);
        
        for (size_t i = 0; i < block_size; i++) {
            buffer[i] = (uint8_t)(i % 256);
        }

        if (!process_block(&ctx, buffer, block_size)) {
            printf("Block processing failed\n");
            return 1;
        }

        remaining -= block_size;
        printf("Processed block %u: %zu bytes\n", ctx.blocks_processed, block_size);
    }

    simulate_compression(&ctx);
    
    if (!validate_header(&ctx.header)) {
        printf("Header validation failed\n");
        return 1;
    }

    print_compression_info(&ctx);
    return 0;
}