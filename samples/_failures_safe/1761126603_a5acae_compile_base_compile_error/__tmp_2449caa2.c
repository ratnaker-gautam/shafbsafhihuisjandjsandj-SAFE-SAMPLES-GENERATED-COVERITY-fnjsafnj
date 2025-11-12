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

void calculate_checksum(struct CompressionBlock *block) {
    if (block == NULL) return;
    block->checksum = 0;
    for (size_t i = 0; i < block->size; i++) {
        block->checksum += block->data[i];
    }
}

int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->size > MAX_BLOCK_SIZE) return 0;
    return 1;
}

int add_dictionary_entry(struct CompressionContext *ctx, const uint8_t *pattern, size_t length) {
    if (ctx == NULL || pattern == NULL || length == 0 || length > 16) return 0;
    if (ctx->dict_size >= MAX_DICT_SIZE) return 0;
    
    struct DictionaryEntry *entry = &ctx->dictionary[ctx->dict_size];
    memcpy(entry->pattern, pattern, length);
    entry->length = length;
    entry->code = (uint16_t)ctx->dict_size;
    ctx->dict_size++;
    return 1;
}

int compress_block(struct CompressionContext *ctx, const struct CompressionBlock *input, struct CompressionBlock *output) {
    if (!validate_block(input) || output == NULL || ctx == NULL) return 0;
    
    output->size = 0;
    size_t input_pos = 0;
    
    while (input_pos < input->size) {
        int found = 0;
        size_t best_length = 0;
        uint16_t best_code = 0;
        
        for (size_t i = 0; i < ctx->dict_size; i++) {
            struct DictionaryEntry *entry = &ctx->dictionary[i];
            if (input_pos + entry->length <= input->size) {
                if (memcmp(&input->data[input_pos], entry->pattern, entry->length) == 0) {
                    if (entry->length > best_length) {
                        best_length = entry->length;
                        best_code = entry->code;
                        found = 1;
                    }
                }
            }
        }
        
        if (found && output->size + 2 <= MAX_BLOCK_SIZE) {
            output->data[output->size++] = (uint8_t)(best_code >> 8);
            output->data[output->size++] = (uint8_t)(best_code & 0xFF);
            input_pos += best_length;
        } else {
            if (output->size + 1 <= MAX_BLOCK_SIZE) {
                output->data[output->size++] = input->data[input_pos++];
            } else {
                break;
            }
        }
    }
    
    calculate_checksum(output);
    ctx->compressed_size += output->size;
    ctx->total_blocks++;
    return 1;
}

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    
    const char *common_patterns[] = {"the", "and", "ing", "ent", "ion", "her", "for", "tha", "ter", "ere"};
    for (int i = 0; i < 10; i++) {
        add_dictionary_entry(ctx, (const uint8_t*)common_patterns[i], strlen(common_patterns[i]));
    }
}

int main() {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    struct CompressionBlock input_block;
    struct CompressionBlock output_block;
    
    printf("Enter text to compress (max %d characters): ", MAX_BLOCK_SIZE - 1);
    
    char input_buffer[MAX_BLOCK_SIZE];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    memcpy(input_block.data, input_buffer, input_len);
    input_block.size = input_len;
    calculate_checksum(&input_block);
    
    if (!compress_block(&ctx, &input_block, &output_block)) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_block.size);
    printf("Compressed size: %zu bytes\n", output_block.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)output_block.size / input_block.size) * 100);
    printf("Original checksum: %u\n", input_block.checksum);
    printf("Compressed checksum: %u\n", output_block.checksum);
    
    printf("Compressed data (hex):