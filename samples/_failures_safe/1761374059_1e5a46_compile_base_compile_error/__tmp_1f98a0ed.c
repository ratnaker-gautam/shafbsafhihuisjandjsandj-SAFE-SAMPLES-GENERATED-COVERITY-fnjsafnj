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
    uint8_t *data;
    size_t size;
    uint32_t offset;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
    uint32_t max_block_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->original_size == 0) return 0;
    if (header->compressed_size > header->original_size) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

struct CompressionBlock *create_block(size_t size, uint32_t offset) {
    if (size == 0 || size > 65536) return NULL;
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) return NULL;
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return NULL;
    }
    block->size = size;
    block->offset = offset;
    block->next = NULL;
    return block;
}

void free_blocks(struct CompressionBlock *block) {
    while (block != NULL) {
        struct CompressionBlock *next = block->next;
        free(block->data);
        free(block);
        block = next;
    }
}

struct CompressionContext *create_context(uint32_t max_block_size) {
    if (max_block_size == 0 || max_block_size > 65536) return NULL;
    struct CompressionContext *ctx = malloc(sizeof(struct CompressionContext));
    if (ctx == NULL) return NULL;
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
    ctx->max_block_size = max_block_size;
    return ctx;
}

void free_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    free_blocks(ctx->first_block);
    free(ctx);
}

int add_block_data(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) return 0;
    if (size > ctx->max_block_size) return 0;
    uint32_t offset = ctx->header.original_size;
    if (offset + size < offset) return 0;
    struct CompressionBlock *block = create_block(size, offset);
    if (block == NULL) return 0;
    memcpy(block->data, data, size);
    if (ctx->first_block == NULL) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    ctx->total_blocks++;
    ctx->header.original_size += size;
    ctx->header.compressed_size += size;
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    uint8_t checksum = 0;
    struct CompressionBlock *block = ctx->first_block;
    while (block != NULL) {
        for (size_t i = 0; i < block->size; i++) {
            checksum ^= block->data[i];
        }
        block = block->next;
    }
    ctx->header.checksum = checksum;
}

int write_compressed_data(const struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL) return 0;
    if (!validate_header(&ctx->header)) return 0;
    FILE *file = fopen(filename, "wb");
    if (file == NULL) return 0;
    if (fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    struct CompressionBlock *block = ctx->first_block;
    while (block != NULL) {
        if (fwrite(block->data, 1, block->size, file) != block->size) {
            fclose(file);
            return 0;
        }
        block = block->next;
    }
    fclose(file);
    return 1;
}

int main() {
    struct CompressionContext *ctx = create_context(1024);
    if (ctx == NULL) {
        printf("Failed to create compression context\n");
        return 1;
    }
    uint8_t test_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20};
    uint8_t test_data2[] = {0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21};
    if (!add_block_data(ctx, test