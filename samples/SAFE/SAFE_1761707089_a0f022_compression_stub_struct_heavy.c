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
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) {
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
    ctx->header.compressed_size += size;
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

int write_compressed_data(struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL || ctx->first_block == NULL) {
        return 0;
    }

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }

    if (fwrite(&ctx->header, sizeof(struct CompressionHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        if (fwrite(current->data, 1, current->size, file) != current->size) {
            fclose(file);
            return 0;
        }
        current = current->next;
    }

    fclose(file);
    return 1;
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }

    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    ctx->first_block = NULL;
    ctx->last_block = NULL;
    ctx->total_blocks = 0;
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    printf("Enter data to compress (max 1024 characters): ");
    char input[1025];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }

    ctx.header.original_size = input_len;
    ctx.header.algorithm = 1;

    const size_t block_size = 64;
    for (size_t i = 0; i < input_len; i += block_size) {
        size_t chunk_size = (i + block_size <= input_len) ? block_size : input_len - i;
        if (!add_block(&ctx, (uint8_t*)input + i, chunk_size)) {
            printf("Error adding compression block\n");
            cleanup_context(&ctx);
            return 1;
        }
    }

    calculate_checksum(&ctx);

    if (!write_compressed_data(&ctx, "compressed.dat")) {
        printf("Error writing compressed data\n");
        cleanup_context(&ctx);
        return 1;
    }

    printf("Compression completed:\n");
    printf("Original size: %u bytes\n", ctx.header.original_size);
    printf("Compressed size: %u bytes\n", ctx.header.compressed_size);
    printf("Blocks: %zu\n", ctx.total_blocks);
    printf("Checksum: 0x%02X\n", ctx.header.checksum);

    cleanup_context(&ctx);
    return 0;
}