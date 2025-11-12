//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int word_count;
    int number_count;
    char *input_ptr;
    char *input_end;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx, char *input) {
    ctx->current_state = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    ctx->input_ptr = input;
    ctx->input_end = input + strlen(input);
}

static parser_state_t handle_start(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) {
        return STATE_END;
    }
    
    char c = *ctx->input_ptr;
    if (isalpha(c)) {
        return STATE_READING_WORD;
    } else if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isspace(c)) {
        ctx->input_ptr++;
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

static parser_state_t handle_reading_word(parser_context_t *ctx) {
    ctx->word_count++;
    while (ctx->input_ptr < ctx->input_end && isalpha(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    return STATE_START;
}

static parser_state_t handle_reading_number(parser_context_t *ctx) {
    ctx->number_count++;
    while (ctx->input_ptr < ctx->input_end && isdigit(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    return STATE_START;
}

static int run_state_machine(parser_context_t *ctx) {
    while (ctx->current_state != STATE_END && ctx->current_state != STATE_ERROR) {
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start(ctx);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
    }
    
    return ctx->current_state == STATE_END ? 0 : 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input_buffer[0] == '\0')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    parser_context_t parser;
    initialize_parser(&parser, input_buffer);
    
    int result = run_state_machine(&parser);
    
    if (result == 0) {
        printf("Parsing successful:\n");
        printf("Words: %d\n", parser.word_count);
        printf("Numbers: %d\n", parser.number_count);
    } else {
        fprintf(stderr, "Parsing error at position %ld\n", parser.input_ptr - input_buffer);
        return 1;
    }
    
    return 0;
}