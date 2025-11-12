//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_SKIPPING,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    char *input_ptr;
    char *input_end;
    int word_count;
    int number_count;
} parser_context_t;

void initialize_parser(parser_context_t *ctx, char *input) {
    if (!ctx || !input) return;
    
    ctx->current_state = STATE_START;
    ctx->input_ptr = input;
    ctx->input_end = input + strlen(input);
    ctx->word_count = 0;
    ctx->number_count = 0;
}

parser_state_t handle_start_state(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) return STATE_END;
    
    char current = *ctx->input_ptr;
    
    if (isalpha(current)) {
        return STATE_READING_WORD;
    } else if (isdigit(current)) {
        return STATE_READING_NUMBER;
    } else {
        ctx->input_ptr++;
        return STATE_SKIPPING;
    }
}

parser_state_t handle_reading_word(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) {
        ctx->word_count++;
        return STATE_END;
    }
    
    char current = *ctx->input_ptr;
    
    if (isalnum(current)) {
        ctx->input_ptr++;
        return STATE_READING_WORD;
    } else {
        ctx->word_count++;
        ctx->input_ptr++;
        return STATE_SKIPPING;
    }
}

parser_state_t handle_reading_number(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) {
        ctx->number_count++;
        return STATE_END;
    }
    
    char current = *ctx->input_ptr;
    
    if (isdigit(current)) {
        ctx->input_ptr++;
        return STATE_READING_NUMBER;
    } else {
        ctx->number_count++;
        ctx->input_ptr++;
        return STATE_SKIPPING;
    }
}

parser_state_t handle_skipping_state(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) return STATE_END;
    
    char current = *ctx->input_ptr;
    
    if (isalpha(current)) {
        return STATE_READING_WORD;
    } else if (isdigit(current)) {
        return STATE_READING_NUMBER;
    } else {
        ctx->input_ptr++;
        return STATE_SKIPPING;
    }
}

void run_state_machine(parser_context_t *ctx) {
    if (!ctx) return;
    
    while (ctx->current_state != STATE_END) {
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start_state(ctx);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(ctx);
                break;
            case STATE_SKIPPING:
                ctx->current_state = handle_skipping_state(ctx);
                break;
            case STATE_END:
                break;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT + 1] = {0};
    parser_context_t parser;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    initialize_parser(&parser, input_buffer);
    run_state_machine(&parser);
    
    printf("Words found: %d\n", parser.word_count);
    printf("Numbers found: %d\n", parser.number_count);
    
    return 0;
}