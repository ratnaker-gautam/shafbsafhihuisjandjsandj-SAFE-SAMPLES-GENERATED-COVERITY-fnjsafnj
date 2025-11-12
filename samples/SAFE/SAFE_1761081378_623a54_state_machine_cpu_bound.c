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
    STATE_SKIPPING,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    int total_chars;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->total_chars = 0;
}

static parser_state_t handle_start_state(int ch, parser_context_t *ctx) {
    if (isdigit(ch)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha(ch)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (isspace(ch)) {
        return STATE_START;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_reading_number(int ch, parser_context_t *ctx) {
    if (isdigit(ch)) {
        return STATE_READING_NUMBER;
    } else if (isalpha(ch)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (isspace(ch)) {
        return STATE_START;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_reading_word(int ch, parser_context_t *ctx) {
    if (isalpha(ch)) {
        return STATE_READING_WORD;
    } else if (isdigit(ch)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isspace(ch)) {
        return STATE_START;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_skipping_state(int ch, parser_context_t *ctx) {
    if (isspace(ch)) {
        return STATE_START;
    }
    return STATE_SKIPPING;
}

static void process_character(int ch, parser_context_t *ctx) {
    ctx->total_chars++;
    
    switch (ctx->current_state) {
        case STATE_START:
            ctx->current_state = handle_start_state(ch, ctx);
            break;
        case STATE_READING_NUMBER:
            ctx->current_state = handle_reading_number(ch, ctx);
            break;
        case STATE_READING_WORD:
            ctx->current_state = handle_reading_word(ch, ctx);
            break;
        case STATE_SKIPPING:
            ctx->current_state = handle_skipping_state(ch, ctx);
            break;
        case STATE_END:
            break;
    }
}

static void process_input_string(const char *input, parser_context_t *ctx) {
    size_t len = strlen(input);
    if (len > MAX_INPUT_LEN) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        process_character(input[i], ctx);
    }
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
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_context_t parser;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input_buffer[0] == '\0')) {
        printf("No input provided\n");
        return 1;
    }
    
    initialize_parser(&parser);
    process_input_string(input_buffer, &parser);
    
    printf("Processing complete. Performing CPU work...\n");
    perform_cpu_work(10000);
    
    printf("Analysis results:\n");
    printf("Total characters processed: %d\n", parser.total_chars);
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}