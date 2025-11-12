//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_READING_SYMBOL,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    int symbol_count;
    int total_chars;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->symbol_count = 0;
    ctx->total_chars = 0;
}

static parser_state_t handle_start_state(int ch) {
    if (isdigit(ch)) return STATE_READING_NUMBER;
    if (isalpha(ch)) return STATE_READING_WORD;
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    if (isspace(ch)) return STATE_START;
    return STATE_ERROR;
}

static parser_state_t handle_reading_number(int ch) {
    if (isdigit(ch)) return STATE_READING_NUMBER;
    if (isspace(ch)) return STATE_START;
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    return STATE_ERROR;
}

static parser_state_t handle_reading_word(int ch) {
    if (isalnum(ch)) return STATE_READING_WORD;
    if (isspace(ch)) return STATE_START;
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    return STATE_ERROR;
}

static parser_state_t handle_reading_symbol(int ch) {
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    if (isspace(ch)) return STATE_START;
    return STATE_ERROR;
}

static void process_character(parser_context_t *ctx, int ch) {
    parser_state_t next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case STATE_START:
            next_state = handle_start_state(ch);
            break;
        case STATE_READING_NUMBER:
            next_state = handle_reading_number(ch);
            if (next_state != STATE_READING_NUMBER) ctx->number_count++;
            break;
        case STATE_READING_WORD:
            next_state = handle_reading_word(ch);
            if (next_state != STATE_READING_WORD) ctx->word_count++;
            break;
        case STATE_READING_SYMBOL:
            next_state = handle_reading_symbol(ch);
            if (next_state != STATE_READING_SYMBOL) ctx->symbol_count++;
            break;
        case STATE_ERROR:
            break;
    }
    
    ctx->current_state = next_state;
    ctx->total_chars++;
}

static void finalize_counting(parser_context_t *ctx) {
    switch (ctx->current_state) {
        case STATE_READING_NUMBER:
            ctx->number_count++;
            break;
        case STATE_READING_WORD:
            ctx->word_count++;
            break;
        case STATE_READING_SYMBOL:
            ctx->symbol_count++;
            break;
        default:
            break;
    }
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_context_t parser;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_parser(&parser);
    
    for (size_t i = 0; i < input_len; i++) {
        process_character(&parser, input_buffer[i]);
        if (parser.current_state == STATE_ERROR) {
            fprintf(stderr, "Parser encountered invalid sequence\n");
            return EXIT_FAILURE;
        }
    }
    
    finalize_counting(&parser);
    
    printf("Analysis results:\n");
    printf("Total characters processed: %d\n", parser.total_chars);
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    printf("Symbol sequences found: %d\n", parser.symbol_count);
    
    return EXIT_SUCCESS;
}