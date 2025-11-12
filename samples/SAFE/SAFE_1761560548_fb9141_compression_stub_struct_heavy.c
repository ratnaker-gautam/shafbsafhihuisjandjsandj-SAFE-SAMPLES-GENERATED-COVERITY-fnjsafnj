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
    uint32_t block_size;
    uint32_t checksum;
    uint8_t data[1024];
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint32_t current_offset;
};

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    ctx->header.algorithm = 1;
    ctx->current_block.block_size = 0;
}

int validate_header(const struct CompressionHeader *header) {
    if (header->magic != 0x43535455) return 0;
    if (header->algorithm > 3) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > 104857600) return 0;
    return 1;
}

uint32_t simple_checksum(const uint8_t *data, size_t len) {
    uint32_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum = (sum + data[i]) & 0xFFFFFFFF;
    }
    return sum;
}

int compress_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (input_size > sizeof(ctx->current_block.data)) return 0;
    if (ctx->current_offset + input_size > ctx->header.original_size) return 0;
    
    memcpy(ctx->current_block.data, input, input_size);
    ctx->current_block.block_size = input_size;
    ctx->current_block.checksum = simple_checksum(input, input_size);
    ctx->total_processed += input_size;
    ctx->current_offset += input_size;
    
    return 1;
}

int decompress_block(struct CompressionContext *ctx, uint8_t *output, size_t output_size) {
    if (ctx->current_block.block_size == 0) return 0;
    if (output_size < ctx->current_block.block_size) return 0;
    
    uint32_t calculated_checksum = simple_checksum(ctx->current_block.data, ctx->current_block.block_size);
    if (calculated_checksum != ctx->current_block.checksum) return 0;
    
    memcpy(output, ctx->current_block.data, ctx->current_block.block_size);
    return ctx->current_block.block_size;
}

void process_compression(struct CompressionContext *ctx) {
    uint8_t test_data[] = "This is a test string for compression demonstration.";
    size_t data_size = sizeof(test_data) - 1;
    
    ctx->header.original_size = data_size;
    ctx->header.compressed_size = data_size;
    
    if (!compress_block(ctx, test_data, data_size)) {
        printf("Compression failed\n");
        return;
    }
    
    uint8_t output[1024];
    int decompressed_size = decompress_block(ctx, output, sizeof(output));
    
    if (decompressed_size > 0) {
        printf("Original: %s\n", test_data);
        printf("Decompressed: ");
        for (int i = 0; i < decompressed_size; i++) {
            printf("%c", output[i]);
        }
        printf("\n");
        printf("Original size: %u, Processed: %u\n", ctx->header.original_size, ctx->total_processed);
    } else {
        printf("Decompression failed\n");
    }
}

int main() {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    if (!validate_header(&ctx.header)) {
        printf("Invalid compression header\n");
        return 1;
    }
    
    process_compression(&ctx);
    return 0;
}