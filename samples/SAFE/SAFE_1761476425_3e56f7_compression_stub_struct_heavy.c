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
    if (header->original_size > 1000000) return 0;
    if (header->compressed_size > 1000000) return 0;
    if (header->algorithm > 5) return 0;
    return 1;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
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

void compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (!input || input_size == 0) return;
    if (input_size > 1000000) return;

    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;

    uint32_t processed = 0;
    while (processed < input_size) {
        uint32_t block_size = (input_size - processed) < 512 ? (input_size - processed) : 512;
        if (!add_block(ctx, input + processed, block_size)) break;
        processed += block_size;
    }

    uint8_t checksum = 0;
    for (uint32_t i = 0; i < input_size; i++) {
        checksum ^= input[i];
    }
    ctx->header.checksum = checksum;
}

void decompress_data(const struct CompressionContext *ctx, uint8_t *output, uint32_t output_size) {
    if (!output || output_size < ctx->header.original_size) return;

    uint32_t offset = 0;
    struct CompressionBlock *current = ctx->first_block;
    while (current && offset < output_size) {
        uint32_t copy_size = current->block_size;
        if (offset + copy_size > output_size) copy_size = output_size - offset;
        memcpy(output + offset, current->data, copy_size);
        offset += copy_size;
        current = current->next;
    }
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

int main() {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    uint8_t test_data[1000];
    for (int i = 0; i < 1000; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }

    compress_data(&ctx, test_data, 1000);

    if (!validate_header(&ctx.header)) {
        printf("Invalid compression header\n");
        free_compression_context(&ctx);
        return 1;
    }

    printf("Original size: %u\n", ctx.header.original_size);
    printf("Compressed size: %u\n", ctx.header.compressed_size);
    printf("Algorithm: %u\n", ctx.header.algorithm);
    printf("Checksum: %u\n", ctx.header.checksum);
    printf("Blocks: %u\n", ctx.total_blocks);

    uint8_t decompressed[1000];
    decompress_data(&ctx, decompressed, 1000);

    uint8_t checksum = 0;
    for (int i = 0; i < 1000; i++) {
        checksum ^= decompressed[i];
    }

    if (checksum == ctx.header.checksum) {
        printf("Decompression successful\n");
    } else {
        printf("Decompression failed\n");
    }

    free_compression_context(&ctx);
    return 0;
}