//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    int total_chars;
} parser_context_t;

static void initialize_context(parser_context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->total_chars = 0;
}

static parser_state_t handle_start_state(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (c == ' ' || c == '\t') {
        return STATE_START;
    } else if (c == '\0' || c == '\n') {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

static parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (c == ' ' || c == '\t') {
        return STATE_START;
    } else if (c == '\0' || c == '\n') {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

static parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isalpha(c)) {
        return STATE_READING_WORD;
    } else if (c == ' ' || c == '\t') {
        return STATE_START;
    } else if (c == '\0' || c == '\n') {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

static void process_input(const char *input, parser_context_t *ctx) {
    if (input == NULL || ctx == NULL) return;
    
    size_t len = strnlen(input, MAX_INPUT_LEN);
    if (len >= MAX_INPUT_LEN) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        ctx->total_chars++;
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start_state(c, ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(c, ctx);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(c, ctx);
                break;
            case STATE_ERROR:
            case STATE_END:
                return;
            default:
                ctx->current_state = STATE_ERROR;
                return;
        }
        
        if (ctx->current_state == STATE_ERROR || ctx->current_state == STATE_END) {
            break;
        }
    }
}

static void print_results(const parser_context_t *ctx) {
    if (ctx == NULL) return;
    
    printf("Parser completed with state: ");
    switch (ctx->current_state) {
        case STATE_END:
            printf("SUCCESS\n");
            break;
        case STATE_ERROR:
            printf("ERROR\n");
            break;
        default:
            printf("UNEXPECTED\n");
            return;
    }
    
    if (ctx->current_state == STATE_END) {
        printf("Numbers found: %d\n", ctx->number_count);
        printf("Words found: %d\n", ctx->word_count);
        printf("Total characters processed: %d\n", ctx->total_chars);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    parser_context_t context;
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strnlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    initialize_context(&context);
    process_input(input, &context);
    print_results(&context);
    
    return (context.current_state == STATE_END) ? EXIT_SUCCESS : EXIT_FAILURE;
}