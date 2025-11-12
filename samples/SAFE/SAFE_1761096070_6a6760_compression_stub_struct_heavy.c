//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t offset;
    uint8_t data[256];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint8_t state;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535442;
    ctx->header.algorithm = 1;
    ctx->state = 1;
}

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->algorithm > 3) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > 104857600) return 0;
    return 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    if (input_size > 256) return 0;
    if (ctx->state != 1) return 0;
    
    ctx->current_block.block_size = (uint32_t)input_size;
    ctx->current_block.offset = ctx->total_processed;
    memcpy(ctx->current_block.data, input, input_size);
    
    for (size_t i = 0; i < input_size; i++) {
        ctx->header.checksum ^= input[i];
    }
    
    ctx->total_processed += (uint32_t)input_size;
    ctx->header.compressed_size += (uint32_t)input_size;
    
    return 1;
}

int finalize_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->state != 1) return 0;
    
    ctx->header.original_size = ctx->total_processed;
    ctx->state = 2;
    return 1;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->state != 2) return;
    
    printf("Compression completed successfully\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (float)ctx->header.compressed_size / ctx->header.original_size * 100.0f);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
}

int main(void) {
    struct CompressionContext ctx;
    uint8_t input_buffer[256];
    size_t bytes_read;
    int block_count = 0;
    
    initialize_context(&ctx);
    
    printf("Enter data to compress (max 256 bytes per line, empty line to finish):\n");
    
    while (block_count < 100) {
        printf("Block %d: ", block_count + 1);
        
        if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        bytes_read = strlen((char*)input_buffer);
        if (bytes_read > 0 && input_buffer[bytes_read - 1] == '\n') {
            input_buffer[--bytes_read] = '\0';
        }
        
        if (bytes_read == 0) {
            break;
        }
        
        if (!process_block(&ctx, input_buffer, bytes_read)) {
            printf("Error processing block\n");
            return 1;
        }
        
        block_count++;
    }
    
    if (!finalize_compression(&ctx)) {
        printf("Error finalizing compression\n");
        return 1;
    }
    
    if (!validate_header(&ctx.header)) {
        printf("Invalid compression header\n");
        return 1;
    }
    
    print_compression_info(&ctx);
    
    return 0;
}