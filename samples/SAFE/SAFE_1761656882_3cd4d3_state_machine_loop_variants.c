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
            
        case COUNT_DIGITS:
            while (ctx->position < ctx->input_len) {
                char c = ctx->input_buffer[ctx->position];
                if (isdigit((unsigned char)c)) {
                    ctx->current_digits++;
                    ctx->position++;
                } else if (isalpha((unsigned char)c)) {
                    ctx->position++;
                } else {
                    break;
                }
            }
            ctx->digit_count += ctx->current_digits;
            ctx->current_state = READ_CHAR;
            break;
            
        case FINALIZE:
            ctx->current_state = DONE;
            break;
            
        case DONE:
            return 1;
    }
    return 0;
}

int main(void) {
    struct context ctx;
    char user_input[512];
    
    printf("Enter text to analyze: ");
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len >= sizeof(ctx.input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    state_init(&ctx);
    strncpy(ctx.input_buffer, user_input, sizeof(ctx.input_buffer) - 1);
    ctx.input_buffer[sizeof(ctx.input_buffer) - 1] = '\0';
    ctx.input_len = strlen(ctx.input_buffer);
    
    int done = 0;
    for (int i = 0; i < 1000 && !done; i++) {
        done = state_machine_step(&ctx);
    }
    
    if (ctx.current_state != DONE) {
        fprintf(stderr, "State machine did not complete\n");
        return 1;
    }
    
    printf("Words processed: %d\n", ctx.word_count);
    printf("Total digits found: %d\n", ctx.digit_count);
    
    return 0;
}