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
} state_t;

typedef struct {
    state_t current;
    int word_count;
    int number_count;
    char buffer[MAX_INPUT];
    size_t buf_pos;
} context_t;

static state_t process_start(char c, context_t *ctx);
static state_t process_reading_word(char c, context_t *ctx);
static state_t process_reading_number(char c, context_t *ctx);
static state_t process_end(char c, context_t *ctx);
static state_t process_error(char c, context_t *ctx);

int main(void) {
    context_t ctx = {0};
    ctx.current = STATE_START;
    
    char input[MAX_INPUT];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        return EXIT_FAILURE;
    }
    
    char *ptr = input;
    char *end = ptr + input_len;
    
    while (ptr < end && ctx.current != STATE_END && ctx.current != STATE_ERROR) {
        char current_char = *ptr;
        
        switch (ctx.current) {
            case STATE_START:
                ctx.current = process_start(current_char, &ctx);
                break;
            case STATE_READING_WORD:
                ctx.current = process_reading_word(current_char, &ctx);
                break;
            case STATE_READING_NUMBER:
                ctx.current = process_reading_number(current_char, &ctx);
                break;
            case STATE_END:
                ctx.current = process_end(current_char, &ctx);
                break;
            case STATE_ERROR:
                ctx.current = process_error(current_char, &ctx);
                break;
        }
        
        if (ctx.buf_pos < sizeof(ctx.buffer) - 1) {
            ctx.buffer[ctx.buf_pos] = current_char;
            ctx.buf_pos++;
        }
        
        ptr++;
    }
    
    if (ctx.current == STATE_READING_WORD) {
        ctx.word_count++;
    } else if (ctx.current == STATE_READING_NUMBER) {
        ctx.number_count++;
    }
    
    if (ctx.buf_pos < sizeof(ctx.buffer)) {
        ctx.buffer[ctx.buf_pos] = '\0';
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Numbers: %d\n", ctx.number_count);
    
    return EXIT_SUCCESS;
}

static state_t process_start(char c, context_t *ctx) {
    if (isspace((unsigned char)c)) {
        return STATE_START;
    } else if (isalpha((unsigned char)c)) {
        return STATE_READING_WORD;
    } else if (isdigit((unsigned char)c)) {
        return STATE_READING_NUMBER;
    } else if (c == '\0' || c == '\n') {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

static state_t process_reading_word(char c, context_t *ctx) {
    if (isalpha((unsigned char)c)) {
        return STATE_READING_WORD;
    } else if (isspace((unsigned char)c)) {
        ctx->word_count++;
        return STATE_START;
    } else if (c == '\0' || c == '\n') {
        ctx->word_count++;
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

static state_t process_reading_number(char c, context_t *ctx) {
    if (isdigit((unsigned char)c)) {
        return STATE_READING_NUMBER;
    } else if (isspace((unsigned char)c)) {
        ctx->number_count++;
        return STATE_START;
    } else if (c == '\0' || c == '\n') {
        ctx->number_count++;
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

static state_t process_end(char c, context_t *ctx) {
    return STATE_END;
}

static state_t process_error(char c, context_t *ctx) {
    return STATE_ERROR;
}