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
    size_t pattern_size;
    uint16_t code;
};

struct CompressionContext {
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
    uint32_t total_blocks;
    uint32_t compressed_size;
};

struct CompressionResult {
    struct CompressionContext context;
    uint8_t output_buffer[MAX_BLOCK_SIZE * 2];
    size_t output_size;
    int success_flag;
};

static uint32_t calculate_checksum(const uint8_t* data, size_t size) {
    uint32_t sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum = (sum + data[i]) & 0xFFFFFFFF;
    }
    return sum;
}

static int validate_block(const struct CompressionBlock* block) {
    if (block == NULL) return 0;
    if (block->size == 0 || block->size > MAX_BLOCK_SIZE) return 0;
    uint32_t computed = calculate_checksum(block->data, block->size);
    return computed == block->checksum;
}

static void initialize_context(struct CompressionContext* ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(*ctx));
    ctx->dict_size = 0;
    ctx->total_blocks = 0;
    ctx->compressed_size = 0;
}

static int add_to_dictionary(struct CompressionContext* ctx, const uint8_t* pattern, size_t pattern_size) {
    if (ctx == NULL || pattern == NULL || pattern_size == 0 || pattern_size > 16) return 0;
    if (ctx->dict_size >= MAX_DICT_SIZE) return 0;
    
    for (size_t i = 0; i < ctx->dict_size; i++) {
        if (ctx->dictionary[i].pattern_size == pattern_size && 
            memcmp(ctx->dictionary[i].pattern, pattern, pattern_size) == 0) {
            return 1;
        }
    }
    
    struct DictionaryEntry* entry = &ctx->dictionary[ctx->dict_size];
    memcpy(entry->pattern, pattern, pattern_size);
    entry->pattern_size = pattern_size;
    entry->code = (uint16_t)ctx->dict_size;
    ctx->dict_size++;
    return 1;
}

static int compress_block(const struct CompressionBlock* block, struct CompressionContext* ctx, struct CompressionResult* result) {
    if (!validate_block(block) || ctx == NULL || result == NULL) return 0;
    
    result->success_flag = 0;
    result->output_size = 0;
    
    if (block->size < 4) {
        if (block->size > 0 && result->output_size < sizeof(result->output_buffer)) {
            result->output_buffer[result->output_size++] = 0x00;
            if (result->output_size + block->size <= sizeof(result->output_buffer)) {
                memcpy(&result->output_buffer[result->output_size], block->data, block->size);
                result->output_size += block->size;
                result->success_flag = 1;
            }
        }
        return result->success_flag;
    }
    
    for (size_t i = 0; i < block->size - 3; i += 2) {
        if (ctx->dict_size < MAX_DICT_SIZE) {
            add_to_dictionary(ctx, &block->data[i], 2);
        }
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < block->size; ) {
        if (output_idx >= sizeof(result->output_buffer) - 2) break;
        
        int found = 0;
        for (size_t j = 0; j < ctx->dict_size; j++) {
            if (i + ctx->dictionary[j].pattern_size <= block->size &&
                memcmp(&block->data[i], ctx->dictionary[j].pattern, ctx->dictionary[j].pattern_size) == 0) {
                if (output_idx + 2 <= sizeof(result->output_buffer)) {
                    result->output_buffer[output_idx++] = 0x80 | (ctx->dictionary[j].code >> 8);
                    result->output_buffer[output_idx++] = ctx->dictionary[j].code & 0xFF;
                    i += ctx->dictionary[j].pattern_size;
                    found = 1;
                    break;
                }
            }
        }
        
        if (!found && output_idx < sizeof(result->output_buffer)) {
            result->output_buffer[output_idx++] = block->data[i++];
        }
    }
    
    result->output_size = output_idx;
    result->success_flag = 1;
    ctx->total_blocks++;
    ctx->compressed_size += result->output_size;
    
    return result->success_flag;
}

static void process_user_input(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    printf("Enter up to %d bytes of data: ", MAX_BLOCK_SIZE);
    
    uint8_t input_buffer[MAX_BLOCK_SIZE];
    size_t input_size = 0;
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size