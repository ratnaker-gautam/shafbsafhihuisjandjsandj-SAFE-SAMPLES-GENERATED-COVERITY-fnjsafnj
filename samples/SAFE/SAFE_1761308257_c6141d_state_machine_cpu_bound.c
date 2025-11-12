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
            if (isalpha(c) || ispunct(c)) return STATE_ERROR;
            return STATE_START;
            
        case STATE_READING_WORD:
            if (isalnum(c)) return STATE_READING_WORD;
            if (ispunct(c)) return STATE_ERROR;
            return STATE_START;
            
        case STATE_READING_SYMBOL:
            if (ispunct(c)) return STATE_READING_SYMBOL;
            if (isalnum(c)) return STATE_ERROR;
            return STATE_START;
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
    }
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
    ctx->total_chars++;
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
    char input[MAX_INPUT_LEN + 2];
    parser_context_t ctx;
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_context(&ctx);
    
    for (size_t i = 0; i < len; i++) {
        process_character(&ctx, input[i]);
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Parsing error at position %d\n", ctx.total_chars);
        return EXIT_FAILURE;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    printf("Symbol sequences: %d\n", ctx.symbol_count);
    printf("Total characters processed: %d\n", ctx.total_chars);
    
    return EXIT_SUCCESS;
}