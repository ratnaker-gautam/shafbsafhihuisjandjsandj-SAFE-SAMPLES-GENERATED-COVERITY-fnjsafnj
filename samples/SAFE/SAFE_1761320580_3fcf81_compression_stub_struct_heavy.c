//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t compressed_size;
    uint32_t original_size;
    uint16_t algorithm;
    uint8_t checksum;
};

struct CompressionBlock {
    uint8_t *data;
    size_t size;
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    size_t total_blocks;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
}

int add_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 1024) {
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
    new_block->size = size;
    new_block->next = NULL;

    if (ctx->first_block == NULL) {
        ctx->first_block = new_block;
        ctx->last_block = new_block;
    } else {
        ctx->last_block->next = new_block;
        ctx->last_block = new_block;
    }

    ctx->total_blocks++;
    ctx->header.original_size += size;
    return 1;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->first_block == NULL) {
        return;
    }

    uint8_t checksum = 0;
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        for (size_t i = 0; i < current->size; i++) {
            checksum ^= current->data[i];
        }
        current = current->next;
    }
    ctx->header.checksum = checksum;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->first_block == NULL) {
        return 0;
    }

    size_t compressed_size = 0;
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        compressed_size += current->size - (current->size / 10);
        current = current->next;
    }

    if (compressed_size > UINT32_MAX) {
        return 0;
    }

    ctx->header.compressed_size = (uint32_t)compressed_size;
    ctx->header.algorithm = 1;
    calculate_checksum(ctx);
    return 1;
}

void free_blocks(struct CompressionBlock *block) {
    while (block != NULL) {
        struct CompressionBlock *next = block->next;
        free(block->data);
        free(block);
        block = next;
    }
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    free_blocks(ctx->first_block);
    memset(ctx, 0, sizeof(struct CompressionContext));
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    uint8_t input_buffer[1024];
    size_t bytes_read;

    printf("Enter data to compress (max 1024 bytes, empty line to finish):\n");
    
    for (int i = 0; i < 10; i++) {
        if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        bytes_read = strlen((char*)input_buffer);
        if (bytes_read == 0 || (bytes_read == 1 && input_buffer[0] == '\n')) {
            break;
        }

        if (bytes_read > 0 && input_buffer[bytes_read - 1] == '\n') {
            input_buffer[bytes_read - 1] = '\0';
            bytes_read--;
        }

        if (bytes_read > 0) {
            if (!add_block(&ctx, input_buffer, bytes_read)) {
                printf("Error: Failed to add block\n");
                cleanup_context(&ctx);
                return 1;
            }
        }
    }

    if (ctx.first_block == NULL) {
        printf("Error: No data provided\n");
        return 1;
    }

    if (!compress_data(&ctx)) {
        printf("Error: Compression failed\n");
        cleanup_context(&ctx);
        return 1;
    }

    printf("Compression completed:\n");
    printf("Original size: %u bytes\n", ctx.header.original_size);
    printf("Compressed size: %u bytes\n", ctx.header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)ctx.header.compressed_size / ctx.header.original_size) * 100);
    printf("Checksum: 0x%02X\n", ctx.header.checksum);
    printf("Blocks processed: %zu\n", ctx.total_blocks);

    cleanup_context(&ctx);
    return 0;
}