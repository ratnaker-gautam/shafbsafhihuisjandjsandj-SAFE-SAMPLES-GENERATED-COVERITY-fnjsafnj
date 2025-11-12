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
    uint8_t replacement;
    size_t frequency;
};

struct CompressionContext {
    struct CompressionBlock current_block;
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
    size_t total_compressed;
    size_t total_original;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->dict_size = 0;
    ctx->total_compressed = 0;
    ctx->total_original = 0;
}

uint32_t calculate_checksum(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return 0;
    uint32_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum = (checksum << 5) + checksum + data[i];
    }
    return checksum;
}

int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->size > MAX_BLOCK_SIZE) return 0;
    if (block->size == 0) return 0;
    return 1;
}

int add_dictionary_entry(struct CompressionContext *ctx, const uint8_t *pattern, size_t pattern_size) {
    if (ctx == NULL || pattern == NULL || pattern_size == 0 || pattern_size > 16) return 0;
    if (ctx->dict_size >= MAX_DICT_SIZE) return 0;
    
    for (size_t i = 0; i < ctx->dict_size; i++) {
        if (memcmp(ctx->dictionary[i].pattern, pattern, pattern_size) == 0) {
            ctx->dictionary[i].frequency++;
            return 1;
        }
    }
    
    memcpy(ctx->dictionary[ctx->dict_size].pattern, pattern, pattern_size);
    ctx->dictionary[ctx->dict_size].replacement = (uint8_t)(ctx->dict_size + 128);
    ctx->dictionary[ctx->dict_size].frequency = 1;
    ctx->dict_size++;
    return 1;
}

size_t compress_block(struct CompressionContext *ctx, const struct CompressionBlock *input) {
    if (!validate_block(input)) return 0;
    if (ctx == NULL) return 0;
    
    size_t compressed_size = 0;
    uint8_t output[MAX_BLOCK_SIZE * 2];
    
    for (size_t i = 0; i < input->size; i++) {
        int found = 0;
        for (size_t j = 0; j < ctx->dict_size; j++) {
            if (i + 4 <= input->size && 
                memcmp(&input->data[i], ctx->dictionary[j].pattern, 4) == 0) {
                output[compressed_size++] = ctx->dictionary[j].replacement;
                i += 3;
                found = 1;
                break;
            }
        }
        if (!found) {
            output[compressed_size++] = input->data[i];
        }
        if (compressed_size >= MAX_BLOCK_SIZE * 2) break;
    }
    
    ctx->total_original += input->size;
    ctx->total_compressed += compressed_size;
    return compressed_size;
}

void process_user_input(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Enter text to compress (max %d characters): ", MAX_BLOCK_SIZE - 1);
    
    char input_buffer[MAX_BLOCK_SIZE];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return;
    }
    
    struct CompressionBlock block;
    if (input_len > MAX_BLOCK_SIZE) input_len = MAX_BLOCK_SIZE;
    memcpy(block.data, input_buffer, input_len);
    block.size = input_len;
    block.checksum = calculate_checksum(block.data, block.size);
    
    for (size_t i = 0; i + 4 <= block.size; i++) {
        add_dictionary_entry(ctx, &block.data[i], 4);
    }
    
    size_t compressed_size = compress_block(ctx, &block);
    
    printf("Original size: %zu bytes\n", block.size);
    printf("Compressed size: %zu bytes\n", compressed_size);
    if (block.size > 0) {
        double ratio = (double)compressed_size / block.size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
    
    printf("Dictionary entries: %zu\n", ctx->dict_size);
}

void show_statistics(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    printf("Compression Statistics:\n