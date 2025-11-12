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
    int error_count;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->symbol_count = 0;
    ctx->error_count = 0;
}

static parser_state_t handle_start_state(char c) {
    if (isdigit(c)) return STATE_READING_NUMBER;
    if (isalpha(c)) return STATE_READING_WORD;
    if (ispunct(c)) return STATE_READING_SYMBOL;
    return STATE_ERROR;
}

static parser_state_t handle_reading_number(char c) {
    if (isdigit(c)) return STATE_READING_NUMBER;
    if (isspace(c)) return STATE_START;
    return STATE_ERROR;
}

static parser_state_t handle_reading_word(char c) {
    if (isalnum(c)) return STATE_READING_WORD;
    if (isspace(c)) return STATE_START;
    return STATE_ERROR;
}

static parser_state_t handle_reading_symbol(char c) {
    if (ispunct(c)) return STATE_READING_SYMBOL;
    if (isspace(c)) return STATE_START;
    return STATE_ERROR;
}

static void process_character(parser_context_t *ctx, char c) {
    parser_state_t next_state = STATE_ERROR;
    
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
        case STATE_READING_SYMBOL:
            next_state = handle_reading_symbol(c);
            break;
        case STATE_ERROR:
            next_state = handle_start_state(c);
            break;
    }
    
    if (ctx->current_state == STATE_READING_NUMBER && next_state == STATE_START) {
        ctx->number_count++;
    } else if (ctx->current_state == STATE_READING_WORD && next_state == STATE_START) {
        ctx->word_count++;
    } else if (ctx->current_state == STATE_READING_SYMBOL && next_state == STATE_START) {
        ctx->symbol_count++;
    }
    
    if (next_state == STATE_ERROR) {
        ctx->error_count++;
    }
    
    ctx->current_state = next_state;
}

static void finalize_counting(parser_context_t *ctx) {
    if (ctx->current_state == STATE_READING_NUMBER) {
        ctx->number_count++;
    } else if (ctx->current_state == STATE_READING_WORD) {
        ctx->word_count++;
    } else if (ctx->current_state == STATE_READING_SYMBOL) {
        ctx->symbol_count++;
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
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_parser(&parser);
    
    for (size_t i = 0; i < input_len; i++) {
        process_character(&parser, input_buffer[i]);
    }
    
    finalize_counting(&parser);
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    printf("Symbol sequences found: %d\n", parser.symbol_count);
    printf("Parsing errors: %d\n", parser.error_count);
    
    return EXIT_SUCCESS;
}