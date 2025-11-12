//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t compressed_size;
    uint32_t original_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t offset;
    uint32_t size;
    uint8_t *data;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    uint32_t total_blocks;
    uint32_t max_block_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->compressed_size == 0) return 0;
    if (header->original_size == 0) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

struct CompressionBlock *create_block(uint32_t offset, uint32_t size, const uint8_t *data) {
    if (size == 0 || data == NULL) return NULL;
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) return NULL;
    
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return NULL;
    }
    
    memcpy(block->data, data, size);
    block->offset = offset;
    block->size = size;
    block->next = NULL;
    
    return block;
}

void free_block(struct CompressionBlock *block) {
    if (block == NULL) return;
    free(block->data);
    free(block);
}

void free_context(struct CompressionContext *context) {
    if (context == NULL) return;
    
    struct CompressionBlock *current = context->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        free_block(current);
        current = next;
    }
    
    free(context);
}

struct CompressionContext *create_context(uint32_t max_block_size) {
    if (max_block_size == 0) return NULL;
    
    struct CompressionContext *context = malloc(sizeof(struct CompressionContext));
    if (context == NULL) return NULL;
    
    context->header.magic = 0x43535442;
    context->header.compressed_size = 0;
    context->header.original_size = 0;
    context->header.algorithm = 1;
    context->header.checksum = 0;
    context->header.flags = 0;
    
    context->first_block = NULL;
    context->last_block = NULL;
    context->total_blocks = 0;
    context->max_block_size = max_block_size;
    
    return context;
}

int add_block(struct CompressionContext *context, uint32_t offset, uint32_t size, const uint8_t *data) {
    if (context == NULL || size == 0 || data == NULL) return 0;
    if (size > context->max_block_size) return 0;
    
    struct CompressionBlock *block = create_block(offset, size, data);
    if (block == NULL) return 0;
    
    if (context->first_block == NULL) {
        context->first_block = block;
        context->last_block = block;
    } else {
        context->last_block->next = block;
        context->last_block = block;
    }
    
    context->total_blocks++;
    context->header.compressed_size += size;
    
    uint32_t new_original_size = offset + size;
    if (new_original_size > context->header.original_size) {
        context->header.original_size = new_original_size;
    }
    
    return 1;
}

void simulate_compression(struct CompressionContext *context) {
    if (context == NULL) return;
    
    uint8_t checksum = 0;
    struct CompressionBlock *current = context->first_block;
    
    while (current != NULL) {
        for (uint32_t i = 0; i < current->size; i++) {
            checksum ^= current->data[i];
        }
        current = current->next;
    }
    
    context->header.checksum = checksum;
}

void print_compression_info(const struct CompressionContext *context) {
    if (context == NULL) return;
    
    printf("Compression Information:\n");
    printf("  Magic: 0x%08X\n", context->header.magic);
    printf("  Compressed Size: %u bytes\n", context->header.compressed_size);
    printf("  Original Size: %u bytes\n", context->header.original_size);
    printf("  Algorithm: %u\n", context->header.algorithm);
    printf("  Checksum: 0x%02X\n", context->header.checksum);
    printf("  Flags: 0x%02X\n", context->header.flags);
    printf("  Total Blocks: %u\n", context->total_blocks);
    printf("  Max Block Size: %u bytes\n", context->max_block_size);
    
    if (context->header.original_size > 0) {
        float ratio = (float)context->header.compressed_size / context->header.original_size * 100.0f;
        printf("  Compression Ratio: %.2f%%\n", ratio);
    }
}

int main(void) {