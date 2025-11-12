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

static void initialize_context(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->symbol_count = 0;
    ctx->error_count = 0;
}

static parser_state_t handle_start_state(int ch) {
    if (ch == EOF) return STATE_START;
    if (isspace(ch)) return STATE_START;
    if (isdigit(ch)) return STATE_READING_NUMBER;
    if (isalpha(ch)) return STATE_READING_WORD;
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    return STATE_ERROR;
}

static parser_state_t handle_number_state(int ch) {
    if (ch == EOF) return STATE_START;
    if (isspace(ch)) return STATE_START;
    if (isdigit(ch)) return STATE_READING_NUMBER;
    if (isalpha(ch)) return STATE_ERROR;
    if (ispunct(ch)) return STATE_ERROR;
    return STATE_ERROR;
}

static parser_state_t handle_word_state(int ch) {
    if (ch == EOF) return STATE_START;
    if (isspace(ch)) return STATE_START;
    if (isalnum(ch)) return STATE_READING_WORD;
    if (ispunct(ch)) return STATE_ERROR;
    return STATE_ERROR;
}

static parser_state_t handle_symbol_state(int ch) {
    if (ch == EOF) return STATE_START;
    if (isspace(ch)) return STATE_START;
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    return STATE_ERROR;
}

static void update_counts(parser_context_t *ctx, parser_state_t prev_state, parser_state_t new_state) {
    if (prev_state != new_state) {
        switch (prev_state) {
            case STATE_READING_NUMBER:
                ctx->number_count++;
                break;
            case STATE_READING_WORD:
                ctx->word_count++;
                break;
            case STATE_READING_SYMBOL:
                ctx->symbol_count++;
                break;
            case STATE_ERROR:
                ctx->error_count++;
                break;
            default:
                break;
        }
    }
}

static void process_input(parser_context_t *ctx) {
    int ch;
    parser_state_t prev_state;
    
    while ((ch = getchar()) != EOF) {
        prev_state = ctx->current_state;
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start_state(ch);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_number_state(ch);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_word_state(ch);
                break;
            case STATE_READING_SYMBOL:
                ctx->current_state = handle_symbol_state(ch);
                break;
            case STATE_ERROR:
                ctx->current_state = handle_start_state(ch);
                break;
        }
        
        update_counts(ctx, prev_state, ctx->current_state);
    }
    
    update_counts(ctx, ctx->current_state, STATE_START);
}

static void print_results(const parser_context_t *ctx) {
    printf("Parsing results:\n");
    printf("Numbers: %d\n", ctx->number_count);
    printf("Words: %d\n", ctx->word_count);
    printf("Symbols: %d\n", ctx->symbol_count);
    printf("Errors: %d\n", ctx->error_count);
}

int main(void) {
    parser_context_t context;
    
    initialize_context(&context);
    
    printf("Enter text to parse (Ctrl+D to end):\n");
    process_input(&context);
    
    print_results(&context);
    
    return 0;
}