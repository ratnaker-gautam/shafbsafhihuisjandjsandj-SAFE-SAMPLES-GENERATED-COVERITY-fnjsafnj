//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
};

struct CompressionBlock {
    uint32_t block_size;
    uint8_t *data;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    uint32_t total_blocks;
};

struct InputBuffer {
    char *data;
    size_t size;
    size_t capacity;
};

void init_input_buffer(struct InputBuffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int resize_input_buffer(struct InputBuffer *buf, size_t new_capacity) {
    if (new_capacity == 0) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
        return 1;
    }
    
    char *new_data = realloc(buf->data, new_capacity);
    if (new_data == NULL) {
        return 0;
    }
    
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

int read_input_data(struct InputBuffer *buf) {
    const size_t chunk_size = 1024;
    size_t total_read = 0;
    
    while (1) {
        if (total_read + chunk_size > buf->capacity) {
            if (!resize_input_buffer(buf, buf->capacity + chunk_size * 2)) {
                return 0;
            }
        }
        
        size_t bytes_read = fread(buf->data + total_read, 1, chunk_size, stdin);
        total_read += bytes_read;
        
        if (bytes_read < chunk_size) {
            if (ferror(stdin)) {
                return 0;
            }
            break;
        }
    }
    
    buf->size = total_read;
    return 1;
}

void init_compression_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    ctx->first_block = NULL;
    ctx->last_block = NULL;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (size == 0 || data == NULL) {
        return 0;
    }
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) {
        return 0;
    }
    
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return 0;
    }
    
    memcpy(block->data, data, size);
    block->block_size = size;
    block->next = NULL;
    
    if (ctx->last_block == NULL) {
        ctx->first_block = block;
        ctx->last_block = block;
    } else {
        ctx->last_block->next = block;
        ctx->last_block = block;
    }
    
    ctx->total_blocks++;
    ctx->header.compressed_size += size;
    return 1;
}

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int perform_compression(struct CompressionContext *ctx, const struct InputBuffer *input) {
    if (input->size == 0 || input->data == NULL) {
        return 0;
    }
    
    if (input->size > UINT32_MAX) {
        return 0;
    }
    
    ctx->header.original_size = (uint32_t)input->size;
    
    const size_t block_size = 512;
    size_t remaining = input->size;
    const uint8_t *current_pos = (const uint8_t *)input->data;
    
    while (remaining > 0) {
        size_t current_block_size = (remaining > block_size) ? block_size : remaining;
        
        if (current_block_size > UINT32_MAX) {
            return 0;
        }
        
        if (!add_compression_block(ctx, current_pos, (uint32_t)current_block_size)) {
            return 0;
        }
        
        current_pos += current_block_size;
        remaining -= current_block_size;
    }
    
    ctx->header.checksum = calculate_checksum((const uint8_t *)input->data, input->size);
    return 1;
}

int write_compressed_output(const struct CompressionContext *ctx) {
    if (fwrite(&ctx->header, sizeof(ctx->header), 1, stdout) != 1) {
        return 0;
    }
    
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        if (fwrite(&current->block_size, sizeof(current->block_size), 1, stdout) != 1