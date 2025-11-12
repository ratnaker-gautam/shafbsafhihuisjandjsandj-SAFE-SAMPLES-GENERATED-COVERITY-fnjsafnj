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

struct CompressionResult {
    int success;
    size_t input_size;
    size_t output_size;
    double compression_ratio;
};

void initialize_compression_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535455;
    ctx->max_block_size = 4096;
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
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
}

int validate_input_data(const uint8_t *data, size_t size) {
    if (data == NULL) {
        return 0;
    }
    if (size == 0 || size > 10485760) {
        return 0;
    }
    return 1;
}

struct CompressionBlock *create_compression_block(const uint8_t *data, size_t size, uint32_t offset) {
    if (data == NULL || size == 0) {
        return NULL;
    }
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) {
        return NULL;
    }
    
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return NULL;
    }
    
    memcpy(block->data, data, size);
    block->size = size;
    block->offset = offset;
    block->next = NULL;
    
    return block;
}

int add_block_to_context(struct CompressionContext *ctx, struct CompressionBlock *block) {
    if (ctx == NULL || block == NULL) {
        return 0;
    }
    
    if (ctx->first_block == NULL) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    ctx->header.compressed_size += block->size;
    
    return 1;
}

struct CompressionResult compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    struct CompressionResult result = {0};
    
    if (!validate_input_data(input, input_size)) {
        return result;
    }
    
    if (ctx == NULL) {
        return result;
    }
    
    initialize_compression_context(ctx);
    ctx->header.original_size = input_size;
    
    size_t remaining = input_size;
    uint32_t offset = 0;
    
    while (remaining > 0) {
        size_t block_size = (remaining > ctx->max_block_size) ? ctx->max_block_size : remaining;
        
        struct CompressionBlock *block = create_compression_block(input + offset, block_size, offset);
        if (block == NULL) {
            cleanup_compression_context(ctx);
            return result;
        }
        
        if (!add_block_to_context(ctx, block)) {
            cleanup_compression_context(ctx);
            return result;
        }
        
        offset += block_size;
        remaining -= block_size;
    }
    
    if (ctx->header.original_size > 0) {
        result.compression_ratio = (double)ctx->header.compressed_size / ctx->header.original_size;
    } else {
        result.compression_ratio = 1.0;
    }
    
    result.success = 1;
    result.input_size = input_size;
    result.output_size = ctx->header.compressed_size;
    
    return result;
}

void print_compression_result(const struct CompressionResult *result) {
    if (result == NULL) {
        printf("Invalid result\n");
        return;
    }
    
    printf("Compression %s\n", result->success ? "successful" : "failed");
    if (result->success) {
        printf("Input size: %zu bytes\n", result->input_size);
        printf("Output size: %zu bytes\n", result->output_size);
        printf("Compression ratio: %.2f\n", result->compression_ratio);
    }
}

int main() {
    struct CompressionContext ctx;
    struct CompressionResult result;
    
    uint8_t test_data[8192];
    for (size_t i = 0; i < sizeof(test_data); i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    result = compress_data(&ctx, test_data, sizeof(test_data));
    print_compression_result(&result);
    
    cleanup_compression_context(&ctx);
    
    uint8_t small_data[100];
    memset(small_data, 'A', sizeof(small_data));
    
    result = compress_data(&ctx, small_data, sizeof(small_data