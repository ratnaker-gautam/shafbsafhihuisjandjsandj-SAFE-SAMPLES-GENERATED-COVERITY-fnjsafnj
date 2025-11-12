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
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint8_t state;
};

void init_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
    ctx->state = 1;
}

int validate_input_data(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0 || size > MAX_BLOCK_SIZE) return 0;
    return 1;
}

void build_frequency_table(struct CompressionBlock *block) {
    if (block == NULL) return;
    
    uint32_t freq[256] = {0};
    
    for (size_t i = 0; i < block->size; i++) {
        if (i < MAX_BLOCK_SIZE) {
            freq[block->data[i]]++;
        }
    }
    
    block->dict_size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0 && block->dict_size < MAX_DICT_SIZE) {
            block->dictionary[block->dict_size].symbol = (uint8_t)i;
            block->dictionary[block->dict_size].frequency = freq[i];
            block->dict_size++;
        }
    }
}

void generate_codes(struct CompressionBlock *block) {
    if (block == NULL) return;
    
    for (size_t i = 0; i < block->dict_size; i++) {
        if (i < 16) {
            block->dictionary[i].code_length = 4;
            block->dictionary[i].code = (uint16_t)i;
        } else {
            block->dictionary[i].code_length = 8;
            block->dictionary[i].code = (uint16_t)(0x80 | i);
        }
    }
}

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return 0;
    
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        if (i < MAX_BLOCK_SIZE) {
            checksum ^= data[i];
        }
    }
    return checksum;
}

int compress_block(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (!validate_input_data(ctx->current_block.data, ctx->current_block.size)) return 0;
    
    build_frequency_table(&ctx->current_block);
    generate_codes(&ctx->current_block);
    
    ctx->header.original_size = (uint32_t)ctx->current_block.size;
    ctx->header.compressed_size = (uint32_t)(ctx->current_block.size * 2);
    if (ctx->header.compressed_size > UINT32_MAX - 100) {
        ctx->header.compressed_size = UINT32_MAX - 100;
    }
    
    ctx->header.checksum = calculate_checksum(ctx->current_block.data, ctx->current_block.size);
    ctx->total_processed += ctx->header.original_size;
    
    return 1;
}

void print_compression_results(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression completed:\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (float)ctx->header.compressed_size / ctx->header.original_size * 100.0f);
    printf("Dictionary entries: %zu\n", ctx->current_block.dict_size);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
}

int main(void) {
    struct CompressionContext ctx;
    char input_buffer[MAX_BLOCK_SIZE + 1];
    
    init_compression_context(&ctx);
    
    printf("Enter data to compress (max %d bytes): ", MAX_BLOCK_SIZE);
    
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
    
    if (input_len > MAX_B