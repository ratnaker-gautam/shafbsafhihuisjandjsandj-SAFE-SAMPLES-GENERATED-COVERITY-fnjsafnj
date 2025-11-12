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

static void initialize_context(parser_context_t *ctx) {
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
            
        case STATE_ERROR:
            return STATE_ERROR;
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

static int get_user_input(parser_context_t *ctx) {
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(ctx->input_buffer, MAX_INPUT_LEN + 1, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(ctx->input_buffer);
    if (len > 0 && ctx->input_buffer[len - 1] == '\n') {
        ctx->input_buffer[len - 1] = '\0';
    }
    
    return 1;
}

static void perform_cpu_work(int iterations) {
    volatile double result = 1.0;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 1000; j++) {
            result += (double)j / (i + 1);
            result *= 1.0001;
        }
    }
}

int main(void) {
    parser_context_t ctx;
    int iterations = 10000;
    
    initialize_context(&ctx);
    
    if (!get_user_input(&ctx)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    process_input(&ctx);
    
    perform_cpu_work(iterations);
    
    printf("Analysis complete:\n");
    printf("Final state: %d\n", ctx.current_state);
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.letter_count);
    printf("Symbols: %d\n", ctx.symbol_count);
    printf("CPU work iterations: %d\n", iterations);
    
    return 0;
}