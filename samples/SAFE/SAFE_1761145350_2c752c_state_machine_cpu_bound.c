//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STATE_COUNT 5
#define MAX_INPUT_LEN 1000

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_SKIPPING,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    uint32_t number_count;
    uint32_t word_count;
    uint32_t other_count;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->other_count = 0;
}

static parser_state_t handle_start_state(char c) {
    if (isdigit(c)) return STATE_READING_NUMBER;
    if (isalpha(c)) return STATE_READING_WORD;
    if (isspace(c)) return STATE_START;
    return STATE_SKIPPING;
}

static parser_state_t handle_reading_number(char c) {
    if (isdigit(c)) return STATE_READING_NUMBER;
    if (isspace(c)) return STATE_START;
    return STATE_SKIPPING;
}

static parser_state_t handle_reading_word(char c) {
    if (isalnum(c)) return STATE_READING_WORD;
    if (isspace(c)) return STATE_START;
    return STATE_SKIPPING;
}

static parser_state_t handle_skipping(char c) {
    if (isspace(c)) return STATE_START;
    return STATE_SKIPPING;
}

static void process_character(parser_context_t *ctx, char c) {
    parser_state_t next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case STATE_START:
            next_state = handle_start_state(c);
            break;
        case STATE_READING_NUMBER:
            next_state = handle_reading_number(c);
            break;
        case STATE_READING_WORD:
            next_state = handle_reading_word(c);
            break;
        case STATE_SKIPPING:
            next_state = handle_skipping(c);
            break;
        case STATE_END:
            return;
    }
    
    if (ctx->current_state != next_state) {
        switch (ctx->current_state) {
            case STATE_READING_NUMBER:
                ctx->number_count++;
                break;
            case STATE_READING_WORD:
                ctx->word_count++;
                break;
            case STATE_SKIPPING:
                ctx->other_count++;
                break;
            default:
                break;
        }
    }
    
    ctx->current_state = next_state;
}

static void finalize_counting(parser_context_t *ctx) {
    switch (ctx->current_state) {
        case STATE_READING_NUMBER:
            ctx->number_count++;
            break;
        case STATE_READING_WORD:
            ctx->word_count++;
            break;
        case STATE_SKIPPING:
            ctx->other_count++;
            break;
        default:
            break;
    }
    ctx->current_state = STATE_END;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0) return 0;
    if (len >= MAX_INPUT_LEN) return 0;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_context_t parser;
    size_t input_len;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_parser(&parser);
    
    for (size_t i = 0; i < input_len; i++) {
        process_character(&parser, input_buffer[i]);
    }
    
    finalize_counting(&parser);
    
    printf("Analysis results:\n");
    printf("Numbers found: %u\n", parser.number_count);
    printf("Words found: %u\n", parser.word_count);
    printf("Other tokens: %u\n", parser.other_count);
    
    return 0;
}