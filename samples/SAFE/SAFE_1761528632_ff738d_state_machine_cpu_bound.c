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
    int position;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->symbol_count = 0;
    ctx->position = 0;
}

static parser_state_t transition_state(parser_state_t current, char c) {
    switch (current) {
        case STATE_START:
            if (isdigit(c)) return STATE_READING_NUMBER;
            if (isalpha(c)) return STATE_READING_WORD;
            if (ispunct(c)) return STATE_READING_SYMBOL;
            return STATE_ERROR;
            
        case STATE_READING_NUMBER:
            if (isdigit(c)) return STATE_READING_NUMBER;
            if (isspace(c)) return STATE_START;
            if (ispunct(c)) return STATE_READING_SYMBOL;
            return STATE_ERROR;
            
        case STATE_READING_WORD:
            if (isalnum(c)) return STATE_READING_WORD;
            if (isspace(c)) return STATE_START;
            if (ispunct(c)) return STATE_READING_SYMBOL;
            return STATE_ERROR;
            
        case STATE_READING_SYMBOL:
            if (ispunct(c)) return STATE_READING_SYMBOL;
            if (isspace(c)) return STATE_START;
            return STATE_ERROR;
            
        case STATE_ERROR:
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

static void process_character(parser_context_t *ctx, char c) {
    parser_state_t new_state = transition_state(ctx->current_state, c);
    
    if (ctx->current_state == STATE_START && new_state != STATE_START) {
        switch (new_state) {
            case STATE_READING_NUMBER: ctx->number_count++; break;
            case STATE_READING_WORD: ctx->word_count++; break;
            case STATE_READING_SYMBOL: ctx->symbol_count++; break;
            default: break;
        }
    }
    
    ctx->current_state = new_state;
}

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
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
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_parser(&parser);
    
    for (size_t i = 0; i < input_len; i++) {
        if (parser.position >= MAX_INPUT_LEN) break;
        process_character(&parser, input_buffer[i]);
        parser.position++;
        
        if (parser.current_state == STATE_ERROR) {
            fprintf(stderr, "Parser error at position %d\n", parser.position);
            return EXIT_FAILURE;
        }
    }
    
    printf("Analysis results:\n");
    printf("Numbers: %d\n", parser.number_count);
    printf("Words: %d\n", parser.word_count);
    printf("Symbol sequences: %d\n", parser.symbol_count);
    
    return EXIT_SUCCESS;
}