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

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0 || size > 1048576) {
        return 0;
    }
    return 1;
}

int initialize_context(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || !validate_input(input, input_size)) {
        return 0;
    }
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    
    ctx->input_size = input_size;
    ctx->input_data = malloc(input_size);
    if (ctx->input_data == NULL) {
        return 0;
    }
    memcpy(ctx->input_data, input, input_size);
    
    ctx->output_size = input_size + 1024;
    ctx->output_data = malloc(ctx->output_size);
    if (ctx->output_data == NULL) {
        free(ctx->input_data);
        return 0;
    }
    
    ctx->header.magic = 0x43535455;
    ctx->header.original_size = (uint32_t)input_size;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.flags = 0;
    ctx->header.reserved = 0;
    
    ctx->block_count = 1;
    ctx->blocks = malloc(sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        free(ctx->input_data);
        free(ctx->output_data);
        return 0;
    }
    
    memset(ctx->blocks, 0, sizeof(struct CompressionBlock));
    ctx->blocks[0].offset = 0;
    ctx->blocks[0].size = (uint32_t)input_size;
    
    return 1;
}

void compress_data_simple(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->input_data == NULL || ctx->output_data == NULL) {
        return;
    }
    
    size_t output_pos = sizeof(struct CompressionHeader);
    
    for (size_t i = 0; i < ctx->input_size; i++) {
        if (output_pos >= ctx->output_size - 1) {
            break;
        }
        
        if (i + 1 < ctx->input_size && ctx->input_data[i] == ctx->input_data[i + 1]) {
            uint8_t count = 1;
            while (i + count < ctx->input_size && 
                   ctx->input_data[i] == ctx->input_data[i + count] && 
                   count < 255) {
                count++;
            }
            
            if (output_pos + 2 >= ctx->output_size) {
                break;
            }
            
            ctx->output_data[output_pos++] = ctx->input_data[i];
            ctx->output_data[output_pos++] = count;
            i += count - 1;
        } else {
            ctx->output_data[output_pos++] = ctx->input_data[i];
        }
    }
    
    ctx->header.compressed_size = (uint32_t)(output_pos - sizeof(struct CompressionHeader));
    memcpy(ctx->output_data, &ctx->header, sizeof(struct CompressionHeader));
    
    ctx->blocks[0].size = ctx->header.compressed_size;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    if (ctx->input_data != NULL) {
        free(ctx->input_data);
    }
    
    if (ctx->output_data != NULL) {
        free(ctx->output_data);
    }
    
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
    }
}

int main(void) {
    uint8_t test_data[] = "AAAABBBCCCDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPP";
    size_t test_size = sizeof(test_data) - 1;
    
    struct CompressionContext ctx;
    
    if (!initialize_context(&ctx, test_data, test_size)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    compress_data_simple(&ctx);
    
    printf("Original size: %u bytes\n", ctx.header.original_size);
    printf("Compressed size: %u bytes\n", ctx.header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)ctx.header.compressed_size / ctx.header.original_size) * 100);
    
    printf("First 32 bytes of compressed data: ");
    for (int i = 0; i < 32 && i < (int)(ctx.header.compressed_size + sizeof(struct CompressionHeader)); i++) {
        printf("%02X ", ctx.output_data[i]);
    }
    printf("\n");
    
    cleanup_context(&ctx);
    
    return 0;
}