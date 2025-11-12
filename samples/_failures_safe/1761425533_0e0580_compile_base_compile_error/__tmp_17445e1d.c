//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
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
    if (buf->size > new_capacity) {
        buf->size = new_capacity;
    }
    return 1;
}

int read_input_data(struct InputBuffer *buf) {
    size_t chunk_size = 1024;
    size_t total_read = 0;
    
    if (!resize_input_buffer(buf, chunk_size)) {
        return 0;
    }
    
    while (1) {
        if (total_read >= buf->capacity) {
            if (!resize_input_buffer(buf, buf->capacity * 2)) {
                return 0;
            }
        }
        
        size_t remaining = buf->capacity - total_read;
        if (remaining == 0) {
            break;
        }
        
        size_t bytes_read = fread(buf->data + total_read, 1, remaining, stdin);
        total_read += bytes_read;
        
        if (bytes_read < remaining) {
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

struct CompressionBlock *create_compression_block(const uint8_t *data, uint32_t size) {
    if (data == NULL || size == 0) {
        return NULL;
    }
    
    struct CompressionBlock *block = malloc(sizeof(struct CompressionBlock));
    if (block == NULL) {
        return NULL;
    }
    
    block->data = malloc(size);
    if (block->data == NULL) {
        free(block);
        return NULL;
    }
    
    memcpy(block->data, data, size);
    block->block_size = size;
    block->next = NULL;
    return block;
}

void free_compression_context(struct CompressionContext *ctx) {
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

int compress_data(struct CompressionContext *ctx, const struct InputBuffer *input) {
    if (ctx == NULL || input == NULL || input->data == NULL || input->size == 0) {
        return 0;
    }
    
    if (input->size > UINT32_MAX) {
        return 0;
    }
    
    ctx->header.original_size = (uint32_t)input->size;
    
    const size_t block_size = 512;
    size_t remaining = input->size;
    const uint8_t *current_pos = (const uint8_t*)input->data;
    
    while (remaining > 0) {
        uint32_t current_block_size = (uint32_t)(remaining > block_size ? block_size : remaining);
        
        struct CompressionBlock *block = create_compression_block(current_pos, current_block_size);
        if (block == NULL) {
            return 0;
        }
        
        if (ctx->first_block == NULL) {
            ctx->first_block = block;
            ctx->last_block = block;
        } else {
            ctx->last_block->next = block;
            ctx->last_block = block;
        }
        
        ctx->total_blocks++;
        if (ctx->header.compressed_size > UINT32_MAX - current_block_size) {
            return 0;
        }
        ctx->header.compressed_size += current_block_size;
        current_pos += current_block_size;
        remaining -= current_block_size;
    }
    
    ctx->header.checksum = 0;
    for (size_t i = 0; i < input->size; i++) {
        ctx->header.checksum ^= input->data[i];
    }
    
    return 1;
}

void write_compressed_output(const struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    if (fwrite(&ctx->header, sizeof(ctx->header), 1, stdout) != 1) {
        return;
    }
    
    struct CompressionBlock *current = ctx->first_block;
    while