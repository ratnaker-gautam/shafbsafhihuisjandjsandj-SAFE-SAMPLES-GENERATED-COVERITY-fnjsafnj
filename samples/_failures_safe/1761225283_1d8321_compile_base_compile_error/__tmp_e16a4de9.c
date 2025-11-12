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
    uint8_t flags;
    uint8_t reserved;
};

struct CompressionBlock {
    uint32_t offset;
    uint32_t size;
    uint8_t checksum[16];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint8_t *input_data;
    uint8_t *output_data;
    size_t input_size;
    size_t output_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->original_size == 0) return 0;
    if (header->algorithm > 3) return 0;
    return 1;
}

int initialize_context(struct CompressionContext *ctx, size_t input_size) {
    if (ctx == NULL || input_size == 0 || input_size > 1048576) return 0;
    
    ctx->input_size = input_size;
    ctx->output_size = input_size * 2;
    
    ctx->input_data = malloc(ctx->input_size);
    ctx->output_data = malloc(ctx->output_size);
    if (ctx->input_data == NULL || ctx->output_data == NULL) {
        free(ctx->input_data);
        free(ctx->output_data);
        return 0;
    }
    
    ctx->block_count = (input_size + 4095) / 4096;
    if (ctx->block_count > 256) ctx->block_count = 256;
    
    ctx->blocks = malloc(ctx->block_count * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        free(ctx->input_data);
        free(ctx->output_data);
        return 0;
    }
    
    memset(ctx->blocks, 0, ctx->block_count * sizeof(struct CompressionBlock));
    
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = (uint32_t)input_size;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.flags = 0;
    ctx->header.reserved = 0;
    
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    free(ctx->input_data);
    free(ctx->output_data);
    free(ctx->blocks);
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->input_data == NULL || ctx->output_data == NULL) return 0;
    
    size_t output_pos = 0;
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        size_t block_start = i * 4096;
        size_t block_size = 4096;
        if (block_start + block_size > ctx->input_size) {
            block_size = ctx->input_size - block_start;
        }
        
        if (output_pos + block_size + 1 > ctx->output_size) return 0;
        
        ctx->output_data[output_pos] = 0;
        memcpy(ctx->output_data + output_pos + 1, ctx->input_data + block_start, block_size);
        
        ctx->blocks[i].offset = (uint32_t)output_pos;
        ctx->blocks[i].size = (uint32_t)(block_size + 1);
        
        for (size_t j = 0; j < 16 && j < block_size; j++) {
            ctx->blocks[i].checksum[j] = ctx->input_data[block_start + j];
        }
        
        output_pos += block_size + 1;
    }
    
    ctx->header.compressed_size = (uint32_t)output_pos;
    return 1;
}

int decompress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->input_data == NULL || ctx->output_data == NULL) return 0;
    
    size_t output_pos = 0;
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].offset + ctx->blocks[i].size > ctx->header.compressed_size) return 0;
        
        uint8_t flag = ctx->input_data[ctx->blocks[i].offset];
        if (flag != 0) return 0;
        
        size_t data_size = ctx->blocks[i].size - 1;
        if (output_pos + data_size > ctx->output_size) return 0;
        
        memcpy(ctx->output_data + output_pos, 
               ctx->input_data + ctx->blocks[i].offset + 1, 
               data_size);
        
        output_pos += data_size;
    }
    
    if (output_pos != ctx->header.original_size) return 0;
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    const char test_data[] = "This is a test string for compression demonstration.";
    size_t data_size = strlen(test_data) + 1;
    
    if (!initialize_context(&ctx, data_size)) {
        printf("Failed to initialize compression context\n");
        return 1;
    }
    
    memcpy(ctx.input_data, test_data, data_size);
    
    printf("Original data: %s\n", (char*)ctx