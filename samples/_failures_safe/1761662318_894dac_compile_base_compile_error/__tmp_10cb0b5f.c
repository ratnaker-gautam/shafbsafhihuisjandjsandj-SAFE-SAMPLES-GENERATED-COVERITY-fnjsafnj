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
    uint8_t *data;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    uint32_t total_blocks;
};

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

int validate_header(const struct CompressionHeader *header) {
    if (header->magic != 0x43535455) return 0;
    if (header->original_size > 100000000) return 0;
    if (header->compressed_size > 100000000) return 0;
    if (header->algorithm > 5) return 0;
    return 1;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (size == 0 || size > 65536) return 0;
    if (ctx->total_blocks >= 1000) return 0;

    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (!block) return 0;

    block->data = malloc(size);
    if (!block->data) {
        free(block);
        return 0;
    }

    memcpy(block->data, data, size);
    block->block_size = size;
    block->next = NULL;

    if (ctx->last_block) {
        ctx->last_block->next = block;
    } else {
        ctx->first_block = block;
    }
    ctx->last_block = block;
    ctx->total_blocks++;
    ctx->header.compressed_size += size;

    return 1;
}

void free_blocks(struct CompressionBlock *block) {
    while (block) {
        struct CompressionBlock *next = block->next;
        free(block->data);
        free(block);
        block = next;
    }
}

uint8_t calculate_checksum(const struct CompressionContext *ctx) {
    uint8_t checksum = 0;
    const struct CompressionBlock *block = ctx->first_block;
    while (block) {
        for (uint32_t i = 0; i < block->block_size; i++) {
            checksum ^= block->data[i];
        }
        block = block->next;
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (!input || input_size == 0 || input_size > 1000000) return 0;

    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;

    uint32_t processed = 0;
    while (processed < input_size) {
        uint32_t block_size = (input_size - processed) > 1024 ? 1024 : (input_size - processed);
        if (!add_block(ctx, input + processed, block_size)) {
            free_blocks(ctx->first_block);
            return 0;
        }
        processed += block_size;
    }

    ctx->header.checksum = calculate_checksum(ctx);
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, uint32_t output_size) {
    if (!output || output_size < ctx->header.original_size) return 0;
    if (!validate_header(&ctx->header)) return 0;

    uint8_t computed_checksum = calculate_checksum(ctx);
    if (computed_checksum != ctx->header.checksum) return 0;

    uint32_t offset = 0;
    const struct CompressionBlock *block = ctx->first_block;
    while (block) {
        if (offset + block->block_size > output_size) return 0;
        memcpy(output + offset, block->data, block->block_size);
        offset += block->block_size;
        block = block->next;
    }

    return offset == ctx->header.original_size;
}

int main() {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    uint8_t test_data[2048];
    for (int i = 0; i < 2048; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }

    printf("Original data size: %u bytes\n", 2048);

    if (!compress_data(&ctx, test_data, 2048)) {
        printf("Compression failed\n");
        free_blocks(ctx.first_block);
        return 1;
    }

    printf("Compressed to %u bytes in %u blocks\n", 
           ctx.header.compressed_size, ctx.total_blocks);
    printf("Header checksum: 0x%02X\n", ctx.header.checksum);

    uint8_t decompressed[2048];
    if (!decompress_data(&ctx, decompressed, 2048)) {
        printf("Decompression failed\n");
        free_blocks(ctx.first_block);
        return 1;
    }

    if (memcmp(test_data, decompressed, 2048) == 0) {
        printf("Decompression verified: data matches original\n");
    } else {
        printf("Decompression error: data mismatch\n");