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

void process_state_machine(parser_context_t *ctx, char input_char) {
    token_type_t token_type = get_token_type(input_char);
    
    switch (ctx->current_state) {
        case STATE_START:
            if (token_type == TOKEN_DIGIT) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->current_token[ctx->token_len++] = input_char;
            } else if (token_type == TOKEN_LETTER) {
                ctx->current_state = STATE_READING_WORD;
                ctx->current_token[ctx->token_len++] = input_char;
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
                    ctx->current_token[ctx->token_len++] = input_char;
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
                    ctx->current_token[ctx->token_len++] = input_char;
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
            break;
            
        case STATE_DONE:
            break;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
    parser_context_t context;
    
    init_context(&context);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= strlen(input_buffer); i++) {
        process_state_machine(&context, input_buffer[i]);
        if (context.current_state == STATE_ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
        if (context.current_state == STATE_DONE) {
            break;
        }
    }
    
    printf("Numbers found: %d\n", context.number_count);
    printf("Words found: %d\n", context.word_count);
    
    return 0;
}