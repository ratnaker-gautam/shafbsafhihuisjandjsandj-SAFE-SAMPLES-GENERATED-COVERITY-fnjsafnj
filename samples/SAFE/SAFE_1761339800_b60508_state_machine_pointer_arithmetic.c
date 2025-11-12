//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    char *input_ptr;
    char *input_end;
    int number_count;
    int word_count;
} parser_context_t;

static void process_start_state(parser_context_t *ctx);
static void process_reading_number(parser_context_t *ctx);
static void process_reading_word(parser_context_t *ctx);
static void process_error_state(parser_context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_context_t ctx;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return EXIT_SUCCESS;
    }
    
    ctx.current_state = STATE_START;
    ctx.input_ptr = input_buffer;
    ctx.input_end = input_buffer + input_len;
    ctx.number_count = 0;
    ctx.word_count = 0;
    
    while (ctx.current_state != STATE_END && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_START:
                process_start_state(&ctx);
                break;
            case STATE_READING_NUMBER:
                process_reading_number(&ctx);
                break;
            case STATE_READING_WORD:
                process_reading_word(&ctx);
                break;
            case STATE_ERROR:
                process_error_state(&ctx);
                break;
            default:
                ctx.current_state = STATE_ERROR;
                break;
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Parser encountered an error\n");
        return EXIT_FAILURE;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return EXIT_SUCCESS;
}

static void process_start_state(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) {
        ctx->current_state = STATE_END;
        return;
    }
    
    char current_char = *ctx->input_ptr;
    
    if (isdigit(current_char)) {
        ctx->current_state = STATE_READING_NUMBER;
        ctx->number_count++;
    } else if (isalpha(current_char)) {
        ctx->current_state = STATE_READING_WORD;
        ctx->word_count++;
    } else if (isspace(current_char)) {
        ctx->input_ptr++;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

static void process_reading_number(parser_context_t *ctx) {
    while (ctx->input_ptr < ctx->input_end && isdigit(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    
    if (ctx->input_ptr >= ctx->input_end) {
        ctx->current_state = STATE_END;
    } else if (isspace(*ctx->input_ptr)) {
        ctx->current_state = STATE_START;
        ctx->input_ptr++;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

static void process_reading_word(parser_context_t *ctx) {
    while (ctx->input_ptr < ctx->input_end && isalpha(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    
    if (ctx->input_ptr >= ctx->input_end) {
        ctx->current_state = STATE_END;
    } else if (isspace(*ctx->input_ptr)) {
        ctx->current_state = STATE_START;
        ctx->input_ptr++;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

static void process_error_state(parser_context_t *ctx) {
    ctx->current_state = STATE_END;
}