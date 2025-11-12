//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t offset;
    uint16_t length;
    uint16_t encoded_length;
    uint8_t data[256];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock blocks[16];
    uint8_t num_blocks;
    uint8_t state;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->original_size > 1048576) return 0;
    if (header->compressed_size > 1048576) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535442;
    ctx->header.algorithm = 1;
    ctx->state = 1;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, size_t len) {
    if (ctx == NULL || data == NULL) return 0;
    if (ctx->num_blocks >= 16) return 0;
    if (len == 0 || len > 256) return 0;
    
    struct CompressionBlock *block = &ctx->blocks[ctx->num_blocks];
    block->offset = ctx->header.original_size;
    block->length = (uint16_t)len;
    block->encoded_length = (uint16_t)(len - 1);
    
    if (block->offset > UINT32_MAX - len) return 0;
    
    memcpy(block->data, data, len);
    ctx->header.original_size += (uint32_t)len;
    ctx->header.compressed_size += block->encoded_length;
    ctx->num_blocks++;
    
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    uint8_t sum = 0;
    for (int i = 0; i < ctx->num_blocks; i++) {
        struct CompressionBlock *block = &ctx->blocks[i];
        for (int j = 0; j < block->length; j++) {
            if (sum > UINT8_MAX - block->data[j]) {
                sum = 0;
                break;
            }
            sum += block->data[j];
        }
    }
    ctx->header.checksum = sum;
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
    
    for (int i = 0; i < ctx->num_blocks; i++) {
        const struct CompressionBlock *block = &ctx->blocks[i];
        if (fwrite(&block->offset, sizeof(uint32_t), 1, file) != 1) {
            fclose(file);
            return 0;
        }
        if (fwrite(&block->encoded_length, sizeof(uint16_t), 1, file) != 1) {
            fclose(file);
            return 0;
        }
        if (fwrite(block->data, 1, block->encoded_length, file) != block->encoded_length) {
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data1[] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x42, 0x43, 0x44};
    uint8_t test_data2[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
    uint8_t test_data3[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30};
    
    if (!add_block(&ctx, test_data1, sizeof(test_data1))) {
        fprintf(stderr, "Failed to add block 1\n");
        return 1;
    }
    
    if (!add_block(&ctx, test_data2, sizeof(test_data2))) {
        fprintf(stderr, "Failed to add block 2\n");
        return 1;
    }
    
    if (!add_block(&ctx, test_data3, sizeof(test_data3))) {
        fprintf(stderr, "