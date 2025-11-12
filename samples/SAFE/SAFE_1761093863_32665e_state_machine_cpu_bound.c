//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_DIGITS,
    STATE_READING_LETTERS,
    STATE_READING_SYMBOLS,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int digit_count;
    int letter_count;
    int symbol_count;
    char input_buffer[MAX_INPUT_LEN + 1];
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->digit_count = 0;
    ctx->letter_count = 0;
    ctx->symbol_count = 0;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
}

static parser_state_t transition_state(parser_state_t current, char c) {
    switch (current) {
        case STATE_START:
            if (isdigit(c)) return STATE_READING_DIGITS;
            if (isalpha(c)) return STATE_READING_LETTERS;
            if (ispunct(c)) return STATE_READING_SYMBOLS;
            return STATE_ERROR;
            
        case STATE_READING_DIGITS:
            if (isdigit(c)) return STATE_READING_DIGITS;
            if (isalpha(c)) return STATE_READING_LETTERS;
            if (ispunct(c)) return STATE_READING_SYMBOLS;
            return STATE_ERROR;
            
        case STATE_READING_LETTERS:
            if (isdigit(c)) return STATE_READING_DIGITS;
            if (isalpha(c)) return STATE_READING_LETTERS;
            if (ispunct(c)) return STATE_READING_SYMBOLS;
            return STATE_ERROR;
            
        case STATE_READING_SYMBOLS:
            if (isdigit(c)) return STATE_READING_DIGITS;
            if (isalpha(c)) return STATE_READING_LETTERS;
            if (ispunct(c)) return STATE_READING_SYMBOLS;
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
    }
}

static void process_character(parser_context_t *ctx, char c) {
    parser_state_t new_state = transition_state(ctx->current_state, c);
    
    if (new_state == STATE_ERROR) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    switch (new_state) {
        case STATE_READING_DIGITS:
            ctx->digit_count++;
            break;
        case STATE_READING_LETTERS:
            ctx->letter_count++;
            break;
        case STATE_READING_SYMBOLS:
            ctx->symbol_count++;
            break;
        default:
            break;
    }
    
    ctx->current_state = new_state;
}

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int main(void) {
    parser_context_t parser;
    char user_input[MAX_INPUT_LEN + 2];
    
    printf("Enter a string to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(user_input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_parser(&parser);
    strncpy(parser.input_buffer, user_input, MAX_INPUT_LEN);
    parser.input_buffer[MAX_INPUT_LEN] = '\0';
    
    for (size_t i = 0; i < input_len; i++) {
        process_character(&parser, user_input[i]);
        if (parser.current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (parser.current_state == STATE_ERROR) {
        printf("Input contains invalid transitions\n");
    } else {
        printf("Analysis complete:\n");
        printf("Digits: %d\n", parser.digit_count);
        printf("Letters: %d\n", parser.letter_count);
        printf("Symbols: %d\n", parser.symbol_count);
        printf("Total characters processed: %zu\n", input_len);
    }
    
    return EXIT_SUCCESS;
}