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

static int is_valid_input_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c) || c == '\0';
}

static parser_state_t handle_start_state(char c, parser_context_t *ctx) {
    if (isdigit((unsigned char)c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha((unsigned char)c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (isspace((unsigned char)c)) {
        return STATE_START;
    }
    return STATE_SKIPPING;
}

static parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (isdigit((unsigned char)c)) {
        return STATE_READING_NUMBER;
    } else if (isalpha((unsigned char)c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (isspace((unsigned char)c)) {
        return STATE_START;
    }
    return STATE_SKIPPING;
}

static parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (isalnum((unsigned char)c)) {
        return STATE_READING_WORD;
    } else if (isspace((unsigned char)c)) {
        return STATE_START;
    }
    return STATE_SKIPPING;
}

static parser_state_t handle_skipping_state(char c, parser_context_t *ctx) {
    if (isspace((unsigned char)c)) {
        return STATE_START;
    }
    return STATE_SKIPPING;
}

static void process_character(char c, parser_context_t *ctx) {
    if (!is_valid_input_char(c)) {
        ctx->current_state = STATE_SKIPPING;
        return;
    }

    switch (ctx->current_state) {
        case STATE_START:
            ctx->current_state = handle_start_state(c, ctx);
            break;
        case STATE_READING_NUMBER:
            ctx->current_state = handle_reading_number(c, ctx);
            break;
        case STATE_READING_WORD:
            ctx->current_state = handle_reading_word(c, ctx);
            break;
        case STATE_SKIPPING:
            ctx->current_state = handle_skipping_state(c, ctx);
            break;
        case STATE_END:
            break;
    }
    
    ctx->total_chars++;
}

static void run_state_machine(const char *input, parser_context_t *ctx) {
    size_t len = strnlen(input, MAX_INPUT_LEN);
    if (len >= MAX_INPUT_LEN) {
        return;
    }

    for (size_t i = 0; i < len; i++) {
        process_character(input[i], ctx);
    }
    
    ctx->current_state = STATE_END;
}

static int get_user_input(char *buffer, size_t buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    printf("Enter text to analyze (max %zu chars): ", buffer_size - 1);
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return 0;
    }

    size_t len = strnlen(buffer, buffer_size);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    for (size_t i = 0; i < len; i++) {
        if (!is_valid_input_char(buffer[i])) {
            return 0;
        }
    }

    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    parser_context_t parser;
    
    if (!get_user_input(input_buffer, sizeof(input_buffer))) {
        fprintf(stderr, "Invalid input detected\n");
        return EXIT_FAILURE;
    }

    initialize_parser(&parser);
    run_state_machine(input_buffer, &parser);

    printf("Analysis results:\n");
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    printf("Total characters processed: %d\n", parser.total_chars);

    return EXIT_SUCCESS;
}