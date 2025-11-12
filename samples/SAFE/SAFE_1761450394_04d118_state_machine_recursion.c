//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
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

int is_space(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

void process_state_machine(struct Context* ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        ctx->current_state = END;
        return;
    }

    char current_char = ctx->input[ctx->position];
    
    switch (ctx->current_state) {
        case START:
            if (!is_space(current_char)) {
                ctx->current_state = READ_CHAR;
            } else {
                ctx->position++;
            }
            break;
            
        case READ_CHAR:
            if (is_digit(current_char)) {
                ctx->current_state = COUNT_DIGITS;
                ctx->digit_count = 0;
            } else if (is_space(current_char)) {
                ctx->current_state = START;
                ctx->position++;
            } else {
                ctx->current_state = PROCESS_WORD;
            }
            break;
            
        case COUNT_DIGITS:
            if (is_digit(current_char)) {
                ctx->digit_count++;
                ctx->position++;
            } else {
                if (ctx->digit_count >= 2) {
                    ctx->word_count++;
                }
                ctx->current_state = is_space(current_char) ? START : PROCESS_WORD;
            }
            break;
            
        case PROCESS_WORD:
            if (is_space(current_char)) {
                ctx->current_state = START;
                ctx->word_count++;
            }
            ctx->position++;
            break;
            
        case END:
            return;
    }
    
    process_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    
    printf("Enter text: ");
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input);
    if (len > 0 && ctx.input[len - 1] == '\n') {
        ctx.input[len - 1] = '\0';
    }
    
    if (strlen(ctx.input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == PROCESS_WORD) {
        ctx.word_count++;
    }
    
    printf("Words with at least 2 consecutive digits: %d\n", ctx.word_count);
    
    return 0;
}