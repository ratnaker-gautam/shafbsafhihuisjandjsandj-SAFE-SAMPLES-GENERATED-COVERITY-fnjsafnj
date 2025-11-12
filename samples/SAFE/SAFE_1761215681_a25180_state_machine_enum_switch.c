//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    COMPLETED,
    ERROR,
    EXIT
};

struct context {
    enum state current_state;
    int data_value;
    int step_count;
    char buffer[64];
};

void state_init(struct context *ctx) {
    ctx->data_value = 0;
    ctx->step_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->current_state = READY;
}

void state_ready(struct context *ctx) {
    printf("Enter a positive integer (1-1000): ");
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    } else if (len == sizeof(ctx->buffer) - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        ctx->current_state = ERROR;
        return;
    }
    
    if (strlen(ctx->buffer) == 0) {
        ctx->current_state = EXIT;
        return;
    }
    
    for (size_t i = 0; i < strlen(ctx->buffer); i++) {
        if (!isdigit((unsigned char)ctx->buffer[i])) {
            ctx->current_state = ERROR;
            return;
        }
    }
    
    char *endptr;
    long val = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') {
        ctx->current_state = ERROR;
        return;
    }
    if (val <= 0 || val > 1000) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->data_value = (int)val;
    ctx->current_state = PROCESSING;
}

void state_processing(struct context *ctx) {
    int n = ctx->data_value;
    ctx->step_count = 0;
    
    while (n != 1) {
        if (ctx->step_count >= 1000) {
            ctx->current_state = ERROR;
            return;
        }
        
        if (n % 2 == 0) {
            if (n > INT_MAX / 2) {
                ctx->current_state = ERROR;
                return;
            }
            n = n / 2;
        } else {
            if (n > (INT_MAX - 1) / 3) {
                ctx->current_state = ERROR;
                return;
            }
            n = 3 * n + 1;
        }
        ctx->step_count++;
    }
    
    ctx->current_state = COMPLETED;
}

void state_completed(struct context *ctx) {
    printf("Processing completed in %d steps\n", ctx->step_count);
    ctx->current_state = READY;
}

void state_error(struct context *ctx) {
    printf("Error: Invalid input or computation overflow\n");
    ctx->current_state = READY;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    while (ctx.current_state != EXIT) {
        switch (ctx.current_state) {
            case INIT:
                state_init(&ctx);
                break;
            case READY:
                state_ready(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case COMPLETED:
                state_completed(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
            case EXIT:
                break;
        }
    }
    
    printf("Program terminated\n");
    return 0;
}