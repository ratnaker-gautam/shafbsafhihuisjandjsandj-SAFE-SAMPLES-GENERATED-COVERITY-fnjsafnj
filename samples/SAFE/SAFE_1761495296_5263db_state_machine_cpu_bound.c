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

static void validate_input(const char *input) {
    if (input == NULL) {
        fprintf(stderr, "Error: Input cannot be NULL\n");
        exit(EXIT_FAILURE);
    }
    
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Error: Input too long\n");
        exit(EXIT_FAILURE);
    }
    
    if (len == 0) {
        fprintf(stderr, "Error: Input cannot be empty\n");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    parser_context_t parser;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    validate_input(input);
    
    initialize_parser(&parser);
    
    for (size_t i = 0; i < len; i++) {
        process_character(input[i], &parser);
    }
    
    parser.current_state = STATE_END;
    
    printf("Analysis results:\n");
    printf("Total characters processed: %d\n", parser.total_chars);
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    if (parser.total_chars > 0) {
        double number_ratio = (double)parser.number_count / parser.total_chars;
        double word_ratio = (double)parser.word_count / parser.total_chars;
        printf("Number ratio: %.2f\n", number_ratio);
        printf("Word ratio: %.2f\n", word_ratio);
    }
    
    return EXIT_SUCCESS;
}