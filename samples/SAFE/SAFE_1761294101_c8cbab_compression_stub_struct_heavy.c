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
    ctx->header.compressed_size += size;
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

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) {
        return 0;
    }
    
    ctx->header.original_size = input_size;
    ctx->header.algorithm = 1;
    
    size_t processed = 0;
    while (processed < input_size) {
        size_t block_size = (input_size - processed > 1024) ? 1024 : (input_size - processed);
        
        if (!add_block(ctx, input + processed, block_size)) {
            return 0;
        }
        
        processed += block_size;
    }
    
    calculate_checksum(ctx);
    return 1;
}

void free_context(struct CompressionContext *ctx) {
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

void display_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    printf("Compression Information:\n");
    printf("  Magic: 0x%08X\n", ctx->header.magic);
    printf("  Original Size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed Size: %u bytes\n", ctx->header.compressed_size);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
    printf("  Blocks: %zu\n", ctx->total_blocks);
    
    double ratio = (ctx->header.original_size > 0) ? 
        (double)ctx->header.compressed_size / ctx->header.original_size : 0.0;
    printf("  Compression Ratio: %.2f\n", ratio);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    const char test_data[] = "This is a test string for compression demonstration. "
                           "It contains multiple sentences to provide sufficient data "
                           "for the compression stub to process.";
    
    size_t data_size = strlen(test_data);
    
    printf("Original data size: %zu bytes\n", data_size);
    
    if (compress_data(&ctx, (const uint8_t*)test_data, data_size)) {
        display_compression_info(&ctx);
    } else {
        printf("Compression failed.\n");
    }
    
    free_context(&ctx);
    return 0;
}