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

static parser_state_t transition_state(parser_context_t *ctx, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                ctx->digit_count++;
                return STATE_READING_DIGITS;
            } else if (isalpha(c)) {
                ctx->letter_count++;
                return STATE_READING_LETTERS;
            } else if (c >= 33 && c <= 126) {
                ctx->symbol_count++;
                return STATE_READING_SYMBOLS;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_DIGITS:
            if (isdigit(c)) {
                ctx->digit_count++;
                return STATE_READING_DIGITS;
            } else if (isalpha(c)) {
                ctx->letter_count++;
                return STATE_READING_LETTERS;
            } else if (c >= 33 && c <= 126) {
                ctx->symbol_count++;
                return STATE_READING_SYMBOLS;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_LETTERS:
            if (isdigit(c)) {
                ctx->digit_count++;
                return STATE_READING_DIGITS;
            } else if (isalpha(c)) {
                ctx->letter_count++;
                return STATE_READING_LETTERS;
            } else if (c >= 33 && c <= 126) {
                ctx->symbol_count++;
                return STATE_READING_SYMBOLS;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_SYMBOLS:
            if (isdigit(c)) {
                ctx->digit_count++;
                return STATE_READING_DIGITS;
            } else if (isalpha(c)) {
                ctx->letter_count++;
                return STATE_READING_LETTERS;
            } else if (c >= 33 && c <= 126) {
                ctx->symbol_count++;
                return STATE_READING_SYMBOLS;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
    }
}

static int process_input(parser_context_t *ctx, const char *input) {
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len > MAX_INPUT_LEN) {
        return -1;
    }
    
    strncpy(ctx->input_buffer, input, MAX_INPUT_LEN);
    ctx->input_buffer[MAX_INPUT_LEN] = '\0';
    
    for (size_t i = 0; i < input_len; i++) {
        ctx->current_state = transition_state(ctx, ctx->input_buffer[i]);
        if (ctx->current_state == STATE_ERROR) {
            return -1;
        }
    }
    
    return 0;
}

static void print_statistics(const parser_context_t *ctx) {
    printf("Input analysis complete:\n");
    printf("Digits: %d\n", ctx->digit_count);
    printf("Letters: %d\n", ctx->letter_count);
    printf("Symbols: %d\n", ctx->symbol_count);
    printf("Total characters processed: %d\n", 
           ctx->digit_count + ctx->letter_count + ctx->symbol_count);
}

int main(void) {
    parser_context_t parser;
    char user_input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
    }
    
    if (strlen(user_input) == 0) {
        fprintf(stderr, "Empty input string\n");
        return EXIT_FAILURE;
    }
    
    initialize_parser(&parser);
    
    if (process_input(&parser, user_input) != 0) {
        fprintf(stderr, "Error processing input: invalid characters detected\n");
        return EXIT_FAILURE;
    }
    
    print_statistics(&parser);
    
    return EXIT_SUCCESS;
}