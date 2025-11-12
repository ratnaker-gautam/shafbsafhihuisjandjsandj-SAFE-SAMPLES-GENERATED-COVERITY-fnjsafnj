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
    uint8_t *data;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    uint32_t total_blocks;
    uint32_t max_block_size;
};

struct CompressionStats {
    uint32_t input_bytes;
    uint32_t output_bytes;
    uint32_t blocks_processed;
    double compression_ratio;
};

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535455;
    ctx->max_block_size = 4096;
}

void cleanup_context(struct CompressionContext *ctx) {
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        if (current->data != NULL) {
            free(current->data);
        }
        free(current);
        current = next;
    }
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > ctx->max_block_size) {
        return 0;
    }

    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) {
        return 0;
    }

    new_block->data = malloc(size);
    if (new_block->data == NULL) {
        free(new_block);
        return 0;
    }

    memcpy(new_block->data, data, size);
    new_block->block_size = size;
    new_block->offset = ctx->header.original_size;
    new_block->next = NULL;

    if (ctx->last_block == NULL) {
        ctx->first_block = new_block;
        ctx->last_block = new_block;
    } else {
        ctx->last_block->next = new_block;
        ctx->last_block = new_block;
    }

    ctx->header.original_size += size;
    ctx->total_blocks++;
    return 1;
}

void calculate_stats(const struct CompressionContext *ctx, struct CompressionStats *stats) {
    if (ctx == NULL || stats == NULL) {
        return;
    }

    stats->input_bytes = ctx->header.original_size;
    stats->output_bytes = ctx->header.compressed_size;
    stats->blocks_processed = ctx->total_blocks;

    if (stats->input_bytes > 0) {
        stats->compression_ratio = (double)stats->output_bytes / stats->input_bytes;
    } else {
        stats->compression_ratio = 0.0;
    }
}

int simulate_compression(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->total_blocks == 0) {
        return 0;
    }

    uint32_t total_compressed = 0;
    struct CompressionBlock *current = ctx->first_block;

    while (current != NULL) {
        uint32_t compressed_size = current->block_size - (current->block_size / 4);
        if (compressed_size < 1) {
            compressed_size = 1;
        }
        total_compressed += compressed_size;
        current = current->next;
    }

    ctx->header.compressed_size = total_compressed;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0xAB;
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    struct CompressionStats stats;
    uint8_t test_data[3][100];
    size_t test_sizes[] = {50, 75, 100};

    initialize_context(&ctx);

    for (int i = 0; i < 3; i++) {
        for (size_t j = 0; j < test_sizes[i]; j++) {
            test_data[i][j] = (uint8_t)(i + j);
        }
        if (!add_compression_block(&ctx, test_data[i], test_sizes[i])) {
            printf("Failed to add block %d\n", i);
            cleanup_context(&ctx);
            return 1;
        }
    }

    if (!simulate_compression(&ctx)) {
        printf("Compression simulation failed\n");
        cleanup_context(&ctx);
        return 1;
    }

    calculate_stats(&ctx, &stats);

    printf("Compression completed:\n");
    printf("Input bytes: %u\n", stats.input_bytes);
    printf("Output bytes: %u\n", stats.output_bytes);
    printf("Blocks processed: %u\n", stats.blocks_processed);
    printf("Compression ratio: %.2f\n", stats.compression_ratio);
    printf("Header magic: 0x%08X\n", ctx.header.magic);

    cleanup_context(&ctx);
    return 0;
}