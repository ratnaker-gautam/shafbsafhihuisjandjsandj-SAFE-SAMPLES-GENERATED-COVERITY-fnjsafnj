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

static void initialize_context(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->symbol_count = 0;
    ctx->total_chars = 0;
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

static void process_character(parser_context_t *ctx, char c) {
    parser_state_t previous_state = ctx->current_state;
    ctx->current_state = transition_state(ctx->current_state, c);
    
    if (previous_state == STATE_START) {
        if (ctx->current_state == STATE_READING_NUMBER) ctx->number_count++;
        else if (ctx->current_state == STATE_READING_WORD) ctx->word_count++;
        else if (ctx->current_state == STATE_READING_SYMBOL) ctx->symbol_count++;
    }
    
    ctx->total_chars++;
}

static void process_input_string(const char *input, parser_context_t *ctx) {
    size_t len = strlen(input);
    if (len > MAX_INPUT_LEN) {
        printf("Input too long. Maximum %d characters allowed.\n", MAX_INPUT_LEN);
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        process_character(ctx, input[i]);
    }
    
    if (ctx->current_state != STATE_START && ctx->current_state != STATE_ERROR) {
        process_character(ctx, ' ');
    }
}

static void display_results(const parser_context_t *ctx) {
    printf("Parsing completed. Results:\n");
    printf("Numbers found: %d\n", ctx->number_count);
    printf("Words found: %d\n", ctx->word_count);
    printf("Symbol sequences found: %d\n", ctx->symbol_count);
    printf("Total characters processed: %d\n", ctx->total_chars);
    
    if (ctx->current_state == STATE_ERROR) {
        printf("Warning: Input contained parsing errors.\n");
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
    parser_context_t context;
    
    printf("Enter text to parse (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (input_len == 1 && input_buffer[0] == '\n') {
        printf("No input provided.\n");
        return 1;
    }
    
    initialize_context(&context);
    process_input_string(input_buffer, &context);
    display_results(&context);
    
    return 0;
}