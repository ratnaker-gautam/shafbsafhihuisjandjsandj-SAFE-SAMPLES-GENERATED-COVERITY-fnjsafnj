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
    STATE_READING_SYMBOL,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    int symbol_count;
    int position;
} parser_context_t;

static void initialize_context(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->symbol_count = 0;
    ctx->position = 0;
}

static parser_state_t handle_start(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (ispunct(c)) {
        ctx->symbol_count++;
        return STATE_READING_SYMBOL;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

static parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isspace(c)) {
        return STATE_START;
    } else if (ispunct(c)) {
        ctx->symbol_count++;
        return STATE_READING_SYMBOL;
    } else {
        return STATE_ERROR;
    }
}

static parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (isalnum(c)) {
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        return STATE_START;
    } else if (ispunct(c)) {
        ctx->symbol_count++;
        return STATE_READING_SYMBOL;
    } else {
        return STATE_ERROR;
    }
}

static parser_state_t handle_reading_symbol(char c, parser_context_t *ctx) {
    if (ispunct(c)) {
        return STATE_READING_SYMBOL;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

static int process_input(const char *input, parser_context_t *ctx) {
    if (input == NULL || ctx == NULL) {
        return -1;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    initialize_context(ctx);
    
    for (ctx->position = 0; ctx->position < (int)len; ctx->position++) {
        char current_char = input[ctx->position];
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start(current_char, ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(current_char, ctx);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(current_char, ctx);
                break;
            case STATE_READING_SYMBOL:
                ctx->current_state = handle_reading_symbol(current_char, ctx);
                break;
            case STATE_ERROR:
                return -1;
            default:
                return -1;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            return -1;
        }
    }
    
    return 0;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    parser_context_t context;
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (process_input(input_buffer, &context) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Parsing results:\n");
    printf("Numbers found: %d\n", context.number_count);
    printf("Words found: %d\n", context.word_count);
    printf("Symbol sequences found: %d\n", context.symbol_count);
    
    return EXIT_SUCCESS;
}