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
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535442;
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
    if (block->size < 4) return 0;
    
    block->dict_size = 0;
    
    for (int i = 0; i < block->size - 3 && block->dict_size < MAX_DICT_SIZE; i++) {
        int found = 0;
        for (int j = 0; j < block->dict_size; j++) {
            if (memcmp(block->data + i, block->dict[j].pattern, 4) == 0) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            struct DictionaryEntry *entry = &block->dict[block->dict_size];
            memcpy(entry->pattern, block->data + i, 4);
            entry->length = 4;
            entry->code = block->dict_size;
            block->dict_size++;
        }
    }
    
    return block->dict_size > 0;
}

int compress_block(struct CompressionBlock *block) {
    if (block->size == 0) return 0;
    if (!build_dictionary(block)) return 0;
    
    uint8_t temp_output[MAX_BLOCK_SIZE * 2];
    uint16_t output_pos = 0;
    
    for (int i = 0; i < block->size;) {
        int best_match = -1;
        int best_length = 0;
        
        for (int j = 0; j < block->dict_size; j++) {
            if (i + block->dict[j].length <= block->size) {
                if (memcmp(block->data + i, block->dict[j].pattern, block->dict[j].length) == 0) {
                    if (block->dict[j].length > best_length) {
                        best_length = block->dict[j].length;
                        best_match = j;
                    }
                }
            }
        }
        
        if (best_match != -1 && output_pos + 2 < sizeof(temp_output)) {
            temp_output[output_pos++] = block->dict[best_match].code;
            i += best_length;
        } else if (output_pos + 1 < sizeof(temp_output)) {
            temp_output[output_pos++] = block->data[i++];
        } else {
            break;
        }
    }
    
    if (output_pos < block->size) {
        block->compressed_size = output_pos;
        memcpy(block->data, temp_output, output_pos);
        return 1;
    }
    
    return 0;
}

int process_compression_chunk(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (!validate_input_data(data, size)) return 0;
    
    size_t remaining = size;
    const uint8_t *current_pos = data;
    
    while (remaining > 0) {
        size_t chunk_size = (remaining < MAX_BLOCK_SIZE) ? remaining : MAX_BLOCK_SIZE;
        
        if (chunk_size > sizeof(ctx->current_block.data)) {
            chunk_size = sizeof(ctx->current_block.data);
        }
        
        memcpy(ctx->current_block.data, current_pos, chunk_size);
        ctx->current_block.size = chunk_size;
        
        if (compress_block(&ctx->current_block)) {
            ctx->total_compressed += ctx->current_block.compressed_size;
        } else {
            ctx->total_compressed += chunk_size;
        }
        
        ctx->total_processed += chunk_size;
        current_pos += chunk_size;
        remaining -= chunk_size;
    }
    
    ctx->header.original_size = ctx->total_processed;
    ctx->header.compressed_size = ctx->total_compressed;
    
    return 1;
}

void print_compression_stats(const struct CompressionContext *ctx) {
    printf("Compression completed:\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    
    if (ctx->header.original_size > 0) {
        float ratio = (float)ctx->header.compressed_size / ctx->header.original