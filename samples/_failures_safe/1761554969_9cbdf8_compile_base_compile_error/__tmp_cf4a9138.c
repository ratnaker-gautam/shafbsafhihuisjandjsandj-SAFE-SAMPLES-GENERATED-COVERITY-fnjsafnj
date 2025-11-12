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
    uint32_t offset;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
    uint32_t current_offset;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535455) return 0;
    if (header->compressed_size == 0) return 0;
    if (header->original_size == 0) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

struct CompressionContext* create_context(void) {
    struct CompressionContext *ctx = malloc(sizeof(struct CompressionContext));
    if (ctx == NULL) return NULL;
    
    ctx->header.magic = 0x43535455;
    ctx->header.compressed_size = 0;
    ctx->header.original_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
    ctx->current_offset = 0;
    
    return ctx;
}

void destroy_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        if (current->data != NULL) {
            free(current->data);
        }
        free(current);
        current = next;
    }
    free(ctx);
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) return 0;
    if (size > 1024 * 1024) return 0;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) return 0;
    
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return 0;
    }
    
    memcpy(block->data, data, size);
    block->size = size;
    block->offset = ctx->current_offset;
    block->next = NULL;
    
    ctx->current_offset += (uint32_t)size;
    ctx->header.compressed_size += (uint32_t)size;
    ctx->header.original_size += (uint32_t)size;
    
    if (ctx->last_block == NULL) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
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

int write_compressed_data(const struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL) return 0;
    if (!validate_header(&ctx->header)) return 0;
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) return 0;
    
    if (fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        if (fwrite(current->data, 1, current->size, file) != current->size) {
            fclose(file);
            return 0;
        }
        current = current->next;
    }
    
    fclose(file);
    return 1;
}

int main(void) {
    struct CompressionContext *ctx = create_context();
    if (ctx == NULL) {
        printf("Failed to create compression context\n");
        return 1;
    }
    
    uint8_t test_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20};
    uint8_t test_data2[] = {0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21};
    
    if (!add_block(ctx, test_data1, sizeof(test_data1))) {
        printf("Failed to add first data block\n");
        destroy_context(ctx);
        return 1;
    }
    
    if (!add_block(ctx, test_data2, sizeof(test_data2))) {
        printf("Failed to add second data block\n");
        destroy_context(ctx);
        return 1;
    }