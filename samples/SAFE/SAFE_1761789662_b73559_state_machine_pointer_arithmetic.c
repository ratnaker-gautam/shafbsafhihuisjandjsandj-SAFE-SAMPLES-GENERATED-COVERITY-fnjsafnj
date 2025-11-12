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
    char *input;
    size_t position;
    size_t word_count;
    size_t number_count;
} context_t;

state_t process_start(context_t *ctx);
state_t process_reading_word(context_t *ctx);
state_t process_reading_number(context_t *ctx);
state_t process_error(context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT + 1] = {0};
    context_t ctx = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && *(input_buffer + len - 1) == '\n') {
        *(input_buffer + len - 1) = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return EXIT_SUCCESS;
    }
    
    char *heap_input = malloc(len + 1);
    if (heap_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    memcpy(heap_input, input_buffer, len + 1);
    
    ctx.current = STATE_START;
    ctx.input = heap_input;
    ctx.position = 0;
    ctx.word_count = 0;
    ctx.number_count = 0;
    
    state_t (*state_handlers[MAX_STATES])(context_t *) = {
        process_start,
        process_reading_word,
        process_reading_number,
        NULL,
        process_error
    };
    
    while (ctx.current != STATE_END && ctx.current != STATE_ERROR) {
        if (ctx.position >= len) {
            ctx.current = STATE_END;
            break;
        }
        
        if (ctx.current >= MAX_STATES || state_handlers[ctx.current] == NULL) {
            ctx.current = STATE_ERROR;
            break;
        }
        
        state_t next_state = state_handlers[ctx.current](&ctx);
        ctx.current = next_state;
    }
    
    if (ctx.current == STATE_ERROR) {
        fprintf(stderr, "Processing error at position %zu\n", ctx.position);
    } else {
        printf("Words: %zu, Numbers: %zu\n", ctx.word_count, ctx.number_count);
    }
    
    free(heap_input);
    return ctx.current == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}

state_t process_start(context_t *ctx) {
    char current_char = *(ctx->input + ctx->position);
    
    if (isalpha(current_char)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (isdigit(current_char)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isspace(current_char)) {
        ctx->position++;
        return STATE_START;
    }
    
    return STATE_ERROR;
}

state_t process_reading_word(context_t *ctx) {
    char *current_ptr = ctx->input + ctx->position;
    
    while (*current_ptr && isalpha(*current_ptr)) {
        current_ptr++;
        ctx->position++;
        
        if (ctx->position >= MAX_INPUT) {
            return STATE_ERROR;
        }
    }
    
    return STATE_START;
}

state_t process_reading_number(context_t *ctx) {
    char *current_ptr = ctx->input + ctx->position;
    
    while (*current_ptr && isdigit(*current_ptr)) {
        current_ptr++;
        ctx->position++;
        
        if (ctx->position >= MAX_INPUT) {
            return STATE_ERROR;
        }
    }
    
    return STATE_START;
}

state_t process_error(context_t *ctx) {
    return STATE_ERROR;
}