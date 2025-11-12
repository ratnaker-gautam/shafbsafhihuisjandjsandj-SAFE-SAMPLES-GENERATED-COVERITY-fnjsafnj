//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BLOCK_SIZE 4096
#define MAX_DICT_SIZE 256

struct CompressionHeader {
    uint32_t magic;
    uint16_t version;
    uint16_t block_size;
    uint32_t original_size;
    uint32_t compressed_size;
};

struct DictionaryEntry {
    uint8_t pattern[4];
    uint8_t length;
    uint8_t code;
};

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    uint16_t size;
    uint16_t compressed_size;
    struct DictionaryEntry dict[MAX_DICT_SIZE];
    uint8_t dict_size;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint32_t total_compressed;
};

void init_compression_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x434D5052;
    ctx->header.version = 1;
    ctx->header.block_size = MAX_BLOCK_SIZE;
}

int validate_input_data(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > UINT32_MAX) return 0;
    return 1;
}

int build_dictionary(struct CompressionBlock *block) {
    if (block->size < 2) return 0;
    
    block->dict_size = 0;
    
    for (int i = 0; i < block->size - 1 && block->dict_size < MAX_DICT_SIZE; i++) {
        if (block->data[i] == block->data[i + 1]) {
            struct DictionaryEntry *entry = &block->dict[block->dict_size];
            entry->pattern[0] = block->data[i];
            entry->pattern[1] = block->data[i];
            entry->length = 2;
            entry->code = block->dict_size + 128;
            block->dict_size++;
            i++;
        }
    }
    
    return block->dict_size > 0;
}

int compress_block(struct CompressionBlock *block) {
    if (block->size == 0) return 0;
    
    if (!build_dictionary(block)) {
        block->compressed_size = block->size;
        return 1;
    }
    
    uint8_t temp_buffer[MAX_BLOCK_SIZE * 2];
    uint16_t temp_size = 0;
    
    for (uint16_t i = 0; i < block->size; i++) {
        int found = 0;
        
        for (uint8_t j = 0; j < block->dict_size; j++) {
            if (i + 1 < block->size && 
                block->data[i] == block->dict[j].pattern[0] &&
                block->data[i + 1] == block->dict[j].pattern[1]) {
                temp_buffer[temp_size++] = block->dict[j].code;
                i++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            temp_buffer[temp_size++] = block->data[i];
        }
        
        if (temp_size >= MAX_BLOCK_SIZE * 2) {
            return 0;
        }
    }
    
    if (temp_size < block->size) {
        block->compressed_size = temp_size;
        memcpy(block->data, temp_buffer, temp_size);
    } else {
        block->compressed_size = block->size;
    }
    
    return 1;
}

int process_data_chunk(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (!validate_input_data(data, size)) return 0;
    
    size_t remaining = size;
    const uint8_t *current = data;
    
    while (remaining > 0) {
        size_t chunk_size = remaining;
        if (chunk_size > MAX_BLOCK_SIZE) {
            chunk_size = MAX_BLOCK_SIZE;
        }
        
        memcpy(ctx->current_block.data, current, chunk_size);
        ctx->current_block.size = chunk_size;
        
        if (!compress_block(&ctx->current_block)) {
            return 0;
        }
        
        ctx->total_processed += chunk_size;
        ctx->total_compressed += ctx->current_block.compressed_size;
        
        current += chunk_size;
        remaining -= chunk_size;
    }
    
    return 1;
}

void print_compression_stats(const struct CompressionContext *ctx) {
    printf("Compression completed:\n");
    printf("  Original size: %u bytes\n", ctx->total_processed);
    printf("  Compressed size: %u bytes\n", ctx->total_compressed);
    
    if (ctx->total_processed > 0) {
        float ratio = (float)ctx->total_compressed / ctx->total_processed * 100.0f;
        printf("  Compression ratio: %.1f%%\n", ratio);
    }
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);
    
    uint8_t test_data[] = {
        'A', 'A', 'B', 'C', 'D', 'D', 'E', 'F', 'F', 'F', 'G',
        'H', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q'