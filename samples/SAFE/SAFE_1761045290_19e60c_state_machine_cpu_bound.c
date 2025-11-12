//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

typedef enum {
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPACE,
    TOKEN_EOF,
    TOKEN_INVALID
} token_type_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    int position;
} parser_context_t;

static token_type_t get_next_token(const char *input, int pos) {
    if (pos >= MAX_INPUT_LEN || input[pos] == '\0') {
        return TOKEN_EOF;
    }
    
    unsigned char c = (unsigned char)input[pos];
    
    if (isspace(c)) {
        return TOKEN_SPACE;
    } else if (isdigit(c)) {
        return TOKEN_DIGIT;
    } else if (isalpha(c)) {
        return TOKEN_LETTER;
    } else {
        return TOKEN_INVALID;
    }
}

static void process_state_machine(parser_context_t *ctx, const char *input) {
    while (ctx->current_state != STATE_END && ctx->current_state != STATE_ERROR) {
        token_type_t token = get_next_token(input, ctx->position);
        
        switch (ctx->current_state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    ctx->current_state = STATE_READING_NUMBER;
                    ctx->number_count++;
                } else if (token == TOKEN_LETTER) {
                    ctx->current_state = STATE_READING_WORD;
                    ctx->word_count++;
                } else if (token == TOKEN_SPACE) {
                    ctx->position++;
                } else if (token == TOKEN_EOF) {
                    ctx->current_state = STATE_END;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    ctx->position++;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    ctx->current_state = STATE_START;
                    if (token == TOKEN_SPACE) ctx->position++;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    ctx->position++;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    ctx->current_state = STATE_START;
                    if (token == TOKEN_SPACE) ctx->position++;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
            case STATE_END:
                break;
        }
        
        if (ctx->position >= MAX_INPUT_LEN) {
            ctx->current_state = STATE_END;
        }
    }
}

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (!isalnum(c) && !isspace(c)) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text (alphanumeric and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    parser_context_t ctx = {
        .current_state = STATE_START,
        .number_count = 0,
        .word_count = 0,
        .position = 0
    };
    
    process_state_machine(&ctx, input);
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Parsing error at position %d\n", ctx.position);
        return EXIT_FAILURE;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return EXIT_SUCCESS;
}