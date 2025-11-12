//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
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
    uint8_t replacement;
    size_t frequency;
};

struct CompressionContext {
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
    struct CompressionBlock current_block;
    size_t total_compressed;
    size_t total_original;
};

void calculate_checksum(struct CompressionBlock *block) {
    if (block == NULL) return;
    block->checksum = 0;
    for (size_t i = 0; i < block->size; i++) {
        block->checksum = (block->checksum * 31) + block->data[i];
    }
}

int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->size > MAX_BLOCK_SIZE) return 0;
    return 1;
}

void init_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->dict_size = 0;
    ctx->total_compressed = 0;
    ctx->total_original = 0;
}

int add_to_dictionary(struct CompressionContext *ctx, const uint8_t *pattern, size_t pattern_size, uint8_t replacement) {
    if (ctx == NULL || pattern == NULL || pattern_size == 0 || pattern_size > 16) return 0;
    if (ctx->dict_size >= MAX_DICT_SIZE) return 0;
    
    struct DictionaryEntry *entry = &ctx->dictionary[ctx->dict_size];
    memcpy(entry->pattern, pattern, pattern_size);
    entry->replacement = replacement;
    entry->frequency = 0;
    ctx->dict_size++;
    return 1;
}

size_t compress_block(struct CompressionContext *ctx, const struct CompressionBlock *input, struct CompressionBlock *output) {
    if (ctx == NULL || input == NULL || output == NULL) return 0;
    if (!validate_block(input)) return 0;
    
    memcpy(&output->data, input->data, input->size);
    output->size = input->size;
    
    for (size_t i = 0; i < ctx->dict_size; i++) {
        struct DictionaryEntry *entry = &ctx->dictionary[i];
        size_t pattern_len = strlen((char*)entry->pattern);
        
        for (size_t pos = 0; pos <= output->size - pattern_len; pos++) {
            if (memcmp(&output->data[pos], entry->pattern, pattern_len) == 0) {
                output->data[pos] = entry->replacement;
                memmove(&output->data[pos + 1], &output->data[pos + pattern_len], output->size - pos - pattern_len);
                output->size -= (pattern_len - 1);
                entry->frequency++;
            }
        }
    }
    
    calculate_checksum(output);
    return output->size;
}

void process_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    struct CompressionBlock compressed;
    size_t compressed_size = compress_block(ctx, &ctx->current_block, &compressed);
    
    if (compressed_size > 0) {
        ctx->total_compressed += compressed_size;
        ctx->total_original += ctx->current_block.size;
        
        printf("Original: %zu bytes, Compressed: %zu bytes, Ratio: %.2f\n", 
               ctx->current_block.size, compressed_size,
               (float)compressed_size / ctx->current_block.size);
        printf("Checksum: %u\n", compressed.checksum);
    }
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);
    
    add_to_dictionary(&ctx, (uint8_t*)"the", 3, 0xFE);
    add_to_dictionary(&ctx, (uint8_t*)"and", 3, 0xFD);
    add_to_dictionary(&ctx, (uint8_t*)"ing", 3, 0xFC);
    
    const char *test_data[] = {
        "the quick brown fox jumps over the lazy dog",
        "and then there were none",
        "testing compression algorithm working"
    };
    
    for (int i = 0; i < 3; i++) {
        size_t len = strlen(test_data[i]);
        if (len < MAX_BLOCK_SIZE) {
            memcpy(ctx.current_block.data, test_data[i], len);
            ctx.current_block.size = len;
            calculate_checksum(&ctx.current_block);
            process_compression(&ctx);
        }
    }
    
    printf("Total compression: %zu -> %zu bytes\n", ctx.total_original, ctx.total_compressed);
    
    return 0;
}