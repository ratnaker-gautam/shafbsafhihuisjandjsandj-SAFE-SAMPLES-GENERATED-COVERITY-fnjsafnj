//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BLOCK_SIZE 4096
#define MAX_DICT_SIZE 256

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t dictionary_size;
    uint8_t algorithm_version;
};

struct DictionaryEntry {
    uint8_t original_byte;
    uint8_t compressed_code;
    uint16_t frequency;
};

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    size_t size;
    uint32_t checksum;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    struct CompressionBlock current_block;
    size_t dictionary_count;
    uint32_t total_input_bytes;
    uint32_t total_output_bytes;
};

void initialize_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x434F4D50;
    ctx->header.algorithm_version = 1;
    ctx->dictionary_count = 0;
    ctx->total_input_bytes = 0;
    ctx->total_output_bytes = 0;
    ctx->current_block.size = 0;
    ctx->current_block.checksum = 0;
}

int validate_input_data(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_BLOCK_SIZE) return 0;
    return 1;
}

uint32_t calculate_checksum(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return 0;
    uint32_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum = (checksum << 3) | (checksum >> 29);
        checksum ^= data[i];
    }
    return checksum;
}

int add_to_dictionary(struct CompressionContext *ctx, uint8_t byte) {
    if (ctx == NULL) return 0;
    if (ctx->dictionary_count >= MAX_DICT_SIZE) return 0;
    
    for (size_t i = 0; i < ctx->dictionary_count; i++) {
        if (ctx->dictionary[i].original_byte == byte) {
            if (ctx->dictionary[i].frequency < 65535) {
                ctx->dictionary[i].frequency++;
            }
            return 1;
        }
    }
    
    if (ctx->dictionary_count < MAX_DICT_SIZE) {
        ctx->dictionary[ctx->dictionary_count].original_byte = byte;
        ctx->dictionary[ctx->dictionary_count].compressed_code = (uint8_t)ctx->dictionary_count;
        ctx->dictionary[ctx->dictionary_count].frequency = 1;
        ctx->dictionary_count++;
        return 1;
    }
    
    return 0;
}

int compress_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL) return 0;
    if (!validate_input_data(input, input_size)) return 0;
    
    ctx->current_block.size = 0;
    ctx->current_block.checksum = calculate_checksum(input, input_size);
    
    for (size_t i = 0; i < input_size; i++) {
        if (!add_to_dictionary(ctx, input[i])) return 0;
        
        if (ctx->current_block.size < MAX_BLOCK_SIZE) {
            ctx->current_block.data[ctx->current_block.size] = input[i];
            ctx->current_block.size++;
        } else {
            return 0;
        }
    }
    
    if (ctx->total_input_bytes + input_size < ctx->total_input_bytes) return 0;
    if (ctx->total_output_bytes + input_size < ctx->total_output_bytes) return 0;
    
    ctx->total_input_bytes += (uint32_t)input_size;
    ctx->total_output_bytes += (uint32_t)input_size;
    
    return 1;
}

void update_header(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    ctx->header.original_size = ctx->total_input_bytes;
    ctx->header.compressed_size = ctx->total_output_bytes;
    ctx->header.dictionary_size = (uint16_t)ctx->dictionary_count;
}

void print_compression_stats(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    printf("Compression Statistics:\n");
    printf("  Original size: %u bytes\n", ctx->header.original_size);
    printf("  Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("  Dictionary entries: %u\n", ctx->header.dictionary_size);
    
    double ratio = 0.0;
    if (ctx->header.original_size > 0) {
        ratio = 100.0 * ctx->header.compressed_size / ctx->header.original_size;
    }
    printf("  Compression ratio: %.2f%%\n", ratio);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_compression_context(&ctx);
    
    uint8_t test_data[] = {0x41, 0x42, 0x43, 0x44, 0x41, 0x42, 0x43