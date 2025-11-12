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
    uint32_t input_size;
    uint32_t output_size;
    struct CompressionContext *context;
};

void initialize_compression_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
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

int validate_input_data(const uint8_t *data, uint32_t size) {
    if (data == NULL && size > 0) return 0;
    if (size > 1048576) return 0;
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || !validate_input_data(data, size)) return 0;
    
    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) return 0;
    
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

uint8_t calculate_checksum(const uint8_t *data, uint32_t size) {
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

struct CompressionResult compress_data(const uint8_t *input, uint32_t input_size) {
    struct CompressionResult result = {0};
    
    if (!validate_input_data(input, input_size)) {
        return result;
    }
    
    struct CompressionContext *ctx = malloc(sizeof(struct CompressionContext));
    if (ctx == NULL) {
        return result;
    }
    
    initialize_compression_context(ctx);
    ctx->header.original_size = input_size;
    
    uint32_t processed = 0;
    const uint32_t block_size = 512;
    
    while (processed < input_size) {
        uint32_t current_block_size = input_size - processed;
        if (current_block_size > block_size) {
            current_block_size = block_size;
        }
        
        if (!add_compression_block(ctx, input + processed, current_block_size)) {
            cleanup_compression_context(ctx);
            free(ctx);
            return result;
        }
        
        processed += current_block_size;
    }
    
    ctx->header.checksum = calculate_checksum(input, input_size);
    
    result.success = 1;
    result.input_size = input_size;
    result.output_size = ctx->header.compressed_size;
    result.context = ctx;
    
    return result;
}

void display_compression_info(const struct CompressionResult *result) {
    if (result == NULL || !result->success) {
        printf("Compression failed or invalid result.\n");
        return;
    }
    
    printf("Compression completed successfully.\n");
    printf("Original size: %u bytes\n", result->input_size);
    printf("Compressed size: %u bytes\n", result->output_size);
    printf("Compression ratio: %.2f%%\n", 
           (result->output_size * 100.0) / result->input_size);
    printf("Number of blocks: %u\n", result->context->total_blocks);
}

int main() {
    uint8_t test_data[1024];
    for (int i = 0; i < 1024; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    printf("Starting compression test...\n");
    struct CompressionResult result = compress_data(test_data, 1024);
    
    display_compression_info(&result);
    
    if (result.success) {
        cleanup_compression_context(result.context);
        free(result.context);
    }
    
    return 0;
}