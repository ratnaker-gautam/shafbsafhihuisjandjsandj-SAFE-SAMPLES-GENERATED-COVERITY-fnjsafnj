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

static parser_state_t process_token(parser_state_t state, token_type_t token, parser_context_t *ctx) {
    switch (state) {
        case STATE_START:
            if (token == TOKEN_DIGIT) {
                ctx->number_count++;
                return STATE_READING_NUMBER;
            } else if (token == TOKEN_LETTER) {
                ctx->word_count++;
                return STATE_READING_WORD;
            } else if (token == TOKEN_SPACE) {
                return STATE_START;
            } else if (token == TOKEN_EOF) {
                return STATE_END;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            if (token == TOKEN_DIGIT) {
                return STATE_READING_NUMBER;
            } else if (token == TOKEN_SPACE) {
                return STATE_START;
            } else if (token == TOKEN_EOF) {
                return STATE_END;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            if (token == TOKEN_LETTER) {
                return STATE_READING_WORD;
            } else if (token == TOKEN_SPACE) {
                return STATE_START;
            } else if (token == TOKEN_EOF) {
                return STATE_END;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        case STATE_END:
            return STATE_END;
    }
    
    return STATE_ERROR;
}

static void run_state_machine(const char *input) {
    parser_context_t ctx = {
        .current_state = STATE_START,
        .number_count = 0,
        .word_count = 0,
        .position = 0
    };
    
    while (ctx.current_state != STATE_END && ctx.current_state != STATE_ERROR) {
        if (ctx.position >= MAX_INPUT_LEN) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        token_type_t token = get_next_token(input, ctx.position);
        parser_state_t next_state = process_token(ctx.current_state, token, &ctx);
        
        if (next_state == STATE_ERROR) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        ctx.current_state = next_state;
        ctx.position++;
        
        if (ctx.position > 0 && ctx.position % 1000 == 0) {
            for (int i = 0; i < 1000; i++) {
                volatile int dummy = i * i;
            }
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Parser error at position %d\n", ctx.position);
    } else {
        printf("Parsing completed: %d numbers, %d words\n", ctx.number_count, ctx.word_count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if ((unsigned char)input[i] > 127) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    run_state_machine(input);
    
    return 0;
}