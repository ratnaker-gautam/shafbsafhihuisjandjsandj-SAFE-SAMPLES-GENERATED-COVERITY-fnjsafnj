//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    int current_number;
    int current_word_len;
} parser_context_t;

static token_type_t get_next_token(const char *input, int *pos) {
    if (input[*pos] == '\0') return TOKEN_EOF;
    if (isspace(input[*pos])) return TOKEN_SPACE;
    if (isdigit(input[*pos])) return TOKEN_DIGIT;
    if (isalpha(input[*pos])) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

static void state_machine_step(parser_context_t *ctx, token_type_t token, char current_char) {
    switch (ctx->current_state) {
        case STATE_START:
            if (token == TOKEN_DIGIT) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->current_number = current_char - '0';
            } else if (token == TOKEN_LETTER) {
                ctx->current_state = STATE_READING_WORD;
                ctx->current_word_len = 1;
            } else if (token == TOKEN_SPACE) {
                ctx->current_state = STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (token == TOKEN_DIGIT) {
                if (ctx->current_number <= (INT_MAX - (current_char - '0')) / 10) {
                    ctx->current_number = ctx->current_number * 10 + (current_char - '0');
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (token == TOKEN_SPACE) {
                ctx->current_state = STATE_START;
                ctx->number_count++;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_WORD:
            if (token == TOKEN_LETTER) {
                if (ctx->current_word_len < MAX_INPUT_LEN - 1) {
                    ctx->current_word_len++;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (token == TOKEN_SPACE) {
                ctx->current_state = STATE_START;
                ctx->word_count++;
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

static void process_input(const char *input) {
    parser_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_START;
    
    int pos = 0;
    int input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    while (ctx.current_state != STATE_ERROR && ctx.current_state != STATE_END) {
        token_type_t token = get_next_token(input, &pos);
        
        if (token == TOKEN_EOF) {
            if (ctx.current_state == STATE_READING_NUMBER) {
                ctx.number_count++;
            } else if (ctx.current_state == STATE_READING_WORD) {
                ctx.word_count++;
            }
            ctx.current_state = STATE_END;
            break;
        }
        
        state_machine_step(&ctx, token, input[pos]);
        
        if (token != TOKEN_EOF) {
            pos++;
            if (pos >= input_len) {
                if (ctx.current_state == STATE_READING_NUMBER) {
                    ctx.number_count++;
                } else if (ctx.current_state == STATE_READING_WORD) {
                    ctx.word_count++;
                }
                ctx.current_state = STATE_END;
            }
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Parse error at position %d\n", pos);
    } else {
        printf("Numbers: %d, Words: %d\n", ctx.number_count, ctx.word_count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Empty input\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}