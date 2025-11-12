//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
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
    char current_token[MAX_INPUT_LEN];
    int token_len;
} parser_context_t;

static token_type_t get_token_type(char c) {
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

static void reset_token(parser_context_t *ctx) {
    ctx->token_len = 0;
    ctx->current_token[0] = '\0';
}

static void add_to_token(parser_context_t *ctx, char c) {
    if (ctx->token_len < MAX_INPUT_LEN - 1) {
        ctx->current_token[ctx->token_len++] = c;
        ctx->current_token[ctx->token_len] = '\0';
    }
}

static void process_token(parser_context_t *ctx) {
    if (ctx->token_len > 0) {
        if (isdigit((unsigned char)ctx->current_token[0])) {
            ctx->number_count++;
        } else if (isalpha((unsigned char)ctx->current_token[0])) {
            ctx->word_count++;
        }
        reset_token(ctx);
    }
}

static parser_state_t handle_state_transition(parser_context_t *ctx, token_type_t token) {
    switch (ctx->current_state) {
        case STATE_START:
            if (token == TOKEN_DIGIT) {
                add_to_token(ctx, ' ');
                ctx->current_state = STATE_READING_NUMBER;
            } else if (token == TOKEN_LETTER) {
                add_to_token(ctx, ' ');
                ctx->current_state = STATE_READING_WORD;
            } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                return STATE_START;
            } else {
                return STATE_ERROR;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (token == TOKEN_DIGIT) {
                add_to_token(ctx, ' ');
            } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                process_token(ctx);
                ctx->current_state = (token == TOKEN_EOF) ? STATE_DONE : STATE_START;
            } else {
                return STATE_ERROR;
            }
            break;
            
        case STATE_READING_WORD:
            if (token == TOKEN_LETTER) {
                add_to_token(ctx, ' ');
            } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                process_token(ctx);
                ctx->current_state = (token == TOKEN_EOF) ? STATE_DONE : STATE_START;
            } else {
                return STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
        case STATE_DONE:
            break;
    }
    
    return ctx->current_state;
}

static int parse_input(const char *input, parser_context_t *ctx) {
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    reset_token(ctx);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        token_type_t token = get_token_type(c);
        
        parser_state_t new_state = handle_state_transition(ctx, token);
        if (new_state == STATE_ERROR) {
            return -1;
        }
        
        if (token != TOKEN_SPACE && token != TOKEN_EOF && token != TOKEN_INVALID) {
            add_to_token(ctx, c);
        }
        
        if (new_state == STATE_DONE) {
            break;
        }
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    parser_context_t ctx;
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (parse_input(input, &ctx) != 0) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}