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
};

struct CompressionBlock {
    uint8_t *data;
    size_t size;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) return 0;
    if (size > 1024) return 0;

    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) return 0;

    new_block->data = malloc(size);
    if (new_block->data == NULL) {
        free(new_block);
        return 0;
    }

    memcpy(new_block->data, data, size);
    new_block->size = size;
    new_block->next = NULL;

    if (ctx->first_block == NULL) {
        ctx->first_block = new_block;
        ctx->last_block = new_block;
    } else {
        ctx->last_block->next = new_block;
        ctx->last_block = new_block;
    }

    ctx->total_blocks++;
    ctx->header.compressed_size += size;
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    uint8_t sum = 0;
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        for (size_t i = 0; i < current->size; i++) {
            sum ^= current->data[i];
        }
        current = current->next;
    }
    ctx->header.checksum = sum;
}

void compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return;

    size_t processed = 0;
    while (processed < input_size) {
        size_t block_size = (input_size - processed) > 16 ? 16 : (input_size - processed);
        if (!add_block(ctx, input + processed, block_size)) {
            break;
        }
        processed += block_size;
    }

    ctx->header.original_size = input_size;
    calculate_checksum(ctx);
}

void free_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    printf("Compression Header:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Original Size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Total Blocks: %zu\n", ctx->total_blocks);

    struct CompressionBlock *current = ctx->first_block;
    size_t block_num = 0;
    while (current != NULL) {
        printf("  Block %zu: %zu bytes\n", block_num++, current->size);
        current = current->next;
    }
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    uint8_t test_data[] = {
        0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F,
        0x72, 0x6C, 0x64, 0x21, 0x0A, 0x54, 0x68, 0x69,
        0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74,
        0x65, 0x73, 0x74, 0x2E, 0x0A
    };
    size_t test_size = sizeof(test_data);

    compress_data(&ctx, test_data, test_size);

    print_compression_info(&ctx);

    free_compression_context(&ctx);
    return 0;
}