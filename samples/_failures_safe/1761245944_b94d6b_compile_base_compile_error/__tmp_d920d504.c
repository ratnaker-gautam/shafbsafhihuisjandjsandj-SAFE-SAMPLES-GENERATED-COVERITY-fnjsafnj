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
    if (ctx == NULL) {
        return;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) {
        return 0;
    }

    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) {
        return 0;
    }

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
    ctx->header.original_size += size;
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->first_block == NULL) {
        return;
    }

    uint8_t checksum = 0;
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        for (size_t i = 0; i < current->size; i++) {
            checksum ^= current->data[i];
        }
        current = current->next;
    }
    ctx->header.checksum = checksum;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->first_block == NULL) {
        return 0;
    }

    size_t total_size = 0;
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        total_size += current->size;
        current = current->next;
    }

    if (total_size > UINT32_MAX) {
        return 0;
    }

    ctx->header.compressed_size = (uint32_t)(total_size * 0.8);
    if (ctx->header.compressed_size > ctx->header.original_size) {
        ctx->header.compressed_size = ctx->header.original_size;
    }
    ctx->header.algorithm = 1;

    calculate_checksum(ctx);
    return 1;
}

void free_blocks(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }

    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }

    printf("Compression Results:\n");
    printf("Magic: 0x%08X\n", ctx->header.magic);
    printf("Original Size: %u bytes\n", ctx->header.original_size);
    printf("Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression Ratio: %.2f%%\n", 
           (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100.0);
    printf("Algorithm: %u\n", ctx->header.algorithm);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
    printf("Blocks Processed: %zu\n", ctx->total_blocks);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    uint8_t sample_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
    uint8_t sample_data2[] = {0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74};
    uint8_t sample_data3[] = {0x43, 0x6F, 0x6D, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6F, 0x6E,