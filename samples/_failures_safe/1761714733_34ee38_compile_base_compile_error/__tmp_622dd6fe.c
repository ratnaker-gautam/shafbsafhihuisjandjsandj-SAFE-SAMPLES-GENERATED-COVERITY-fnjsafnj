//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BLOCK_SIZE 1024
#define MAX_DICT_SIZE 256

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    size_t size;
    uint32_t checksum;
};

struct DictionaryEntry {
    uint8_t pattern[16];
    size_t length;
    uint16_t code;
};

struct CompressionContext {
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
    uint32_t total_blocks;
    uint32_t compressed_size;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    ctx->dict_size = 0;
    ctx->total_blocks = 0;
    ctx->compressed_size = 0;
    
    for (size_t i = 0; i < MAX_DICT_SIZE; i++) {
        ctx->dictionary[i].length = 0;
        ctx->dictionary[i].code = 0;
        memset(ctx->dictionary[i].pattern, 0, 16);
    }
}

uint32_t calculate_checksum(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return 0;
    
    uint32_t sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum = (sum + data[i]) % 0xFFFFFFFF;
    }
    return sum;
}

int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->size == 0 || block->size > MAX_BLOCK_SIZE) return 0;
    return 1;
}

int add_to_dictionary(struct CompressionContext *ctx, const uint8_t *pattern, size_t length) {
    if (ctx == NULL || pattern == NULL || length == 0 || length > 16) return 0;
    if (ctx->dict_size >= MAX_DICT_SIZE) return 0;
    
    for (size_t i = 0; i < ctx->dict_size; i++) {
        if (ctx->dictionary[i].length == length && 
            memcmp(ctx->dictionary[i].pattern, pattern, length) == 0) {
            return 1;
        }
    }
    
    struct DictionaryEntry *entry = &ctx->dictionary[ctx->dict_size];
    memcpy(entry->pattern, pattern, length);
    entry->length = length;
    entry->code = (uint16_t)ctx->dict_size;
    ctx->dict_size++;
    
    return 1;
}

int compress_block(struct CompressionContext *ctx, const struct CompressionBlock *block) {
    if (!validate_block(block)) return 0;
    if (ctx == NULL) return 0;
    
    uint32_t calculated_checksum = calculate_checksum(block->data, block->size);
    if (calculated_checksum != block->checksum) return 0;
    
    for (size_t i = 0; i < block->size; i += 4) {
        size_t remaining = block->size - i;
        size_t chunk_size = (remaining < 4) ? remaining : 4;
        
        if (!add_to_dictionary(ctx, &block->data[i], chunk_size)) {
            return 0;
        }
    }
    
    ctx->total_blocks++;
    ctx->compressed_size += block->size / 2;
    
    return 1;
}

void print_compression_stats(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Statistics:\n");
    printf("Total blocks processed: %u\n", ctx->total_blocks);
    printf("Dictionary entries: %zu\n", ctx->dict_size);
    printf("Compressed size: %u bytes\n", ctx->compressed_size);
    
    if (ctx->dict_size > 0) {
        printf("Dictionary contents:\n");
        for (size_t i = 0; i < ctx->dict_size && i < 5; i++) {
            printf("  Code %hu: ", ctx->dictionary[i].code);
            for (size_t j = 0; j < ctx->dictionary[i].length; j++) {
                printf("%02X ", ctx->dictionary[i].pattern[j]);
            }
            printf("\n");
        }
        if (ctx->dict_size > 5) {
            printf("  ... and %zu more entries\n", ctx->dict_size - 5);
        }
    }
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    struct CompressionBlock blocks[3];
    
    uint8_t sample1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
    if (sizeof(sample1) <= MAX_BLOCK_SIZE) {
        blocks[0].size = sizeof(sample1);
        memcpy(blocks[0].data, sample1, blocks[0].size);
        blocks[0].checksum = calculate_checksum(sample1, blocks[0].size);
    } else {
        blocks[0].size = 0;
        blocks[0].checksum = 0;
    }
    
    uint8_t sample2[] = {0x54, 0x65, 0x