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
    if (ctx == NULL) return;
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
    if (ctx == NULL) return;
    
    parser_state_t new_state = transition_state(ctx->current_state, c);
    ctx->current_state = new_state;
    
    if (new_state == STATE_ERROR) return;
    
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
}

static int validate_and_process_input(parser_context_t *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (input[i] == '\0') break;
        process_character(ctx, input[i]);
        if (ctx->current_state == STATE_ERROR) return 0;
    }
    
    return 1;
}

static void perform_cpu_bound_analysis(parser_context_t *ctx) {
    if (ctx == NULL) return;
    
    int total_chars = ctx->digit_count + ctx->letter_count + ctx->symbol_count;
    if (total_chars == 0) return;
    
    for (int i = 0; i < 1000000; i++) {
        int temp = ctx->digit_count * ctx->letter_count;
        temp += ctx->symbol_count;
        temp = temp % 1000;
        if (temp < 0) temp = -temp;
    }
    
    double digit_ratio = (double)ctx->digit_count / total_chars * 100.0;
    double letter_ratio = (double)ctx->letter_count / total_chars * 100.0;
    double symbol_ratio = (double)ctx->symbol_count / total_chars * 100.0;
    
    printf("Analysis complete:\n");
    printf("Digits: %d (%.1f%%)\n", ctx->digit_count, digit_ratio);
    printf("Letters: %d (%.1f%%)\n", ctx->letter_count, letter_ratio);
    printf("Symbols: %d (%.1f%%)\n", ctx->symbol_count, symbol_ratio);
    printf("Total characters processed: %d\n", total_chars);
}

int main(void) {
    parser_context_t ctx;
    char user_input[MAX_INPUT_LEN + 1];
    
    initialize_context(&ctx);
    
    printf("Enter a string to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    
    if (!validate_and_process_input(&ctx, user_input