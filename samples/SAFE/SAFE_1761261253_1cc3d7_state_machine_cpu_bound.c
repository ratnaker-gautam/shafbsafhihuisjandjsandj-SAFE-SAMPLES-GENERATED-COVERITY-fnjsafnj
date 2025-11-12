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

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->symbol_count = 0;
    ctx->total_chars = 0;
}

static parser_state_t handle_start_state(int ch) {
    if (isdigit(ch)) return STATE_READING_NUMBER;
    if (isalpha(ch)) return STATE_READING_WORD;
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    if (isspace(ch)) return STATE_START;
    return STATE_ERROR;
}

static parser_state_t handle_reading_number(int ch) {
    if (isdigit(ch)) return STATE_READING_NUMBER;
    if (isspace(ch)) return STATE_START;
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    return STATE_ERROR;
}

static parser_state_t handle_reading_word(int ch) {
    if (isalnum(ch)) return STATE_READING_WORD;
    if (isspace(ch)) return STATE_START;
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    return STATE_ERROR;
}

static parser_state_t handle_reading_symbol(int ch) {
    if (ispunct(ch)) return STATE_READING_SYMBOL;
    if (isspace(ch)) return STATE_START;
    return STATE_ERROR;
}

static void process_character(parser_context_t *ctx, int ch) {
    parser_state_t next_state = STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_START:
            next_state = handle_start_state(ch);
            break;
        case STATE_READING_NUMBER:
            next_state = handle_reading_number(ch);
            if (next_state == STATE_START) ctx->number_count++;
            break;
        case STATE_READING_WORD:
            next_state = handle_reading_word(ch);
            if (next_state == STATE_START) ctx->word_count++;
            break;
        case STATE_READING_SYMBOL:
            next_state = handle_reading_symbol(ch);
            if (next_state == STATE_START) ctx->symbol_count++;
            break;
        case STATE_ERROR:
            next_state = STATE_ERROR;
            break;
    }
    
    ctx->current_state = next_state;
    ctx->total_chars++;
}

static void finalize_counts(parser_context_t *ctx) {
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

static void perform_cpu_intensive_processing(parser_context_t *ctx) {
    for (int i = 0; i < 1000000; i++) {
        int temp = ctx->number_count + ctx->word_count + ctx->symbol_count;
        temp = temp * temp;
        temp = temp % 1000;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_context_t parser;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    initialize_parser(&parser);
    
    for (size_t i = 0; i < input_len; i++) {
        process_character(&parser, input_buffer[i]);
        if (parser.current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }
    
    finalize_counts(&parser);
    perform_cpu_intensive_processing(&parser);
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    printf("Symbol sequences found: %d\n", parser.symbol_count);
    printf("Total characters processed: %d\n", parser.total_chars);
    
    return 0;
}