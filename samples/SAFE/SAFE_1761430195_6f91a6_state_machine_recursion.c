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

void state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case START:
            if (ctx->position >= 0 && ctx->position < 256) {
                ctx->current_state = READ_CHAR;
            } else {
                ctx->current_state = END;
            }
            state_machine(ctx);
            break;
            
        case READ_CHAR:
            if (ctx->position < 256 && ctx->input[ctx->position] != '\0') {
                char current = ctx->input[ctx->position];
                if (!is_valid_char(current)) {
                    ctx->current_state = END;
                } else if (current >= '0' && current <= '9') {
                    ctx->current_state = COUNT_DIGITS;
                } else if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')) {
                    ctx->current_state = PROCESS_WORD;
                } else if (current == ' ') {
                    ctx->position++;
                    state_machine(ctx);
                    return;
                }
            } else {
                ctx->current_state = END;
            }
            state_machine(ctx);
            break;
            
        case COUNT_DIGITS:
            if (ctx->position < 256 && ctx->input[ctx->position] != '\0') {
                char current = ctx->input[ctx->position];
                if (current >= '0' && current <= '9') {
                    ctx->digit_count++;
                    ctx->position++;
                    ctx->current_state = READ_CHAR;
                } else {
                    ctx->current_state = READ_CHAR;
                }
            } else {
                ctx->current_state = END;
            }
            state_machine(ctx);
            break;
            
        case PROCESS_WORD:
            if (ctx->position < 256 && ctx->input[ctx->position] != '\0') {
                char current = ctx->input[ctx->position];
                if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')) {
                    ctx->position++;
                } else {
                    ctx->word_count++;
                    ctx->current_state = READ_CHAR;
                }
            } else {
                ctx->word_count++;
                ctx->current_state = END;
            }
            state_machine(ctx);
            break;
            
        case END:
            return;
            
        default:
            ctx->current_state = END;
            state_machine(ctx);
            break;
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input);
    if (len > 0 && ctx.input[len - 1] == '\n') {
        ctx.input[len - 1] = '\0';
    }
    
    if (strlen(ctx.input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    for (int i = 0; i < 256 && ctx.input[i] != '\0'; i++) {
        if (!is_valid_char(ctx.input[i])) {
            printf("Invalid character in input\n");
            return 1;
        }
    }
    
    state_machine(&ctx);
    
    printf("Total digits: %d\n", ctx.digit_count);
    printf("Total words: %d\n", ctx.word_count);
    
    return 0;
}