//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
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

static parser_state_t handle_start(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (isalpha(c)) {
        return STATE_READING_WORD;
    } else if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_skipping(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else {
        return STATE_SKIPPING;
    }
}

static void process_character(char c, parser_context_t *ctx) {
    ctx->total_chars++;
    
    switch (ctx->current_state) {
        case STATE_START:
            ctx->current_state = handle_start(c, ctx);
            break;
        case STATE_READING_NUMBER:
            ctx->current_state = handle_reading_number(c, ctx);
            break;
        case STATE_READING_WORD:
            ctx->current_state = handle_reading_word(c, ctx);
            break;
        case STATE_SKIPPING:
            ctx->current_state = handle_skipping(c, ctx);
            break;
        case STATE_END:
            break;
    }
}

static void process_input(const char *input, parser_context_t *ctx) {
    size_t len = strlen(input);
    if (len > MAX_INPUT_LEN) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        process_character(input[i], ctx);
    }
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
        if (input[i] == '\0') {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_context_t parser;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_parser(&parser);
    process_input(input_buffer, &parser);
    
    printf("Analysis results:\n");
    printf("Total characters processed: %d\n", parser.total_chars);
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    printf("Final state: %d\n", parser.current_state);
    
    return EXIT_SUCCESS;
}