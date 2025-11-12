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

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ' || c == '\0';
}

void state_machine(struct Context *ctx, int depth) {
    if (depth > 1000) {
        return;
    }
    
    char current_char = ctx->input[ctx->position];
    
    switch (ctx->current_state) {
        case START:
            if (ctx->position >= 0 && ctx->position < 256) {
                ctx->current_state = READ_CHAR;
                state_machine(ctx, depth + 1);
            }
            break;
            
        case READ_CHAR:
            if (ctx->position >= 256 || current_char == '\0') {
                ctx->current_state = END;
                state_machine(ctx, depth + 1);
            } else if (current_char >= '0' && current_char <= '9') {
                ctx->current_state = COUNT_DIGITS;
                state_machine(ctx, depth + 1);
            } else if ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z')) {
                ctx->current_state = PROCESS_WORD;
                state_machine(ctx, depth + 1);
            } else if (current_char == ' ') {
                ctx->position++;
                state_machine(ctx, depth + 1);
            }
            break;
            
        case COUNT_DIGITS:
            if (ctx->position < 256 && current_char >= '0' && current_char <= '9') {
                ctx->digit_count++;
                ctx->position++;
                ctx->current_state = READ_CHAR;
                state_machine(ctx, depth + 1);
            } else {
                ctx->current_state = READ_CHAR;
                state_machine(ctx, depth + 1);
            }
            break;
            
        case PROCESS_WORD:
            if (ctx->position < 256 && ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z'))) {
                ctx->position++;
                state_machine(ctx, depth + 1);
            } else {
                ctx->word_count++;
                ctx->current_state = READ_CHAR;
                state_machine(ctx, depth + 1);
            }
            break;
            
        case END:
            break;
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    
    printf("Enter text (max 255 characters): ");
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input);
    if (len > 0 && ctx.input[len - 1] == '\n') {
        ctx.input[len - 1] = '\0';
    }
    
    for (int i = 0; i < 256 && ctx.input[i] != '\0'; i++) {
        if (!is_valid_char(ctx.input[i])) {
            printf("Invalid character in input\n");
            return 1;
        }
    }
    
    state_machine(&ctx, 0);
    
    printf("Words: %d\n", ctx.word_count);
    printf("Digits: %d\n", ctx.digit_count);
    
    return 0;
}