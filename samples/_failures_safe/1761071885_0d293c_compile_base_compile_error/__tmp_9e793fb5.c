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

token_type_t get_token_type(char c) {
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

void init_context(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->token_len = 0;
    memset(ctx->current_token, 0, sizeof(ctx->current_token));
}

int process_token(parser_context_t *ctx, token_type_t token_type, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            if (token_type == TOKEN_DIGIT) {
                ctx->current_state = STATE_READING_NUMBER;
                if (ctx->token_len < MAX_INPUT_LEN - 1) {
                    ctx->current_token[ctx->token_len++] = c;
                }
            } else if (token_type == TOKEN_LETTER) {
                ctx->current_state = STATE_READING_WORD;
                if (ctx->token_len < MAX_INPUT_LEN - 1) {
                    ctx->current_token[ctx->token_len++] = c;
                }
            } else if (token_type == TOKEN_SPACE) {
                ctx->current_state = STATE_START;
            } else if (token_type == TOKEN_EOF) {
                ctx->current_state = STATE_DONE;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (token_type == TOKEN_DIGIT) {
                if (ctx->token_len < MAX_INPUT_LEN - 1) {
                    ctx->current_token[ctx->token_len++] = c;
                }
            } else if (token_type == TOKEN_SPACE || token_type == TOKEN_EOF) {
                ctx->number_count++;
                ctx->token_len = 0;
                memset(ctx->current_token, 0, sizeof(ctx->current_token));
                ctx->current_state = (token_type == TOKEN_EOF) ? STATE_DONE : STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_WORD:
            if (token_type == TOKEN_LETTER) {
                if (ctx->token_len < MAX_INPUT_LEN - 1) {
                    ctx->current_token[ctx->token_len++] = c;
                }
            } else if (token_type == TOKEN_SPACE || token_type == TOKEN_EOF) {
                ctx->word_count++;
                ctx->token_len = 0;
                memset(ctx->current_token, 0, sizeof(ctx->current_token));
                ctx->current_state = (token_type == TOKEN_EOF) ? STATE_DONE : STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            return -1;
            
        case STATE_DONE:
            break;
    }
    return 0;
}

int parse_input(const char *input, int *numbers, int *words) {
    parser_context_t ctx;
    init_context(&ctx);
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        token_type_t token_type = get_token_type(c);
        
        if (process_token(&ctx, token_type, c) != 0) {
            return -1;
        }
        
        if (ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (ctx.current_state != STATE_DONE && ctx.current_state != STATE_START) {
        return -1;
    }
    
    *numbers = ctx.number_count;
    *words = ctx.word_count;
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int numbers, words;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (parse_input(input, &numbers, &words) != 0) {
        fprintf(stderr, "Error parsing input\n