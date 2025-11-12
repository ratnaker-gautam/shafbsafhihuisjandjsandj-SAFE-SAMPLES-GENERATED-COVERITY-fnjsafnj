//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    PROCESS_WORD,
    FINISHED,
    ERROR
};

struct Context {
    char input[256];
    int position;
    int digit_count;
    int word_count;
    enum State current_state;
};

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == ' ' || c == '\0';
}

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= 256) return ERROR;
    if (ctx->input[ctx->position] == '\0') return FINISHED;
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    if (ctx->position >= 256) return ERROR;
    char current = ctx->input[ctx->position];
    
    if (!is_valid_char(current)) return ERROR;
    
    if (current >= '0' && current <= '9') {
        return COUNT_DIGITS;
    } else if ((current >= 'a' && current <= 'z') || 
               (current >= 'A' && current <= 'Z')) {
        return PROCESS_WORD;
    } else if (current == ' ') {
        ctx->position++;
        return READ_CHAR;
    }
    
    return ERROR;
}

enum State handle_count_digits(struct Context *ctx) {
    if (ctx->position >= 256) return ERROR;
    
    while (ctx->position < 256 && ctx->input[ctx->position] != '\0') {
        char current = ctx->input[ctx->position];
        if (current >= '0' && current <= '9') {
            ctx->digit_count++;
            ctx->position++;
        } else if (current == ' ') {
            ctx->position++;
            break;
        } else {
            return ERROR;
        }
    }
    
    return READ_CHAR;
}

enum State handle_process_word(struct Context *ctx) {
    if (ctx->position >= 256) return ERROR;
    
    while (ctx->position < 256 && ctx->input[ctx->position] != '\0') {
        char current = ctx->input[ctx->position];
        if ((current >= 'a' && current <= 'z') || 
            (current >= 'A' && current <= 'Z')) {
            ctx->position++;
        } else if (current == ' ') {
            ctx->word_count++;
            ctx->position++;
            break;
        } else {
            return ERROR;
        }
    }
    
    return READ_CHAR;
}

void run_state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case READ_CHAR:
            ctx->current_state = handle_read_char(ctx);
            break;
        case COUNT_DIGITS:
            ctx->current_state = handle_count_digits(ctx);
            break;
        case PROCESS_WORD:
            ctx->current_state = handle_process_word(ctx);
            break;
        case FINISHED:
            return;
        case ERROR:
            return;
    }
    
    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        run_state_machine(ctx);
    }
}

int main() {
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = START;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input);
    if (len > 0 && ctx.input[len - 1] == '\n') {
        ctx.input[len - 1] = '\0';
    }
    
    if (len >= sizeof(ctx.input)) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(ctx.input[i])) {
            printf("Invalid character in input\n");
            return 1;
        }
    }
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error processing input\n");
        return 1;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Digits: %d\n", ctx.digit_count);
    
    return 0;
}