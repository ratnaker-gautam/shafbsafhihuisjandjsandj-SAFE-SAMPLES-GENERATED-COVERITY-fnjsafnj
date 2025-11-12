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
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == ' ' || c == '\0';
}

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= 255) return END;
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    if (ctx->position >= 255) return END;
    
    char current = ctx->input[ctx->position];
    if (!is_valid_char(current)) return END;
    
    if (current == '\0') return END;
    if (current >= '0' && current <= '9') return COUNT_DIGITS;
    if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')) return PROCESS_WORD;
    if (current == ' ') {
        ctx->position++;
        return READ_CHAR;
    }
    
    return END;
}

enum State handle_count_digits(struct Context *ctx) {
    if (ctx->position >= 255) return END;
    
    char current = ctx->input[ctx->position];
    if (!is_valid_char(current)) return END;
    
    if (current >= '0' && current <= '9') {
        ctx->digit_count++;
        ctx->position++;
        return COUNT_DIGITS;
    }
    
    return READ_CHAR;
}

enum State handle_process_word(struct Context *ctx) {
    if (ctx->position >= 255) return END;
    
    char current = ctx->input[ctx->position];
    if (!is_valid_char(current)) return END;
    
    if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')) {
        ctx->position++;
        return PROCESS_WORD;
    }
    
    ctx->word_count++;
    return READ_CHAR;
}

void process_state_machine(struct Context *ctx) {
    if (ctx->current_state == END) return;
    
    enum State next_state;
    
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx);
            break;
        case READ_CHAR:
            next_state = handle_read_char(ctx);
            break;
        case COUNT_DIGITS:
            next_state = handle_count_digits(ctx);
            break;
        case PROCESS_WORD:
            next_state = handle_process_word(ctx);
            break;
        case END:
            return;
    }
    
    ctx->current_state = next_state;
    process_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    
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
    
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    
    process_state_machine(&ctx);
    
    printf("Words: %d\n", ctx.word_count);
    printf("Digits: %d\n", ctx.digit_count);
    
    return 0;
}