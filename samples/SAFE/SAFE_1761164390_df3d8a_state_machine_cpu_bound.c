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

static parser_state_t handle_start_state(char c) {
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        return STATE_READING_WORD;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_reading_number(char c) {
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        return STATE_READING_WORD;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_reading_word(char c) {
    if (isalpha(c)) {
        return STATE_READING_WORD;
    } else if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_skipping_state(char c) {
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        return STATE_READING_WORD;
    } else {
        return STATE_SKIPPING;
    }
}

static void process_character(parser_context_t *ctx, char c) {
    parser_state_t previous_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case STATE_START:
            ctx->current_state = handle_start_state(c);
            break;
        case STATE_READING_NUMBER:
            ctx->current_state = handle_reading_number(c);
            break;
        case STATE_READING_WORD:
            ctx->current_state = handle_reading_word(c);
            break;
        case STATE_SKIPPING:
            ctx->current_state = handle_skipping_state(c);
            break;
        case STATE_END:
            return;
    }
    
    if (previous_state == STATE_START || previous_state == STATE_SKIPPING) {
        if (ctx->current_state == STATE_READING_NUMBER) {
            ctx->number_count++;
        } else if (ctx->current_state == STATE_READING_WORD) {
            ctx->word_count++;
        }
    }
    
    ctx->total_chars++;
}

static void process_input_string(const char *input, parser_context_t *ctx) {
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        process_character(ctx, input[i]);
    }
    ctx->current_state = STATE_END;
}

static int validate_input(const char *input) {
    if (input == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
    parser_context_t parser;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_parser(&parser);
    process_input_string(input_buffer, &parser);
    
    printf("Analysis results:\n");
    printf("Total characters processed: %d\n", parser.total_chars);
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}