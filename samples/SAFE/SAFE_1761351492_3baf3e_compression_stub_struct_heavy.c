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
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t offset;
    uint8_t data[4096];
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
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
    ctx->state = 1;
}

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535455) return 0;
    if (header->algorithm > 3) return 0;
    if (header->original_size > 0xFFFFFFFF) return 0;
    if (header->compressed_size > 0xFFFFFFFF) return 0;
    return 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    if (input_size > sizeof(ctx->current_block.data)) return 0;
    if (ctx->state != 1) return 0;

    ctx->current_block.block_size = (uint32_t)input_size;
    ctx->current_block.offset = ctx->total_processed;
    memcpy(ctx->current_block.data, input, input_size);
    
    uint8_t checksum = 0;
    for (size_t i = 0; i < input_size; i++) {
        checksum ^= input[i];
    }
    ctx->current_block.data[0] = checksum;
    
    ctx->total_processed += (uint32_t)input_size;
    return 1;
}

int finalize_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->state != 1) return 0;
    
    ctx->header.original_size = ctx->total_processed;
    ctx->header.compressed_size = ctx->total_processed;
    
    uint8_t final_checksum = 0;
    for (uint32_t i = 0; i < ctx->total_processed; i++) {
        final_checksum ^= ctx->current_block.data[i % sizeof(ctx->current_block.data)];
    }
    ctx->header.checksum = final_checksum;
    
    ctx->state = 2;
    return 1;
}

void display_results(const struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->state != 2) return;
    
    printf("Compression completed successfully.\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (float)ctx->header.compressed_size / ctx->header.original_size * 100.0f);
    printf("Algorithm: %u\n", ctx->header.algorithm);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    printf("Enter data to compress (max 4096 bytes): ");
    char input_buffer[4097];
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_block(&ctx, (uint8_t*)input_buffer, input_len)) {
        printf("Error processing input block.\n");
        return 1;
    }
    
    if (!finalize_compression(&ctx)) {
        printf("Error finalizing compression.\n");
        return 1;
    }
    
    if (!validate_header(&ctx.header)) {
        printf("Invalid compression header.\n");
        return 1;
    }
    
    display_results(&ctx);
    return 0;
}