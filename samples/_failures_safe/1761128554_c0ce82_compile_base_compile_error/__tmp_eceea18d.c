//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_BLOCK_SIZE 4096
#define MAX_DICT_SIZE 256

struct CompressionHeader {
    uint32_t magic;
    uint16_t version;
    uint32_t original_size;
    uint32_t compressed_size;
    uint8_t algorithm;
    uint8_t checksum;
};

struct DictionaryEntry {
    uint8_t symbol;
    uint32_t frequency;
    uint8_t code_length;
    uint16_t code;
};

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    size_t size;
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
    uint32_t crc;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    size_t total_processed;
    uint8_t state;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535455) return 0;
    if (header->version > 1) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > header->original_size) return 0;
    if (header->algorithm > 2) return 0;
    return 1;
}

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->header.algorithm = 1;
    ctx->current_block.size = 0;
    ctx->current_block.dict_size = 0;
    ctx->current_block.crc = 0;
    ctx->total_processed = 0;
    ctx->state = 1;
}

uint32_t calculate_crc(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) return 0;
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return ~crc;
}

int process_block(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) return 0;
    if (input_size > MAX_BLOCK_SIZE) return 0;
    if (ctx->state != 1) return 0;

    memcpy(ctx->current_block.data, input, input_size);
    ctx->current_block.size = input_size;
    ctx->current_block.crc = calculate_crc(input, input_size);

    for (size_t i = 0; i < input_size; i++) {
        uint8_t symbol = input[i];
        int found = 0;
        for (size_t j = 0; j < ctx->current_block.dict_size; j++) {
            if (ctx->current_block.dictionary[j].symbol == symbol) {
                if (ctx->current_block.dictionary[j].frequency < UINT32_MAX) {
                    ctx->current_block.dictionary[j].frequency++;
                }
                found = 1;
                break;
            }
        }
        if (!found && ctx->current_block.dict_size < MAX_DICT_SIZE) {
            ctx->current_block.dictionary[ctx->current_block.dict_size].symbol = symbol;
            ctx->current_block.dictionary[ctx->current_block.dict_size].frequency = 1;
            ctx->current_block.dictionary[ctx->current_block.dict_size].code_length = 0;
            ctx->current_block.dictionary[ctx->current_block.dict_size].code = 0;
            ctx->current_block.dict_size++;
        }
    }

    ctx->total_processed += input_size;
    return 1;
}

void generate_codes(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    for (size_t i = 0; i < ctx->current_block.dict_size; i++) {
        if (i < 8) {
            ctx->current_block.dictionary[i].code_length = (uint8_t)((i % 8) + 1);
            ctx->current_block.dictionary[i].code = (uint16_t)(i * 3);
        }
    }
}

int compress_data(struct CompressionContext *ctx, uint8_t *output, size_t *output_size) {
    if (ctx == NULL || output == NULL || output_size == NULL) return 0;
    if (*output_size < ctx->current_block.size) return 0;

    generate_codes(ctx);

    size_t compressed = 0;
    for (size_t i = 0; i < ctx->current_block.size; i++) {
        uint8_t symbol = ctx->current_block.data[i];
        for (size_t j = 0; j < ctx->current_block.dict_size; j++) {
            if (ctx->current_block.dictionary[j].symbol == symbol) {
                if (compressed < *output_size) {
                    output[compressed] = (uint8_t)ctx->current_block