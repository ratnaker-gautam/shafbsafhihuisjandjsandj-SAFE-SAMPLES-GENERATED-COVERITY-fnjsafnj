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
    char *input;
    size_t position;
    int number_count;
    int word_count;
} Context;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

State handle_start(Context *ctx) {
    char current = *(ctx->input + ctx->position);
    if (current == '\0') return FINISHED;
    if (isspace((unsigned char)current)) return START;
    if (isdigit((unsigned char)current)) return READING_NUMBER;
    if (isalpha((unsigned char)current)) return READING_WORD;
    return ERROR;
}

State handle_reading_number(Context *ctx) {
    char current = *(ctx->input + ctx->position);
    if (current == '\0') {
        ctx->number_count++;
        return FINISHED;
    }
    if (isspace((unsigned char)current)) {
        ctx->number_count++;
        return START;
    }
    if (isdigit((unsigned char)current)) return READING_NUMBER;
    return ERROR;
}

State handle_reading_word(Context *ctx) {
    char current = *(ctx->input + ctx->position);
    if (current == '\0') {
        ctx->word_count++;
        return FINISHED;
    }
    if (isspace((unsigned char)current)) {
        ctx->word_count++;
        return START;
    }
    if (isalpha((unsigned char)current)) return READING_WORD;
    return ERROR;
}

State process_next(Context *ctx) {
    if (ctx->position >= MAX_INPUT || *(ctx->input + ctx->position) == '\0') {
        return FINISHED;
    }
    
    char current = *(ctx->input + ctx->position);
    if (!is_valid_char(current)) {
        return ERROR;
    }
    
    switch (ctx->current) {
        case START:
            return handle_start(ctx);
        case READING_NUMBER:
            return handle_reading_number(ctx);
        case READING_WORD:
            return handle_reading_word(ctx);
        case ERROR:
        case FINISHED:
            return ctx->current;
    }
    return ERROR;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(*(input + i))) return 0;
    }
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT + 1] = {0};
    
    printf("Enter text (alphanumeric and spaces only, max %d chars): ", MAX_INPUT);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && *(input_buffer + len - 1) == '\n') {
        *(input_buffer + len - 1) = '\0';
    }
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    Context ctx = {
        .current = START,
        .input = input_buffer,
        .position = 0,
        .number_count = 0,
        .word_count = 0
    };
    
    while (ctx.current != FINISHED && ctx.current != ERROR) {
        ctx.current = process_next(&ctx);
        if (ctx.current != FINISHED && ctx.current != ERROR) {
            ctx.position++;
        }
    }
    
    if (ctx.current == ERROR) {
        fprintf(stderr, "Error processing input at position %zu\n", ctx.position);
        return 1;
    }
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    
    return 0;
}