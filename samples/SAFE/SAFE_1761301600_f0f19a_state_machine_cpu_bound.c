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

static void initialize_context(parser_context_t *ctx) {
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
        
        case STATE_ERROR:
            return STATE_ERROR;
        
        default:
            return STATE_ERROR;
    }
}

static void process_character(parser_context_t *ctx, char c) {
    switch (ctx->current_state) {
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
}

static int validate_and_process_input(parser_context_t *ctx, const char *input) {
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len > MAX_INPUT_LEN) {
        return 0;
    }
    
    strncpy(ctx->input_buffer, input, MAX_INPUT_LEN);
    ctx->input_buffer[MAX_INPUT_LEN] = '\0';
    
    initialize_context(ctx);
    
    for (size_t i = 0; i < input_len; i++) {
        char current_char = ctx->input_buffer[i];
        
        if (!isprint(current_char)) {
            ctx->current_state = STATE_ERROR;
            return 0;
        }
        
        parser_state_t next_state = transition_state(ctx->current_state, current_char);
        
        if (next_state == STATE_ERROR) {
            ctx->current_state = STATE_ERROR;
            return 0;
        }
        
        ctx->current_state = next_state;
        process_character(ctx, current_char);
    }
    
    return 1;
}

int main(void) {
    parser_context_t ctx;
    char user_input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
    }
    
    if (!validate_and_process_input(&ctx, user_input)) {
        printf("Invalid input or parsing error\n");
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.letter_count);
    printf("Symbols: %d\n", ctx.symbol_count);
    printf("Final state: %d\n", ctx.current_state);
    
    return 0;
}