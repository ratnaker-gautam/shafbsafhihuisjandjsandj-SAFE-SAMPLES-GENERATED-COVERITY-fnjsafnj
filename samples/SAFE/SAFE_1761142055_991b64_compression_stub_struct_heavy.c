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
    if (header->original_size > 1000000) return 0;
    if (header->compressed_size > 1000000) return 0;
    if (header->algorithm > 5) return 0;
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (size == 0 || size > 8192) return 0;
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

    if (!ctx->first_block) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }

    ctx->total_blocks++;
    ctx->header.compressed_size += size;
    return 1;
}

void free_compression_context(struct CompressionContext *ctx) {
    struct CompressionBlock *current = ctx->first_block;
    while (current) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
}

uint8_t calculate_checksum(const struct CompressionContext *ctx) {
    uint8_t checksum = 0;
    const struct CompressionBlock *current = ctx->first_block;
    while (current) {
        for (uint32_t i = 0; i < current->block_size; i++) {
            checksum ^= current->data[i];
        }
        current = current->next;
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (!input || input_size == 0 || input_size > 1000000) return 0;

    initialize_context(ctx);
    ctx->header.original_size = input_size;

    uint32_t processed = 0;
    while (processed < input_size) {
        uint32_t block_size = (input_size - processed) > 512 ? 512 : (input_size - processed);
        if (!add_compression_block(ctx, input + processed, block_size)) {
            free_compression_context(ctx);
            return 0;
        }
        processed += block_size;
    }

    ctx->header.checksum = calculate_checksum(ctx);
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, uint32_t output_size) {
    if (!ctx || !output || output_size < ctx->header.original_size) return 0;
    if (!validate_header(&ctx->header)) return 0;

    uint32_t total_size = 0;
    const struct CompressionBlock *current = ctx->first_block;
    while (current) {
        if (total_size + current->block_size > output_size) return 0;
        memcpy(output + total_size, current->data, current->block_size);
        total_size += current->block_size;
        current = current->next;
    }

    if (total_size != ctx->header.original_size) return 0;
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    uint8_t test_data[1024];
    uint8_t decompressed[1024];

    for (int i = 0; i < 1024; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }

    printf("Starting compression test...\n");

    if (!compress_data(&ctx, test_data, 1024)) {
        printf("Compression failed\n");
        return 1;
    }

    printf("Compression successful: %u -> %u bytes\n", 
           ctx.header.original_size, ctx.header.compressed_size);

    if (!decompress_data(&ctx, decompressed, 1024)) {
        printf("Decompression failed\n");
        free_compression_context(&ctx);
        return 1;
    }

    if (memcmp(test_data, decompressed, 1024) == 0) {
        printf("Decompression verified: data matches original\n");
    } else {
        printf("Decompression error: data mismatch\n");
    }

    free_compression_context(&ctx);
    return 0;
}