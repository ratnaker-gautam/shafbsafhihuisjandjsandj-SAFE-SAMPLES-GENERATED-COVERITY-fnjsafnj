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

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

int validate_header(const struct CompressionHeader *header) {
    if (header->magic != 0x43535455) return 0;
    if (header->original_size > 100000000) return 0;
    if (header->compressed_size > 100000000) return 0;
    if (header->algorithm > 5) return 0;
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) return 0;
    
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

void free_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
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

int compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0 || input_size > 1000000) return 0;
    
    initialize_context(ctx);
    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;
    
    uint32_t processed = 0;
    while (processed < input_size) {
        uint32_t block_size = (input_size - processed > 1024) ? 1024 : (input_size - processed);
        
        uint8_t compressed_block[1024];
        uint32_t compressed_size = 0;
        
        for (uint32_t i = 0; i < block_size && compressed_size < sizeof(compressed_block); i++) {
            if (i > 0 && input[processed + i] == input[processed + i - 1]) {
                continue;
            }
            compressed_block[compressed_size++] = input[processed + i];
        }
        
        if (!add_compression_block(ctx, compressed_block, compressed_size)) {
            free_compression_context(ctx);
            return 0;
        }
        
        processed += block_size;
    }
    
    ctx->header.checksum = 0;
    for (uint32_t i = 0; i < input_size && i < 256; i++) {
        ctx->header.checksum ^= input[i];
    }
    
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, uint32_t output_size) {
    if (ctx == NULL || output == NULL || output_size < ctx->header.original_size) return 0;
    if (!validate_header(&ctx->header)) return 0;
    
    uint32_t decompressed = 0;
    struct CompressionBlock *current = ctx->first_block;
    
    while (current != NULL && decompressed < output_size) {
        if (current->block_size == 0 || current->data == NULL) {
            return 0;
        }
        
        for (uint32_t i = 0; i < current->block_size && decompressed < output_size; i++) {
            output[decompressed++] = current->data[i];
        }
        
        current = current->next;
    }
    
    if (decompressed != ctx->header.original_size) return 0;
    
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < decompressed && i < 256; i++) {
        checksum ^= output[i];
    }
    
    return checksum == ctx->header.checksum;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Information:\n");
    printf("  Original size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed size