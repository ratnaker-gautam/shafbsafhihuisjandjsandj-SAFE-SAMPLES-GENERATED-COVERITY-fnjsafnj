//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

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
    char current_token[MAX_INPUT_LEN];
    int token_len;
} parser_context_t;

static void initialize_context(parser_context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->token_len = 0;
    memset(ctx->current_token, 0, sizeof(ctx->current_token));
}

static int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

static void process_character(parser_context_t *ctx, char c) {
    if (ctx == NULL || !is_valid_input_char(c)) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->current_token[ctx->token_len++] = c;
            } else if (isalpha(c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->current_token[ctx->token_len++] = c;
            } else if (isspace(c)) {
                ctx->current_state = STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->token_len < MAX_INPUT_LEN - 1) {
                    ctx->current_token[ctx->token_len++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->number_count++;
                ctx->token_len = 0;
                memset(ctx->current_token, 0, sizeof(ctx->current_token));
                ctx->current_state = STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_WORD:
            if (isalpha(c)) {
                if (ctx->token_len < MAX_INPUT_LEN - 1) {
                    ctx->current_token[ctx->token_len++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->word_count++;
                ctx->token_len = 0;
                memset(ctx->current_token, 0, sizeof(ctx->current_token));
                ctx->current_state = STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_ERROR:
            break;

        case STATE_END:
            break;
    }
}

static int validate_input_string(const char *input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_input_char(input[i])) {
            return 0;
        }
    }
    return 1;
}

static void run_state_machine(parser_context_t *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return;
    
    size_t len = strlen(input);
    for (size_t i = 0; i <= len; i++) {
        if (ctx->current_state == STATE_ERROR) break;
        process_character(ctx, input[i]);
    }
    
    if (ctx->current_state != STATE_ERROR) {
        ctx->current_state = STATE_END;
    }
}

static void print_results(const parser_context_t *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
    } else {
        printf("Parsing completed successfully\n");
        printf("Numbers found: %d\n", ctx->number_count);
        printf("Words found: %d\n", ctx->word_count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    parser_context_t context;
    
    printf("Enter text (alphanumeric characters and spaces only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input_string(input)) {
        printf("Error: Invalid input characters or length\n");
        return EXIT_FAILURE;
    }
    
    initialize_context(&context);
    run_state_machine(&context, input);
    print_results(&context);
    
    return (context.current_state == STATE_ERROR) ? EXIT_FAILURE : EXIT_SUCCESS;
}