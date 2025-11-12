//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    FINALIZING,
    DONE,
    ERROR
};

struct context {
    enum state current_state;
    int counter;
    char buffer[64];
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= 63) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

void state_init(struct context* ctx) {
    ctx->current_state = READY;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_ready(struct context* ctx, const char* input) {
    if (!validate_input(input)) {
        ctx->current_state = ERROR;
        return;
    }
    strncpy(ctx->buffer, input, sizeof(ctx->buffer) - 1);
    ctx->current_state = PROCESSING;
}

void state_processing(struct context* ctx) {
    char* endptr;
    long value = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') {
        ctx->current_state = ERROR;
        return;
    }
    if (value < 0 || value > 10000) {
        ctx->current_state = ERROR;
        return;
    }
    ctx->counter = (int)value;
    ctx->current_state = FINALIZING;
}

void state_finalizing(struct context* ctx) {
    int result = 0;
    int i = 0;
    while (i < ctx->counter) {
        if (i % 2 == 0) {
            result += i;
        }
        i++;
    }
    int j = 0;
    do {
        if (j % 3 == 0) {
            result -= j / 2;
        }
        j++;
    } while (j < ctx->counter);
    for (int k = 0; k < ctx->counter; k++) {
        if (k % 5 == 0) {
            result += k * 2;
        }
    }
    ctx->counter = result;
    ctx->current_state = DONE;
}

void state_done(struct context* ctx) {
    printf("Result: %d\n", ctx->counter);
    ctx->current_state = INIT;
}

void state_error(struct context* ctx) {
    printf("Error: Invalid input\n");
    ctx->current_state = INIT;
}

int main(void) {
    struct context ctx;
    char input[128];
    
    state_init(&ctx);
    
    while (1) {
        switch (ctx.current_state) {
            case INIT:
                state_init(&ctx);
                break;
            case READY:
                printf("Enter a number (0-10000): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                state_ready(&ctx, input);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case FINALIZING:
                state_finalizing(&ctx);
                break;
            case DONE:
                state_done(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
        }
        
        if (ctx.current_state == INIT) {
            printf("Continue? (y/n): ");
            if (fgets(input, sizeof(input), stdin) == NULL) break;
            if (input[0] == 'n' || input[0] == 'N') break;
        }
    }
    
    return 0;
}