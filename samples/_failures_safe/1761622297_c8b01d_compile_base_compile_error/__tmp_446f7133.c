//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t compressed_size;
    uint32_t original_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t offset;
    uint32_t size;
    uint8_t *data;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    uint32_t total_blocks;
    uint32_t max_block_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->compressed_size > 1000000) return 0;
    if (header->original_size > 1000000) return 0;
    if (header->algorithm > 5) return 0;
    return 1;
}

struct CompressionBlock *create_block(uint32_t offset, uint32_t size, const uint8_t *data) {
    if (data == NULL || size == 0 || size > 4096) return NULL;
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) return NULL;
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return NULL;
    }
    memcpy(block->data, data, size);
    block->offset = offset;
    block->size = size;
    block->next = NULL;
    return block;
}

void free_block(struct CompressionBlock *block) {
    if (block == NULL) return;
    free(block->data);
    free(block);
}

void free_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        free_block(current);
        current = next;
    }
    free(ctx);
}

struct CompressionContext *create_context(void) {
    struct CompressionContext *ctx = malloc(sizeof(struct CompressionContext));
    if (ctx == NULL) return NULL;
    ctx->header.magic = 0x43535442;
    ctx->header.compressed_size = 0;
    ctx->header.original_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    ctx->header.flags = 0;
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
    ctx->max_block_size = 4096;
    return ctx;
}

int add_block(struct CompressionContext *ctx, uint32_t offset, uint32_t size, const uint8_t *data) {
    if (ctx == NULL || data == NULL) return 0;
    if (size == 0 || size > ctx->max_block_size) return 0;
    struct CompressionBlock *block = create_block(offset, size, data);
    if (block == NULL) return 0;
    if (ctx->first_block == NULL) {
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

void simulate_compression(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return;
    uint32_t processed = 0;
    while (processed < input_size) {
        uint32_t block_size = (input_size - processed < ctx->max_block_size) ? 
                             input_size - processed : ctx->max_block_size;
        if (!add_block(ctx, processed, block_size, input + processed)) {
            break;
        }
        processed += block_size;
    }
    ctx->header.original_size = input_size;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    printf("Compression Header:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Compressed Size: %u\n", ctx->header.compressed_size);
    printf("  Original Size: %u\n", ctx->header.original_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Flags: 0x%02X\n", ctx->header.flags);
    printf("Total Blocks: %u\n", ctx->total_blocks);
    printf("Max Block Size: %u\n", ctx->max_block_size);
}

int main(void) {
    struct CompressionContext *ctx = create_context();
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create compression context\n");
        return 1;
    }

    uint8_t test_data[256];
    for (int i = 0; i < 256; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }

    simulate_compression(ctx, test_data, sizeof(test_data));
    print_compression_info(ctx);