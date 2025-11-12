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
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
}

uint32_t calculate_crc(const uint8_t *data, size_t size) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < size; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc;
}

int build_frequency_dict(struct CompressionBlock *block) {
    if (block->size == 0 || block->size > MAX_BLOCK_SIZE) {
        return 0;
    }

    block->dict_size = 0;
    for (size_t i = 0; i < block->size; i++) {
        uint8_t symbol = block->data[i];
        int found = 0;
        
        for (size_t j = 0; j < block->dict_size; j++) {
            if (block->dictionary[j].symbol == symbol) {
                if (block->dictionary[j].frequency < UINT32_MAX) {
                    block->dictionary[j].frequency++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found && block->dict_size < MAX_DICT_SIZE) {
            block->dictionary[block->dict_size].symbol = symbol;
            block->dictionary[block->dict_size].frequency = 1;
            block->dictionary[block->dict_size].code_length = 0;
            block->dictionary[block->dict_size].code = 0;
            block->dict_size++;
        }
    }
    
    return 1;
}

int compress_block(struct CompressionBlock *block) {
    if (!build_frequency_dict(block)) {
        return 0;
    }
    
    if (block->dict_size == 0) {
        return 0;
    }
    
    for (size_t i = 0; i < block->dict_size - 1; i++) {
        for (size_t j = i + 1; j < block->dict_size; j++) {
            if (block->dictionary[i].frequency < block->dictionary[j].frequency) {
                struct DictionaryEntry temp = block->dictionary[i];
                block->dictionary[i] = block->dictionary[j];
                block->dictionary[j] = temp;
            }
        }
    }
    
    for (size_t i = 0; i < block->dict_size; i++) {
        if (i < 16) {
            block->dictionary[i].code_length = 4;
            block->dictionary[i].code = (uint16_t)i;
        } else {
            block->dictionary[i].code_length = 8;
            block->dictionary[i].code = (uint16_t)(0x80 | i);
        }
    }
    
    block->crc = calculate_crc(block->data, block->size);
    return 1;
}

int process_input(struct CompressionContext *ctx) {
    printf("Enter text to compress (max %d characters): ", MAX_BLOCK_SIZE);
    
    char input_buffer[MAX_BLOCK_SIZE + 2];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return 0;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        return 0;
    }
    
    if (input_len > MAX_BLOCK_SIZE) {
        input_len = MAX_BLOCK_SIZE;
    }
    
    memcpy(ctx->current_block.data, input_buffer, input_len);
    ctx->current_block.size = input_len;
    ctx->total_original = (uint32_t)input_len;
    
    if (!compress_block(&ctx->current_block)) {
        return 0;
    }
    
    ctx->total_blocks = 1;
    ctx->header.original_size = ctx->total_original;
    
    uint32_t dict_overhead = (uint32_t)(ctx->current_block.dict_size * 4);
    uint32