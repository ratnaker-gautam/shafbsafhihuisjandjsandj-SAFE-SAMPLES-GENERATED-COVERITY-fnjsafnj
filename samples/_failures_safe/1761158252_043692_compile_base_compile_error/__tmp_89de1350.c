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

struct CompressionResult {
    int success;
    uint32_t original_size;
    uint32_t compressed_size;
    float ratio;
};

void initialize_compression_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        if (current->data != NULL) {
            free(current->data);
        }
        free(current);
        current = next;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
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
    new_block->block_size = size;
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

struct CompressionResult compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    struct CompressionResult result = {0};

    if (ctx == NULL || input == NULL || input_size == 0) {
        return result;
    }

    initialize_compression_context(ctx);
    ctx->header.original_size = input_size;

    uint32_t processed = 0;
    while (processed < input_size) {
        uint32_t block_size = (input_size - processed < 1024) ? input_size - processed : 1024;
        
        if (!add_compression_block(ctx, input + processed, block_size)) {
            cleanup_compression_context(ctx);
            return result;
        }
        
        processed += block_size;
    }

    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    for (uint32_t i = 0; i < input_size; i++) {
        ctx->header.checksum ^= input[i];
    }

    result.success = 1;
    result.original_size = input_size;
    result.compressed_size = ctx->header.compressed_size;
    result.ratio = (float)result.compressed_size / (float)result.original_size;
    
    return result;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, uint32_t output_size) {
    if (ctx == NULL || output == NULL || output_size < ctx->header.original_size) {
        return 0;
    }

    uint32_t decompressed = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current != NULL && decompressed < ctx->header.original_size) {
        uint32_t copy_size = current->block_size;
        if (decompressed + copy_size > ctx->header.original_size) {
            copy_size = ctx->header.original_size - decompressed;
        }
        
        memcpy(output + decompressed, current->data, copy_size);
        decompressed += copy_size;
        current = current->next;
    }

    uint8_t checksum = 0;
    for (uint32_t i = 0; i < decompressed; i++) {
        checksum ^= output[i];
    }

    return (decompressed == ctx->header.original_size && checksum == ctx->header.checksum);
}

int main() {
    struct CompressionContext ctx;
    uint8_t test_data[2048];
    
    for (int i = 0; i < 2048; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }

    struct CompressionResult result = compress_data(&ctx, test_data, sizeof(test_data));
    
    if (result.success) {
        printf("Compression successful\n");
        printf("Original size: %u bytes\n", result.original_size);
        printf("Compressed size: %u bytes\n", result.compressed_size);
        printf("Compression ratio: %.2f\n", result.ratio);
        
        uint8_t decompressed[2048];
        if (decompress_data(&ctx, decompressed, sizeof(decompressed))) {
            printf("Decompression successful\n");
            
            if (memcmp(test_data, decompressed, sizeof(test_data