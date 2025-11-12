//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_SKIPPING,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    uint64_t number_sum;
    uint32_t word_count;
    uint32_t number_count;
    char current_word[64];
    size_t word_pos;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_sum = 0;
    ctx->word_count = 0;
    ctx->number_count = 0;
    ctx->word_pos = 0;
    memset(ctx->current_word, 0, sizeof(ctx->current_word));
}

static int is_valid_number(const char *str, size_t len) {
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(str[i])) return 0;
    }
    if (len == 10) {
        const char *max_uint32 = "4294967295";
        for (size_t i = 0; i < len; i++) {
            if (str[i] < max_uint32[i]) break;
            if (str[i] > max_uint32[i]) return 0;
        }
    }
    return 1;
}

static uint32_t parse_number(const char *str, size_t len) {
    uint32_t result = 0;
    for (size_t i = 0; i < len; i++) {
        if (result > UINT32_MAX / 10) return 0;
        result *= 10;
        uint32_t digit = str[i] - '0';
        if (result > UINT32_MAX - digit) return 0;
        result += digit;
    }
    return result;
}

static void process_character(parser_context_t *ctx, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->current_word[0] = c;
                ctx->word_pos = 1;
            } else if (isalpha(c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->current_word[0] = c;
                ctx->word_pos = 1;
            } else if (!isspace(c)) {
                ctx->current_state = STATE_SKIPPING;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->word_pos < sizeof(ctx->current_word) - 1) {
                    ctx->current_word[ctx->word_pos++] = c;
                }
            } else {
                if (is_valid_number(ctx->current_word, ctx->word_pos)) {
                    uint32_t num = parse_number(ctx->current_word, ctx->word_pos);
                    ctx->number_sum += num;
                    ctx->number_count++;
                }
                if (isalpha(c)) {
                    ctx->current_state = STATE_READING_WORD;
                    ctx->current_word[0] = c;
                    ctx->word_pos = 1;
                } else if (isspace(c)) {
                    ctx->current_state = STATE_START;
                } else {
                    ctx->current_state = STATE_SKIPPING;
                }
            }
            break;
            
        case STATE_READING_WORD:
            if (isalnum(c)) {
                if (ctx->word_pos < sizeof(ctx->current_word) - 1) {
                    ctx->current_word[ctx->word_pos++] = c;
                }
            } else {
                ctx->word_count++;
                if (isdigit(c)) {
                    ctx->current_state = STATE_READING_NUMBER;
                    ctx->current_word[0] = c;
                    ctx->word_pos = 1;
                } else if (isspace(c)) {
                    ctx->current_state = STATE_START;
                } else {
                    ctx->current_state = STATE_SKIPPING;
                }
            }
            break;
            
        case STATE_SKIPPING:
            if (isspace(c)) {
                ctx->current_state = STATE_START;
            } else if (isdigit(c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->current_word[0] = c;
                ctx->word_pos = 1;
            } else if (isalpha(c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->current_word[0] = c;
                ctx->word_pos = 1;
            }
            break;
            
        case STATE_END:
            break;
    }
}

static void finalize_parser(parser_context_t *ctx) {
    switch (ctx->current_state) {
        case STATE_READING_NUMBER:
            if (is_valid_number(ctx->current_word, ctx->word_pos)) {
                uint32_t num = parse_number(ctx->current_word, ctx->word_pos);
                ctx->number_sum += num;
                ctx->number_count++;
            }
            break;
        case STATE_READING_WORD:
            ctx->word_count++;
            break;
        default:
            break;
    }
    ctx->current_state = STATE_END;
}

int main(void