//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    int position;
} parser_context_t;

static token_type_t get_next_token(const char *input, int pos) {
    if (pos >= MAX_INPUT_LEN || input[pos] == '\0') {
        return TOKEN_EOF;
    }
    
    if (isspace((unsigned char)input[pos])) {
        return TOKEN_SPACE;
    }
    
    if (isdigit((unsigned char)input[pos])) {
        return TOKEN_DIGIT;
    }
    
    if (isalpha((unsigned char)input[pos])) {
        return TOKEN_LETTER;
    }
    
    return TOKEN_INVALID;
}

static void process_state_machine(parser_context_t *ctx, const char *input) {
    token_type_t token;
    
    while (ctx->current_state != STATE_DONE && ctx->current_state != STATE_ERROR) {
        token = get_next_token(input, ctx->position);
        
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
                    ctx->current_state = STATE_DONE;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    ctx->position++;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    ctx->current_state = STATE_START;
                    if (token == TOKEN_SPACE) {
                        ctx->position++;
                    }
                } else {
                    ctx->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    ctx->position++;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    ctx->current_state = STATE_START;
                    if (token == TOKEN_SPACE) {
                        ctx->position++;
                    }
                } else {
                    ctx->current_state = STATE_ERROR;
                }
                break;
                
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
        
        if (ctx->position >= MAX_INPUT_LEN) {
            ctx->current_state = STATE_DONE;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    parser_context_t ctx;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_START;
    
    process_state_machine(&ctx, input);
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input format at position %d\n", ctx.position);
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}