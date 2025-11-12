//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    INIT,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_DIGITS,
    FINALIZE,
    DONE
};

struct context {
    enum state current_state;
    char input_buffer[256];
    size_t input_len;
    size_t position;
    int word_count;
    int digit_count;
    int current_digits;
};

void state_init(struct context *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->current_state = INIT;
}

int state_machine_step(struct context *ctx) {
    switch (ctx->current_state) {
        case INIT:
            ctx->position = 0;
            ctx->word_count = 0;
            ctx->digit_count = 0;
            ctx->current_state = READ_CHAR;
            break;
            
        case READ_CHAR:
            if (ctx->position >= ctx->input_len) {
                ctx->current_state = FINALIZE;
            } else {
                char c = ctx->input_buffer[ctx->position];
                if (isalnum((unsigned char)c)) {
                    ctx->current_state = PROCESS_WORD;
                } else {
                    ctx->position++;
                }
            }
            break;
            
        case PROCESS_WORD:
            ctx->word_count++;
            ctx->current_digits = 0;
            ctx->current_state = COUNT_DIGITS;
            break;
            
        case COUNT_DIGITS: {
            int processed = 0;
            while (ctx->position < ctx->input_len) {
                char c = ctx->input_buffer[ctx->position];
                if (isdigit((unsigned char)c)) {
                    ctx->current_digits++;
                    ctx->digit_count++;
                } else if (!isalnum((unsigned char)c)) {
                    break;
                }
                ctx->position++;
                processed = 1;
            }
            if (processed && ctx->current_digits > 0) {
                printf("Word %d has %d digits\n", ctx->word_count, ctx->current_digits);
            }
            ctx->current_state = READ_CHAR;
            break;
        }
            
        case FINALIZE:
            printf("Total words: %d\n", ctx->word_count);
            printf("Total digits: %d\n", ctx->digit_count);
            ctx->current_state = DONE;
            break;
            
        case DONE:
            return 1;
    }
    return 0;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    printf("Enter text: ");
    if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    ctx.input_len = strlen(ctx.input_buffer);
    if (ctx.input_len > 0 && ctx.input_buffer[ctx.input_len - 1] == '\n') {
        ctx.input_buffer[ctx.input_len - 1] = '\0';
        ctx.input_len--;
    }
    
    if (ctx.input_len >= sizeof(ctx.input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int done = 0;
    while (!done) {
        done = state_machine_step(&ctx);
    }
    
    return 0;
}