//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_DIGITS,
    FINISHED,
    ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int digit_count;
    int total_chars;
};

void state_machine_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = START;
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->digit_count = 0;
    ctx->total_chars = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

enum state handle_start(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Enter text (max 255 chars): ");
    return READ_CHAR;
}

enum state handle_read_char(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    
    int c = getchar();
    if (c == EOF || c == '\n') {
        if (ctx->buffer_pos > 0) {
            ctx->buffer[ctx->buffer_pos] = '\0';
            return PROCESS_WORD;
        }
        return FINISHED;
    }
    
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        return FINISHED;
    }
    
    ctx->buffer[ctx->buffer_pos++] = (char)c;
    ctx->total_chars++;
    return READ_CHAR;
}

enum state handle_process_word(struct context *ctx) {
    if (ctx == NULL || ctx->buffer_pos == 0) return ERROR;
    
    size_t i = 0;
    while (i < ctx->buffer_pos) {
        while (i < ctx->buffer_pos && isspace((unsigned char)ctx->buffer[i])) {
            i++;
        }
        
        if (i >= ctx->buffer_pos) break;
        
        ctx->word_count++;
        
        while (i < ctx->buffer_pos && !isspace((unsigned char)ctx->buffer[i])) {
            if (isdigit((unsigned char)ctx->buffer[i])) {
                ctx->digit_count++;
            }
            i++;
        }
    }
    
    return COUNT_DIGITS;
}

enum state handle_count_digits(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Words: %d, Digits: %d, Total chars: %d\n", 
           ctx->word_count, ctx->digit_count, ctx->total_chars);
    return FINISHED;
}

enum state handle_finished(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    return FINISHED;
}

enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Error occurred\n");
    return FINISHED;
}

int main(void) {
    struct context ctx;
    state_machine_init(&ctx);
    
    int loop_counter = 0;
    const int max_iterations = 1000;
    
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        if (loop_counter++ >= max_iterations) {
            ctx.current_state = ERROR;
            break;
        }
        
        switch (ctx.current_state) {
            case START:
                ctx.current_state = handle_start(&ctx);
                break;
            case READ_CHAR:
                ctx.current_state = handle_read_char(&ctx);
                break;
            case PROCESS_WORD:
                ctx.current_state = handle_process_word(&ctx);
                break;
            case COUNT_DIGITS:
                ctx.current_state = handle_count_digits(&ctx);
                break;
            case FINISHED:
                ctx.current_state = handle_finished(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
    }
    
    if (ctx.current_state == ERROR) {
        printf("State machine terminated with error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}