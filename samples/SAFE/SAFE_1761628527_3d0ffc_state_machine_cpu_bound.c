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

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->digit_count = 0;
    ctx->letter_count = 0;
    ctx->symbol_count = 0;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
}

static parser_state_t transition_state(parser_context_t *ctx, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                ctx->digit_count++;
                return STATE_READING_DIGITS;
            } else if (isalpha(c)) {
                ctx->letter_count++;
                return STATE_READING_LETTERS;
            } else if (c != '\0' && c != '\n') {
                ctx->symbol_count++;
                return STATE_READING_SYMBOLS;
            }
            break;
            
        case STATE_READING_DIGITS:
            if (isdigit(c)) {
                ctx->digit_count++;
                return STATE_READING_DIGITS;
            } else if (isalpha(c)) {
                ctx->letter_count++;
                return STATE_READING_LETTERS;
            } else if (c != '\0' && c != '\n') {
                ctx->symbol_count++;
                return STATE_READING_SYMBOLS;
            }
            break;
            
        case STATE_READING_LETTERS:
            if (isdigit(c)) {
                ctx->digit_count++;
                return STATE_READING_DIGITS;
            } else if (isalpha(c)) {
                ctx->letter_count++;
                return STATE_READING_LETTERS;
            } else if (c != '\0' && c != '\n') {
                ctx->symbol_count++;
                return STATE_READING_SYMBOLS;
            }
            break;
            
        case STATE_READING_SYMBOLS:
            if (isdigit(c)) {
                ctx->digit_count++;
                return STATE_READING_DIGITS;
            } else if (isalpha(c)) {
                ctx->letter_count++;
                return STATE_READING_LETTERS;
            } else if (c != '\0' && c != '\n') {
                ctx->symbol_count++;
                return STATE_READING_SYMBOLS;
            }
            break;
            
        default:
            break;
    }
    
    return ctx->current_state;
}

static void process_input(parser_context_t *ctx) {
    size_t len = strlen(ctx->input_buffer);
    if (len > MAX_INPUT_LEN) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        ctx->current_state = transition_state(ctx, ctx->input_buffer[i]);
        if (ctx->current_state == STATE_ERROR) {
            break;
        }
    }
}

static void print_results(const parser_context_t *ctx) {
    printf("Parser completed. Final state: ");
    switch (ctx->current_state) {
        case STATE_START: printf("START"); break;
        case STATE_READING_DIGITS: printf("READING_DIGITS"); break;
        case STATE_READING_LETTERS: printf("READING_LETTERS"); break;
        case STATE_READING_SYMBOLS: printf("READING_SYMBOLS"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf("\n");
    printf("Digits: %d\n", ctx->digit_count);
    printf("Letters: %d\n", ctx->letter_count);
    printf("Symbols: %d\n", ctx->symbol_count);
}

int main(void) {
    parser_context_t parser;
    initialize_parser(&parser);
    
    printf("Enter text to parse (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(parser.input_buffer, sizeof(parser.input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(parser.input_buffer);
    if (len > 0 && parser.input_buffer[len - 1] == '\n') {
        parser.input_buffer[len - 1] = '\0';
    }
    
    if (strlen(parser.input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    process_input(&parser);
    print_results(&parser);
    
    return EXIT_SUCCESS;
}