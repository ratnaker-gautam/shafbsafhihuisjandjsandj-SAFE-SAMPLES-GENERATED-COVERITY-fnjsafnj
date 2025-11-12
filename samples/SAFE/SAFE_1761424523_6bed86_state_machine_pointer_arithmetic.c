//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current;
    int word_count;
    int number_count;
    char *input_ptr;
    char *input_end;
} parser_context_t;

static void state_start(parser_context_t *ctx);
static void state_reading_word(parser_context_t *ctx);
static void state_reading_number(parser_context_t *ctx);
static void state_error(parser_context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT + 1] = {0};
    parser_context_t context = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
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
    
    context.current = STATE_START;
    context.input_ptr = input_buffer;
    context.input_end = input_buffer + input_len;
    
    while (context.current != STATE_END && context.current != STATE_ERROR) {
        if (context.input_ptr >= context.input_end) {
            context.current = STATE_END;
            break;
        }
        
        switch (context.current) {
            case STATE_START:
                state_start(&context);
                break;
            case STATE_READING_WORD:
                state_reading_word(&context);
                break;
            case STATE_READING_NUMBER:
                state_reading_number(&context);
                break;
            case STATE_ERROR:
                state_error(&context);
                break;
            default:
                context.current = STATE_ERROR;
                break;
        }
    }
    
    if (context.current == STATE_ERROR) {
        fprintf(stderr, "Parser encountered an error\n");
        return EXIT_FAILURE;
    }
    
    printf("Words: %d\n", context.word_count);
    printf("Numbers: %d\n", context.number_count);
    
    return EXIT_SUCCESS;
}

static void state_start(parser_context_t *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL || ctx->input_end == NULL) {
        ctx->current = STATE_ERROR;
        return;
    }
    
    if (ctx->input_ptr >= ctx->input_end) {
        ctx->current = STATE_END;
        return;
    }
    
    char current_char = *ctx->input_ptr;
    
    if (isalpha(current_char)) {
        ctx->current = STATE_READING_WORD;
        ctx->word_count++;
    } else if (isdigit(current_char)) {
        ctx->current = STATE_READING_NUMBER;
        ctx->number_count++;
    } else if (isspace(current_char)) {
        ctx->input_ptr++;
    } else {
        ctx->current = STATE_ERROR;
    }
}

static void state_reading_word(parser_context_t *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL || ctx->input_end == NULL) {
        ctx->current = STATE_ERROR;
        return;
    }
    
    while (ctx->input_ptr < ctx->input_end && isalpha(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    
    ctx->current = STATE_START;
}

static void state_reading_number(parser_context_t *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL || ctx->input_end == NULL) {
        ctx->current = STATE_ERROR;
        return;
    }
    
    while (ctx->input_ptr < ctx->input_end && isdigit(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    
    ctx->current = STATE_START;
}

static void state_error(parser_context_t *ctx) {
    if (ctx != NULL) {
        ctx->current = STATE_ERROR;
    }
}