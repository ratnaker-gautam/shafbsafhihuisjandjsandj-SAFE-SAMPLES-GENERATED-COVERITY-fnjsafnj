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
    int total_chars;
} parser_context_t;

static void initialize_context(parser_context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->symbol_count = 0;
    ctx->total_chars = 0;
}

static parser_state_t handle_start_state(char c) {
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        return STATE_READING_WORD;
    } else if (ispunct(c)) {
        return STATE_READING_SYMBOL;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

static parser_state_t handle_reading_number(char c) {
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isspace(c)) {
        return STATE_START;
    } else if (ispunct(c)) {
        return STATE_READING_SYMBOL;
    } else {
        return STATE_ERROR;
    }
}

static parser_state_t handle_reading_word(char c) {
    if (isalnum(c)) {
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        return STATE_START;
    } else if (ispunct(c)) {
        return STATE_READING_SYMBOL;
    } else {
        return STATE_ERROR;
    }
}

static parser_state_t handle_reading_symbol(char c) {
    if (ispunct(c)) {
        return STATE_READING_SYMBOL;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

static void process_character(parser_context_t *ctx, char c) {
    if (ctx == NULL) return;
    
    ctx->total_chars++;
    
    switch (ctx->current_state) {
        case STATE_START:
            ctx->current_state = handle_start_state(c);
            break;
        case STATE_READING_NUMBER:
            ctx->current_state = handle_reading_number(c);
            if (ctx->current_state != STATE_READING_NUMBER) {
                ctx->number_count++;
            }
            break;
        case STATE_READING_WORD:
            ctx->current_state = handle_reading_word(c);
            if (ctx->current_state != STATE_READING_WORD) {
                ctx->word_count++;
            }
            break;
        case STATE_READING_SYMBOL:
            ctx->current_state = handle_reading_symbol(c);
            if (ctx->current_state != STATE_READING_SYMBOL) {
                ctx->symbol_count++;
            }
            break;
        case STATE_ERROR:
            break;
    }
}

static void finalize_counts(parser_context_t *ctx) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_READING_NUMBER:
            ctx->number_count++;
            break;
        case STATE_READING_WORD:
            ctx->word_count++;
            break;
        case STATE_READING_SYMBOL:
            ctx->symbol_count++;
            break;
        default:
            break;
    }
}

static void process_input_string(const char *input, parser_context_t *ctx) {
    if (input == NULL || ctx == NULL) return;
    
    size_t len = strnlen(input, MAX_INPUT_LEN);
    if (len >= MAX_INPUT_LEN) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        process_character(ctx, input[i]);
        if (ctx->current_state == STATE_ERROR) {
            break;
        }
    }
    
    finalize_counts(ctx);
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
    parser_context_t context;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strnlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    initialize_context(&context);
    process_input_string(input_buffer, &context);
    
    if (context.current_state == STATE_ERROR) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Analysis results:\n");
    printf("Total characters processed: %d\n", context.total_chars);