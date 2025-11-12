//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t algorithm;
    uint8_t checksum;
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

struct FileInfo {
    char filename[256];
    size_t size;
    uint8_t is_compressed;
};

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535442;
    ctx->header.algorithm = 1;
    ctx->state = 0;
}

int validate_header(const struct CompressionHeader *header) {
    if (header->magic != 0x43535442) return 0;
    if (header->algorithm > 3) return 0;
    if (header->original_size > 100000000) return 0;
    if (header->compressed_size > 100000000) return 0;
    return 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL) return 0;
    if (input_size == 0 || input_size > 256) return 0;
    
    ctx->current_block.block_size = (uint32_t)input_size;
    ctx->current_block.offset = ctx->total_processed;
    memcpy(ctx->current_block.data, input, input_size);
    
    for (size_t i = 0; i < input_size; i++) {
        ctx->current_block.data[i] ^= 0x55;
    }
    
    ctx->total_processed += (uint32_t)input_size;
    return 1;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *data, size_t data_size) {
    if (ctx == NULL || data == NULL) return 0;
    if (data_size == 0 || data_size > 1000000) return 0;
    
    initialize_context(ctx);
    ctx->header.original_size = (uint32_t)data_size;
    
    size_t processed = 0;
    while (processed < data_size) {
        size_t block_size = data_size - processed;
        if (block_size > 256) block_size = 256;
        
        if (!process_block(ctx, data + processed, block_size)) {
            return 0;
        }
        
        processed += block_size;
    }
    
    ctx->header.compressed_size = ctx->total_processed;
    ctx->header.checksum = 0;
    for (uint32_t i = 0; i < ctx->total_processed; i++) {
        ctx->header.checksum ^= ctx->current_block.data[i % 256];
    }
    
    return 1;
}

int decompress_data(const struct CompressionContext *ctx, uint8_t *output, size_t output_size) {
    if (ctx == NULL || output == NULL) return 0;
    if (output_size < ctx->header.original_size) return 0;
    if (!validate_header(&ctx->header)) return 0;
    
    for (uint32_t i = 0; i < ctx->header.original_size; i++) {
        output[i] = ctx->current_block.data[i % 256] ^ 0x55;
    }
    
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    struct FileInfo file_info;
    
    printf("Enter filename: ");
    if (fgets(file_info.filename, sizeof(file_info.filename), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(file_info.filename);
    if (len > 0 && file_info.filename[len-1] == '\n') {
        file_info.filename[len-1] = '\0';
    }
    
    if (strlen(file_info.filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    printf("Enter file size (1-1000 bytes): ");
    if (scanf("%zu", &file_info.size) != 1) {
        printf("Invalid size\n");
        return 1;
    }
    
    if (file_info.size == 0 || file_info.size > 1000) {
        printf("Size out of range\n");
        return 1;
    }
    
    uint8_t *test_data = malloc(file_info.size);
    if (test_data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    for (size_t i = 0; i < file_info.size; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    printf("Compressing data...\n");
    if (!compress_data(&ctx, test_data, file_info.size)) {
        printf("Compression failed\n");
        free(test_data);
        return 1;
    }
    
    printf("Original size: %u bytes\n", ctx.header.original_size);
    printf("Compressed size: %u bytes\n", ctx.header.compressed_size);
    
    if (ctx.header.original_size > 0) {
        float ratio = ((float)ctx.header