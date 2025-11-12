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
    char input_buffer[256];
    size_t input_pos;
    int digit_count;
    int word_count;
};

void initialize_context(struct Context *ctx) {
    ctx->current_state = START;
    ctx->input_pos = 0;
    ctx->digit_count = 0;
    ctx->word_count = 0;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
}

enum State process_start(struct Context *ctx) {
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) != NULL) {
        size_t len = strlen(ctx->input_buffer);
        if (len > 0 && ctx->input_buffer[len - 1] == '\n') {
            ctx->input_buffer[len - 1] = '\0';
        }
        ctx->input_pos = 0;
        return READ_CHAR;
    }
    return END;
}

enum State process_read_char(struct Context *ctx) {
    if (ctx->input_pos >= sizeof(ctx->input_buffer) || ctx->input_buffer[ctx->input_pos] == '\0') {
        return END;
    }
    
    char c = ctx->input_buffer[ctx->input_pos];
    if (c >= '0' && c <= '9') {
        return COUNT_DIGITS;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return PROCESS_WORD;
    }
    
    ctx->input_pos++;
    return READ_CHAR;
}

enum State process_count_digits(struct Context *ctx) {
    char c = ctx->input_buffer[ctx->input_pos];
    while (ctx->input_pos < sizeof(ctx->input_buffer) && ctx->input_buffer[ctx->input_pos] != '\0') {
        c = ctx->input_buffer[ctx->input_pos];
        if (c >= '0' && c <= '9') {
            ctx->digit_count++;
            ctx->input_pos++;
        } else {
            break;
        }
    }
    return READ_CHAR;
}

enum State process_word(struct Context *ctx) {
    char c = ctx->input_buffer[ctx->input_pos];
    while (ctx->input_pos < sizeof(ctx->input_buffer) && ctx->input_buffer[ctx->input_pos] != '\0') {
        c = ctx->input_buffer[ctx->input_pos];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            ctx->input_pos++;
        } else {
            break;
        }
    }
    ctx->word_count++;
    return READ_CHAR;
}

void run_state_machine(struct Context *ctx) {
    switch (ctx->current_state) {
        case START:
            ctx->current_state = process_start(ctx);
            break;
        case READ_CHAR:
            ctx->current_state = process_read_char(ctx);
            break;
        case COUNT_DIGITS:
            ctx->current_state = process_count_digits(ctx);
            break;
        case PROCESS_WORD:
            ctx->current_state = process_word(ctx);
            break;
        case END:
            return;
    }
    
    if (ctx->current_state != END) {
        run_state_machine(ctx);
    }
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    
    run_state_machine(&ctx);
    
    printf("Words found: %d\n", ctx.word_count);
    printf("Digits found: %d\n", ctx.digit_count);
    
    return 0;
}