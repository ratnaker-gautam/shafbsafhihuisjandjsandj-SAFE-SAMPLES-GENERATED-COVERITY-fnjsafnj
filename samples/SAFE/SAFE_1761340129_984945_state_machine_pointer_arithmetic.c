//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    FINISHED
} State;

typedef struct {
    State current;
    char *input_ptr;
    int number_count;
    int word_count;
} Context;

State handle_start(Context *ctx);
State handle_reading_number(Context *ctx);
State handle_reading_word(Context *ctx);
State handle_error(Context *ctx);
State handle_finished(Context *ctx);

State (*state_handlers[STATE_COUNT])(Context *) = {
    handle_start,
    handle_reading_number,
    handle_reading_word,
    handle_error,
    handle_finished
};

State handle_start(Context *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL) return ERROR;
    
    char c = *ctx->input_ptr;
    if (c == '\0') return FINISHED;
    if (isdigit(c)) return READING_NUMBER;
    if (isalpha(c)) return READING_WORD;
    if (isspace(c)) {
        ctx->input_ptr++;
        return START;
    }
    return ERROR;
}

State handle_reading_number(Context *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL) return ERROR;
    
    char c = *ctx->input_ptr;
    if (c == '\0') {
        ctx->number_count++;
        return FINISHED;
    }
    if (isdigit(c)) {
        ctx->input_ptr++;
        return READING_NUMBER;
    }
    if (isspace(c)) {
        ctx->number_count++;
        ctx->input_ptr++;
        return START;
    }
    return ERROR;
}

State handle_reading_word(Context *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL) return ERROR;
    
    char c = *ctx->input_ptr;
    if (c == '\0') {
        ctx->word_count++;
        return FINISHED;
    }
    if (isalpha(c)) {
        ctx->input_ptr++;
        return READING_WORD;
    }
    if (isspace(c)) {
        ctx->word_count++;
        ctx->input_ptr++;
        return START;
    }
    return ERROR;
}

State handle_error(Context *ctx) {
    return ERROR;
}

State handle_finished(Context *ctx) {
    return FINISHED;
}

int main(void) {
    char input_buffer[MAX_INPUT + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided\n");
        return EXIT_SUCCESS;
    }
    
    Context ctx;
    ctx.current = START;
    ctx.input_ptr = input_buffer;
    ctx.number_count = 0;
    ctx.word_count = 0;
    
    while (ctx.current != FINISHED && ctx.current != ERROR) {
        if (ctx.input_ptr >= input_buffer + sizeof(input_buffer)) {
            ctx.current = ERROR;
            break;
        }
        
        State next_state = state_handlers[ctx.current](&ctx);
        ctx.current = next_state;
    }
    
    if (ctx.current == ERROR) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return EXIT_SUCCESS;
}