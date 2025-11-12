//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_BLOCK_SIZE 4096
#define MAX_DICT_SIZE 256

struct CompressionHeader {
    uint32_t magic;
    uint16_t version;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t algorithm;
    uint8_t checksum;
};

struct DictionaryEntry {
    uint8_t symbol;
    uint32_t frequency;
    uint8_t code_length;
    uint16_t code;
};

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    size_t size;
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
    uint32_t crc;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    size_t total_blocks;
    uint32_t total_compressed;
    uint32_t total_original;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
}

uint32_t calculate_crc(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return 0;
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < size; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return ~crc;
}

int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->size > MAX_BLOCK_SIZE) return 0;
    if (block->dict_size > MAX_DICT_SIZE) return 0;
    return 1;
}

int compress_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0 || input_size > MAX_BLOCK_SIZE) {
        return 0;
    }

    struct CompressionBlock *block = &ctx->current_block;
    memset(block, 0, sizeof(*block));
    
    if (input_size <= MAX_BLOCK_SIZE) {
        memcpy(block->data, input, input_size);
    }
    block->size = input_size;
    
    uint8_t freq[256] = {0};
    for (size_t i = 0; i < input_size; i++) {
        freq[input[i]]++;
    }
    
    block->dict_size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0 && block->dict_size < MAX_DICT_SIZE) {
            block->dictionary[block->dict_size].symbol = (uint8_t)i;
            block->dictionary[block->dict_size].frequency = freq[i];
            block->dictionary[block->dict_size].code_length = 8;
            block->dictionary[block->dict_size].code = (uint16_t)i;
            block->dict_size++;
        }
    }
    
    block->crc = calculate_crc(input, input_size);
    
    if (!validate_block(block)) {
        return 0;
    }
    
    if (ctx->total_original + input_size > UINT32_MAX - ctx->total_original) {
        return 0;
    }
    ctx->total_original += (uint32_t)input_size;
    ctx->total_compressed += (uint32_t)input_size;
    ctx->total_blocks++;
    
    return 1;
}

void print_compression_stats(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Statistics:\n");
    printf("Magic: 0x%08X\n", ctx->header.magic);
    printf("Version: %u\n", ctx->header.version);
    printf("Algorithm: %u\n", ctx->header.algorithm);
    printf("Total blocks: %zu\n", ctx->total_blocks);
    printf("Original size: %u bytes\n", ctx->total_original);
    printf("Compressed size: %u bytes\n", ctx->total_compressed);
    
    if (ctx->total_original > 0) {
        double ratio = (double)ctx->total_compressed / ctx->total_original * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    if (ctx->total_blocks > 0 && validate_block(&ctx->current_block)) {
        printf("Last block CRC: 0x%08X\n", ctx->current_block.crc);
        printf("Last block dictionary entries: %zu\n", ctx->current_block.dict_size);
    }
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    uint8_t test_data1[] = "This is a test string for compression demonstration.";
    uint8_t test_data2[] = "Another test block with different content for processing.";
    uint8_t test_data3[] = "Final test data block to complete the compression example.";