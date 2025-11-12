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
    char buffer[MAX_INPUT_LEN + 1];
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->total_chars = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static parser_state_t handle_start_state(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_END;
    
    if (isdigit(c)) {
        ctx->buffer[ctx->total_chars++] = c;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->total_chars++] = c;
        return STATE_READING_WORD;
    } else if (c == ' ' || c == '\t') {
        return STATE_START;
    } else {
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_END;
    
    if (isdigit(c)) {
        if (ctx->total_chars < MAX_INPUT_LEN) {
            ctx->buffer[ctx->total_chars++] = c;
        }
        return STATE_READING_NUMBER;
    } else if (c == ' ' || c == '\t' || c == '\0') {
        ctx->number_count++;
        ctx->total_chars = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    } else {
        ctx->total_chars = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_END;
    
    if (isalpha(c)) {
        if (ctx->total_chars < MAX_INPUT_LEN) {
            ctx->buffer[ctx->total_chars++] = c;
        }
        return STATE_READING_WORD;
    } else if (c == ' ' || c == '\t' || c == '\0') {
        ctx->word_count++;
        ctx->total_chars = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    } else {
        ctx->total_chars = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_SKIPPING;
    }
}

static parser_state_t handle_skipping_state(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_END;
    
    if (c == ' ' || c == '\t' || c == '\0') {
        return STATE_START;
    }
    return STATE_SKIPPING;
}

static void process_character(char c, parser_context_t *ctx) {
    if (ctx == NULL) return;
    
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
}

static void finalize_parsing(parser_context_t *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_READING_NUMBER) {
        ctx->number_count++;
    } else if (ctx->current_state == STATE_READING_WORD) {
        ctx->word_count++;
    }
    ctx->current_state = STATE_END;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0) return 0;
    if (len > MAX_INPUT_LEN) return 0;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    parser_context_t parser;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EX