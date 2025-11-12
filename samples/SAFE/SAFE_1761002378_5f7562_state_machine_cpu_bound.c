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
    char input[MAX_INPUT_LEN + 1];
} parser_context_t;

static token_type_t get_next_token(parser_context_t *ctx) {
    if (ctx->position >= MAX_INPUT_LEN || ctx->input[ctx->position] == '\0') {
        return TOKEN_EOF;
    }
    
    char c = ctx->input[ctx->position];
    
    if (isspace((unsigned char)c)) {
        return TOKEN_SPACE;
    } else if (isdigit((unsigned char)c)) {
        return TOKEN_DIGIT;
    } else if (isalpha((unsigned char)c)) {
        return TOKEN_LETTER;
    } else {
        return TOKEN_INVALID;
    }
}

static void process_state_machine(parser_context_t *ctx) {
    while (ctx->current_state != STATE_END && ctx->current_state != STATE_ERROR) {
        token_type_t token = get_next_token(ctx);
        
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
                
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
    }
}

static int read_input_safe(char *buffer, size_t max_len) {
    if (max_len == 0) return 0;
    
    size_t i = 0;
    int c;
    
    while (i < max_len - 1) {
        c = getchar();
        if (c == EOF || c == '\n') break;
        buffer[i++] = (char)c;
    }
    
    buffer[i] = '\0';
    return 1;
}

int main(void) {
    parser_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (!read_input_safe(ctx.input, sizeof(ctx.input))) {
        printf("Error reading input\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input format at position %d\n", ctx.position);
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}