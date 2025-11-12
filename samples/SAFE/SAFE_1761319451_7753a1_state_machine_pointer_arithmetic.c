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
    return isalnum(c) || isspace(c) || c == '\0';
}

State process_start(Context *ctx) {
    char current = *(ctx->input + ctx->position);
    if (current == '\0') return FINISHED;
    if (isspace(current)) return START;
    if (isdigit(current)) return READING_NUMBER;
    if (isalpha(current)) return READING_WORD;
    return ERROR;
}

State process_reading_number(Context *ctx) {
    char current = *(ctx->input + ctx->position);
    if (current == '\0') {
        ctx->number_count++;
        return FINISHED;
    }
    if (isspace(current)) {
        ctx->number_count++;
        return START;
    }
    if (isdigit(current)) return READING_NUMBER;
    return ERROR;
}

State process_reading_word(Context *ctx) {
    char current = *(ctx->input + ctx->position);
    if (current == '\0') {
        ctx->word_count++;
        return FINISHED;
    }
    if (isspace(current)) {
        ctx->word_count++;
        return START;
    }
    if (isalpha(current)) return READING_WORD;
    return ERROR;
}

State (*state_handlers[STATE_COUNT])(Context *) = {
    process_start,
    process_reading_number,
    process_reading_word,
    NULL,
    NULL
};

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
    char user_input[MAX_INPUT];
    
    printf("Enter text (alphanumeric and spaces only, max %d chars): ", MAX_INPUT - 1);
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && *(user_input + input_len - 1) == '\n') {
        *(user_input + input_len - 1) = '\0';
        input_len--;
    }
    
    if (!validate_input(user_input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    Context ctx = {
        .current = START,
        .input = user_input,
        .position = 0,
        .number_count = 0,
        .word_count = 0
    };
    
    while (ctx.current != FINISHED && ctx.current != ERROR) {
        if (ctx.position >= MAX_INPUT) {
            ctx.current = ERROR;
            break;
        }
        
        State (*handler)(Context *) = state_handlers[ctx.current];
        if (handler == NULL) {
            ctx.current = ERROR;
            break;
        }
        
        State next_state = handler(&ctx);
        ctx.current = next_state;
        ctx.position++;
    }
    
    if (ctx.current == ERROR) {
        fprintf(stderr, "Error processing input at position %zu\n", ctx.position);
        return EXIT_FAILURE;
    }
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    
    return EXIT_SUCCESS;
}