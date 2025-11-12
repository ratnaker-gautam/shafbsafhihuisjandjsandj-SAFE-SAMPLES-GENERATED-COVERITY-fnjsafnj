//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
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
    double compression_ratio;
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
    ctx->first_block = NULL;
    ctx->last_block = NULL;
}

int validate_input_data(const uint8_t *data, uint32_t size) {
    if (data == NULL && size > 0) return 0;
    if (size > 1048576) return 0;
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (!validate_input_data(data, size)) return 0;
    
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
    
    if (ctx->last_block == NULL) {
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

uint8_t calculate_checksum(const struct CompressionContext *ctx) {
    uint8_t checksum = 0;
    const struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        for (uint32_t i = 0; i < current->block_size; i++) {
            checksum ^= current->data[i];
        }
        current = current->next;
    }
    return checksum;
}

struct CompressionResult perform_compression(struct CompressionContext *ctx) {
    struct CompressionResult result = {0};
    
    if (ctx->first_block == NULL) {
        return result;
    }
    
    uint32_t compressed_size = 0;
    const struct CompressionBlock *current = ctx->first_block;
    
    while (current != NULL) {
        if (current->block_size > 0) {
            compressed_size += current->block_size - (current->block_size / 10);
        }
        current = current->next;
    }
    
    if (compressed_size > ctx->header.original_size) {
        compressed_size = ctx->header.original_size;
    }
    
    ctx->header.compressed_size = compressed_size;
    ctx->header.checksum = calculate_checksum(ctx);
    
    result.success = 1;
    result.input_size = ctx->header.original_size;
    result.output_size = compressed_size;
    
    if (ctx->header.original_size > 0) {
        result.compression_ratio = (double)compressed_size / ctx->header.original_size;
    } else {
        result.compression_ratio = 1.0;
    }
    
    return result;
}

void display_compression_result(const struct CompressionResult *result) {
    if (result == NULL) return;
    
    printf("Compression completed: %s\n", result->success ? "SUCCESS" : "FAILED");
    if (result->success) {
        printf("Input size: %u bytes\n", result->input_size);
        printf("Output size: %u bytes\n", result->output_size);
        printf("Compression ratio: %.2f\n", result->compression_ratio);
    }
}

int main() {
    struct CompressionContext ctx;
    initialize_compression_context(&ctx);
    
    uint8_t test_data1[] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41};
    uint8_t test_data2[] = {0x42, 0x43, 0x44, 0x45, 0x46};
    uint8_t test_data3[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    
    printf("Adding compression blocks...\n");
    
    if (!add_compression_block(&ctx, test_data