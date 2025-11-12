//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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

static void init_parser(parser_context_t *ctx, char *input) {
    ctx->current_state = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    ctx->input_ptr = input;
    ctx->input_end = input + strlen(input);
}

static int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

static parser_state_t handle_start(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) return STATE_END;
    
    char c = *ctx->input_ptr;
    if (isalpha(c)) return STATE_READING_WORD;
    if (isdigit(c)) return STATE_READING_NUMBER;
    if (isspace(c)) {
        ctx->input_ptr++;
        return STATE_START;
    }
    return STATE_ERROR;
}

static parser_state_t handle_reading_word(parser_context_t *ctx) {
    while (ctx->input_ptr < ctx->input_end && isalpha(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    ctx->word_count++;
    return STATE_START;
}

static parser_state_t handle_reading_number(parser_context_t *ctx) {
    while (ctx->input_ptr < ctx->input_end && isdigit(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    ctx->number_count++;
    return STATE_START;
}

static int run_state_machine(parser_context_t *ctx) {
    int max_iterations = (int)(ctx->input_end - ctx->input_ptr) * 2;
    int iterations = 0;
    
    while (ctx->current_state != STATE_END && ctx->current_state != STATE_ERROR) {
        if (iterations++ > max_iterations) {
            ctx->current_state = STATE_ERROR;
            break;
        }
        
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
    
    return ctx->current_state == STATE_END;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text to parse (words and numbers): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!is_valid_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    parser_context_t parser;
    init_parser(&parser, input_buffer);
    
    if (!run_state_machine(&parser)) {
        fprintf(stderr, "Parsing error\n");
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Words found: %d\n", parser.word_count);
    printf("Numbers found: %d\n", parser.number_count);
    
    return 0;
}