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

void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->symbol_count = 0;
    ctx->total_chars = 0;
}

parser_state_t transition_state(parser_state_t current, char c) {
    switch (current) {
        case STATE_START:
            if (isdigit(c)) return STATE_READING_NUMBER;
            if (isalpha(c)) return STATE_READING_WORD;
            if (ispunct(c)) return STATE_READING_SYMBOL;
            return STATE_ERROR;
            
        case STATE_READING_NUMBER:
            if (isdigit(c)) return STATE_READING_NUMBER;
            if (isspace(c)) return STATE_START;
            if (isalpha(c) || ispunct(c)) return STATE_ERROR;
            return STATE_READING_NUMBER;
            
        case STATE_READING_WORD:
            if (isalnum(c)) return STATE_READING_WORD;
            if (isspace(c)) return STATE_START;
            if (ispunct(c)) return STATE_ERROR;
            return STATE_READING_WORD;
            
        case STATE_READING_SYMBOL:
            if (ispunct(c)) return STATE_READING_SYMBOL;
            if (isspace(c)) return STATE_START;
            if (isalnum(c)) return STATE_ERROR;
            return STATE_READING_SYMBOL;
            
        case STATE_ERROR:
            if (isspace(c)) return STATE_START;
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
    }
}

void process_character(parser_context_t *ctx, char c) {
    parser_state_t previous_state = ctx->current_state;
    ctx->current_state = transition_state(ctx->current_state, c);
    
    if (previous_state == STATE_START) {
        if (ctx->current_state == STATE_READING_NUMBER) ctx->number_count++;
        else if (ctx->current_state == STATE_READING_WORD) ctx->word_count++;
        else if (ctx->current_state == STATE_READING_SYMBOL) ctx->symbol_count++;
    }
    
    ctx->total_chars++;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

void process_input(parser_context_t *ctx, const char *input) {
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return;
    }
    
    for (size_t i = 0; i < strlen(input); i++) {
        process_character(ctx, input[i]);
    }
    
    if (ctx->current_state != STATE_START && ctx->current_state != STATE_ERROR) {
        process_character(ctx, ' ');
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    parser_context_t ctx;
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input length\n");
        return 1;
    }
    
    initialize_parser(&ctx);
    process_input(&ctx, input);
    
    printf("Parsing results:\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    printf("Symbol sequences found: %d\n", ctx.symbol_count);
    printf("Total characters processed: %d\n", ctx.total_chars);
    printf("Final state: %d\n", ctx.current_state);
    
    return 0;
}