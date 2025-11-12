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
    STATE_ERROR,
    STATE_END
} state_t;

typedef struct {
    state_t current;
    char *input;
    size_t pos;
    size_t word_count;
    size_t number_count;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading_word(context_t *ctx);
state_t handle_reading_number(context_t *ctx);
state_t handle_error(context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    context_t ctx;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && *(input_buffer + len - 1) == '\n') {
        *(input_buffer + len - 1) = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    ctx.input = input_buffer;
    ctx.pos = 0;
    ctx.word_count = 0;
    ctx.number_count = 0;
    ctx.current = STATE_START;
    
    state_t (*handlers[])(context_t*) = {
        handle_start,
        handle_reading_word,
        handle_reading_number,
        handle_error
    };
    
    while (ctx.current != STATE_END && ctx.current != STATE_ERROR) {
        if (ctx.pos >= len) {
            ctx.current = STATE_END;
            break;
        }
        
        if (ctx.current >= 0 && ctx.current < 4) {
            ctx.current = handlers[ctx.current](&ctx);
        } else {
            ctx.current = STATE_ERROR;
        }
    }
    
    if (ctx.current == STATE_ERROR) {
        printf("Processing error at position %zu\n", ctx.pos);
        return 1;
    }
    
    printf("Words: %zu, Numbers: %zu\n", ctx.word_count, ctx.number_count);
    return 0;
}

state_t handle_start(context_t *ctx) {
    char current_char = *(ctx->input + ctx->pos);
    
    if (isalpha(current_char)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (isdigit(current_char)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isspace(current_char)) {
        ctx->pos++;
        return STATE_START;
    }
    
    return STATE_ERROR;
}

state_t handle_reading_word(context_t *ctx) {
    char current_char = *(ctx->input + ctx->pos);
    
    if (isalpha(current_char)) {
        ctx->pos++;
        return STATE_READING_WORD;
    } else if (isspace(current_char)) {
        ctx->pos++;
        return STATE_START;
    }
    
    return STATE_ERROR;
}

state_t handle_reading_number(context_t *ctx) {
    char current_char = *(ctx->input + ctx->pos);
    
    if (isdigit(current_char)) {
        ctx->pos++;
        return STATE_READING_NUMBER;
    } else if (isspace(current_char)) {
        ctx->pos++;
        return STATE_START;
    }
    
    return STATE_ERROR;
}

state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}