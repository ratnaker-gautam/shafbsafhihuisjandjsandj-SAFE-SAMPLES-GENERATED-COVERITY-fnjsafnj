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
    uint8_t *data;
    size_t size;
    size_t capacity;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock input;
    struct CompressionBlock output;
    int compression_level;
};

void init_compression_block(struct CompressionBlock *block, size_t initial_capacity) {
    block->data = malloc(initial_capacity);
    if (block->data) {
        block->capacity = initial_capacity;
        block->size = 0;
    } else {
        block->capacity = 0;
        block->size = 0;
    }
}

void free_compression_block(struct CompressionBlock *block) {
    if (block->data) {
        free(block->data);
        block->data = NULL;
    }
    block->size = 0;
    block->capacity = 0;
}

int resize_compression_block(struct CompressionBlock *block, size_t new_capacity) {
    if (new_capacity == 0) {
        free_compression_block(block);
        return 1;
    }
    uint8_t *new_data = realloc(block->data, new_capacity);
    if (!new_data) {
        return 0;
    }
    block->data = new_data;
    block->capacity = new_capacity;
    if (block->size > new_capacity) {
        block->size = new_capacity;
    }
    return 1;
}

int append_to_compression_block(struct CompressionBlock *block, const uint8_t *data, size_t data_size) {
    if (!block || !data || data_size == 0) {
        return 0;
    }
    if (block->size + data_size > block->capacity) {
        size_t new_capacity = block->capacity * 2;
        if (new_capacity < block->size + data_size) {
            new_capacity = block->size + data_size;
        }
        if (!resize_compression_block(block, new_capacity)) {
            return 0;
        }
    }
    memcpy(block->data + block->size, data, data_size);
    block->size += data_size;
    return 1;
}

uint8_t calculate_checksum(const uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx) {
    if (!ctx || ctx->input.size == 0) {
        return 0;
    }
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = (uint32_t)ctx->input.size;
    if (ctx->header.original_size != ctx->input.size) {
        return 0;
    }
    size_t output_needed = ctx->input.size + sizeof(struct CompressionHeader);
    if (output_needed < ctx->input.size) {
        return 0;
    }
    if (!resize_compression_block(&ctx->output, output_needed)) {
        return 0;
    }
    for (size_t i = 0; i < ctx->input.size; i++) {
        if (i % 2 == 0) {
            ctx->output.data[i] = ctx->input.data[i] ^ 0xAA;
        } else {
            ctx->output.data[i] = ctx->input.data[i];
        }
    }
    ctx->output.size = ctx->input.size;
    ctx->header.compressed_size = (uint32_t)ctx->output.size;
    if (ctx->header.compressed_size != ctx->output.size) {
        return 0;
    }
    ctx->header.algorithm = 1;
    ctx->header.checksum = calculate_checksum(ctx->output.data, ctx->output.size);
    return 1;
}

int save_compressed_data(const struct CompressionContext *ctx, const char *filename) {
    if (!ctx || !filename) {
        return 0;
    }
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return 0;
    }
    if (fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    if (fwrite(ctx->output.data, 1, ctx->output.size, file) != ctx->output.size) {
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.compression_level = 1;
    
    init_compression_block(&ctx.input, 1024);
    init_compression_block(&ctx.output, 1024);
    
    if (!ctx.input.data || !ctx.output.data) {
        fprintf(stderr, "Memory allocation failed\n");
        free_compression_block(&ctx.input);
        free_compression_block(&ctx.output);
        return 1;
    }
    
    printf("Enter data to compress (max 1024 bytes): ");
    char buffer[1025];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        fprintf