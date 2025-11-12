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
        compressed_size += current->size;
        current = current->next;
    }

    ctx->header.compressed_size = compressed_size;
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
    ctx->first_block = NULL;
    ctx->last_block = NULL;
}

int write_compressed_file(struct CompressionContext *ctx, const char *filename) {
    if (ctx == NULL || filename == NULL || strlen(filename) == 0) {
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

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    uint8_t sample_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20};
    uint8_t sample_data2[] = {0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21};

    if (!add_block(&ctx, sample_data1, sizeof(sample_data1))) {
        cleanup_context(&ctx);
        return 1;
    }

    if (!add_block(&ctx, sample_data2, sizeof(sample_data2))) {
        cleanup_context(&ctx);
        return 1;
    }

    if (!compress_data(&ctx)) {
        cleanup_context(&ctx);
        return 1;
    }

    if (!write_compressed_file(&ctx, "output.cst")) {
        cleanup_context(&ctx);
        return 1;
    }

    printf("Compression completed: %u bytes -> %u bytes\n", 
           ctx.header.original_size, ctx.header.compressed_size);
    printf("Checksum: 0x%02X\n", ctx.header.checksum);

    cleanup_context(&ctx);
    return 0;
}