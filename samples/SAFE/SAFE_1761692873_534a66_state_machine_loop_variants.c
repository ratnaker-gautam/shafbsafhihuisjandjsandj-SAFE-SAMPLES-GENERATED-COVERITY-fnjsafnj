//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    PROCESS_WORD,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int digit_count;
    int char_count;
};

void state_machine_init(struct Context *ctx) {
    ctx->current_state = START;
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->digit_count = 0;
    ctx->char_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

enum State handle_start(struct Context *ctx, int c) {
    if (c == EOF) {
        return FINISHED;
    } else if (isspace(c)) {
        return START;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buffer_pos++] = (char)c;
        ctx->char_count++;
        return READ_CHAR;
    } else {
        return ERROR;
    }
}

enum State handle_read_char(struct Context *ctx, int c) {
    if (c == EOF || isspace(c)) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        ctx->word_count++;
        ctx->buffer_pos = 0;
        return (c == EOF) ? FINISHED : START;
    } else if (isalnum(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
            if (isdigit(c)) {
                ctx->digit_count++;
            }
            return READ_CHAR;
        } else {
            return ERROR;
        }
    } else {
        return ERROR;
    }
}

enum State handle_error(struct Context *ctx) {
    printf("Error: Invalid input character encountered\n");
    return FINISHED;
}

void process_state_machine(struct Context *ctx) {
    int c;
    
    while (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        c = getchar();
        
        switch (ctx->current_state) {
            case START:
                ctx->current_state = handle_start(ctx, c);
                break;
            case READ_CHAR:
                ctx->current_state = handle_read_char(ctx, c);
                break;
            case ERROR:
                ctx->current_state = handle_error(ctx);
                break;
            default:
                ctx->current_state = ERROR;
                break;
        }
        
        if (ctx->current_state == ERROR) {
            ctx->current_state = handle_error(ctx);
        }
    }
}

int main(void) {
    struct Context ctx;
    
    state_machine_init(&ctx);
    
    printf("Enter text (letters and digits only, words separated by spaces):\n");
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == FINISHED) {
        printf("Processing complete:\n");
        printf("Words processed: %d\n", ctx.word_count);
        printf("Total characters: %d\n", ctx.char_count);
        printf("Digits found: %d\n", ctx.digit_count);
    }
    
    return 0;
}