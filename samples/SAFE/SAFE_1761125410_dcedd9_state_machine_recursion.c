//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    PROCESS_WORD,
    END
};

struct Context {
    enum State current_state;
    char input[256];
    int position;
    int digit_count;
    int word_count;
};

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_whitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

void state_machine(struct Context *ctx, int depth) {
    if (depth > 1000) {
        return;
    }
    
    char current_char = ctx->input[ctx->position];
    
    switch (ctx->current_state) {
        case START:
            if (current_char == '\0') {
                ctx->current_state = END;
            } else if (is_whitespace(current_char)) {
                ctx->position++;
            } else {
                ctx->current_state = READ_CHAR;
            }
            break;
            
        case READ_CHAR:
            if (current_char == '\0') {
                ctx->current_state = END;
            } else if (is_digit(current_char)) {
                ctx->current_state = COUNT_DIGITS;
                ctx->digit_count = 0;
            } else if (is_whitespace(current_char)) {
                ctx->position++;
                ctx->current_state = START;
            } else {
                ctx->current_state = PROCESS_WORD;
            }
            break;
            
        case COUNT_DIGITS:
            if (is_digit(current_char)) {
                ctx->digit_count++;
                ctx->position++;
            } else {
                ctx->current_state = PROCESS_WORD;
            }
            break;
            
        case PROCESS_WORD:
            if (current_char == '\0' || is_whitespace(current_char)) {
                ctx->word_count++;
                ctx->current_state = START;
                if (current_char != '\0') {
                    ctx->position++;
                }
            } else {
                ctx->position++;
            }
            break;
            
        case END:
            return;
    }
    
    if (ctx->current_state != END) {
        state_machine(ctx, depth + 1);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    
    printf("Enter text: ");
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(ctx.input);
    if (len > 0 && ctx.input[len - 1] == '\n') {
        ctx.input[len - 1] = '\0';
    }
    
    if (strlen(ctx.input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    state_machine(&ctx, 0);
    
    printf("Words processed: %d\n", ctx.word_count);
    printf("Total digits found: %d\n", ctx.digit_count);
    
    return 0;
}