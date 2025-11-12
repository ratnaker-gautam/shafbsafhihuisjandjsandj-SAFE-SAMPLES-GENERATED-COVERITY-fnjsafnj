//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm_id;
    uint8_t checksum;
};

struct CompressionBlock {
    uint8_t data[256];
    uint16_t size;
    uint32_t offset;
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
    ctx->header.algorithm_id = 1;
    ctx->state = 1;
}

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > UINT32_MAX) return 0;
    return 1;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > 256) return 0;
    if (ctx->state != 1) return 0;

    memcpy(ctx->current_block.data, input, input_size);
    ctx->current_block.size = (uint16_t)input_size;
    ctx->current_block.offset = ctx->total_processed;
    ctx->total_processed += (uint32_t)input_size;

    uint8_t sum = 0;
    for (size_t i = 0; i < input_size; i++) {
        sum ^= input[i];
    }
    ctx->header.checksum ^= sum;

    return 1;
}

int finalize_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    if (ctx->state != 1) return 0;
    if (ctx->total_processed == 0) return 0;

    ctx->header.original_size = ctx->total_processed;
    ctx->header.compressed_size = ctx->total_processed;
    ctx->state = 2;
    return 1;
}

void print_compression_result(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->state != 2) return;

    printf("Compression completed:\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Algorithm ID: %u\n", ctx->header.algorithm_id);
    printf("Checksum: 0x%02X\n", ctx->header.checksum);
    printf("Total blocks processed: %u\n", (ctx->total_processed + 255) / 256);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    uint8_t test_data[] = {
        0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21,
        0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 0x65,
        0x73, 0x74, 0x20, 0x64, 0x61, 0x74, 0x61, 0x20, 0x73, 0x74, 0x72, 0x65,
        0x61, 0x6D, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x63, 0x6F, 0x6D, 0x70, 0x72,
        0x65, 0x73, 0x73, 0x69, 0x6F, 0x6E, 0x2E
    };
    size_t data_size = sizeof(test_data);

    if (!validate_input(test_data, data_size)) {
        printf("Input validation failed\n");
        return 1;
    }

    size_t processed = 0;
    while (processed < data_size) {
        size_t block_size = data_size - processed;
        if (block_size > 256) {
            block_size = 256;
        }

        if (!process_block(&ctx, test_data + processed, block_size)) {
            printf("Block processing failed\n");
            return 1;
        }

        processed += block_size;
    }

    if (!finalize_compression(&ctx)) {
        printf("Finalization failed\n");
        return 1;
    }

    print_compression_result(&ctx);
    return 0;
}