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

static void state_start(parser_context_t *ctx);
static void state_reading_word(parser_context_t *ctx);
static void state_reading_number(parser_context_t *ctx);
static void state_error(parser_context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_context_t ctx;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    ctx.current_state = STATE_START;
    ctx.word_count = 0;
    ctx.number_count = 0;
    ctx.input_ptr = input_buffer;
    ctx.input_end = input_buffer + input_len;
    
    while (ctx.current_state != STATE_END && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_START:
                state_start(&ctx);
                break;
            case STATE_READING_WORD:
                state_reading_word(&ctx);
                break;
            case STATE_READING_NUMBER:
                state_reading_number(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            default:
                ctx.current_state = STATE_ERROR;
                break;
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error parsing input\n");
        return 1;
    }
    
    printf("Words: %d\nNumbers: %d\n", ctx.word_count, ctx.number_count);
    return 0;
}

static void state_start(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) {
        ctx->current_state = STATE_END;
        return;
    }
    
    char current_char = *ctx->input_ptr;
    
    if (isspace((unsigned char)current_char)) {
        ctx->input_ptr++;
    } else if (isalpha((unsigned char)current_char)) {
        ctx->current_state = STATE_READING_WORD;
        ctx->word_count++;
    } else if (isdigit((unsigned char)current_char)) {
        ctx->current_state = STATE_READING_NUMBER;
        ctx->number_count++;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

static void state_reading_word(parser_context_t *ctx) {
    while (ctx->input_ptr < ctx->input_end) {
        char current_char = *ctx->input_ptr;
        
        if (isalpha((unsigned char)current_char)) {
            ctx->input_ptr++;
        } else if (isspace((unsigned char)current_char)) {
            ctx->current_state = STATE_START;
            ctx->input_ptr++;
            break;
        } else {
            ctx->current_state = STATE_ERROR;
            break;
        }
    }
    
    if (ctx->input_ptr >= ctx->input_end && ctx->current_state == STATE_READING_WORD) {
        ctx->current_state = STATE_END;
    }
}

static void state_reading_number(parser_context_t *ctx) {
    while (ctx->input_ptr < ctx->input_end) {
        char current_char = *ctx->input_ptr;
        
        if (isdigit((unsigned char)current_char)) {
            ctx->input_ptr++;
        } else if (isspace((unsigned char)current_char)) {
            ctx->current_state = STATE_START;
            ctx->input_ptr++;
            break;
        } else {
            ctx->current_state = STATE_ERROR;
            break;
        }
    }
    
    if (ctx->input_ptr >= ctx->input_end && ctx->current_state == STATE_READING_NUMBER) {
        ctx->current_state = STATE_END;
    }
}

static void state_error(parser_context_t *ctx) {
    ctx->current_state = STATE_ERROR;
}