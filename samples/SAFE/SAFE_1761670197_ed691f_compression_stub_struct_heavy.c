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
    memset(ctx, 0, sizeof(struct CompressionContext));
}

int main(void) {
    struct CompressionContext ctx;
    initialize_context(&ctx);

    printf("Enter up to 5 lines of text (max 100 chars each):\n");
    
    char buffer[101];
    for (int i = 0; i < 5; i++) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
            len--;
        }
        
        if (len > 0) {
            if (!add_block(&ctx, (uint8_t*)buffer, len)) {
                printf("Error: Failed to add block\n");
                cleanup_context(&ctx);
                return 1;
            }
            ctx.header.original_size += len;
        }
    }

    if (ctx.total_blocks == 0) {
        printf("Error: No data to compress\n");
        cleanup_context(&ctx);
        return 1;
    }

    ctx.header.algorithm = 1;
    calculate_checksum(&ctx);

    if (!write_compressed_data(&ctx, "output.cst")) {
        printf("Error: Failed to write compressed file\n");
        cleanup_context(&ctx);
        return 1;
    }

    printf("Compression complete:\n");
    printf("Original size: %u bytes\n", ctx.header.original_size);
    printf("Compressed size: %u bytes\n", ctx.header.compressed_size);
    printf("Blocks: %zu\n", ctx.total_blocks);
    printf("Checksum: 0x%02X\n", ctx.header.checksum);

    cleanup_context(&ctx);
    return 0;
}