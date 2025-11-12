//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    FINISHED,
    ERROR
};

struct context {
    enum state current_state;
    int data_value;
    int counter;
    char buffer[32];
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 31) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

void state_init(struct context* ctx) {
    ctx->current_state = READY;
    ctx->data_value = 0;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_ready(struct context* ctx) {
    printf("Enter a number (0-9999): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len-1] == '\n') {
        ctx->buffer[len-1] = '\0';
    }
    
    if (!validate_input(ctx->buffer)) {
        ctx->current_state = ERROR;
        return;
    }
    
    int value = atoi(ctx->buffer);
    if (value < 0 || value > 9999) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->data_value = value;
    ctx->current_state = PROCESSING;
}

void state_processing(struct context* ctx) {
    int result = 0;
    int i = 0;
    
    while (i < ctx->data_value) {
        if (i > 0 && i % 1000 == 0) {
            result += i / 100;
        } else {
            result += i % 10;
        }
        i++;
    }
    
    int j = 0;
    do {
        if (j < ctx->data_value) {
            result -= j % 5;
        }
        j++;
    } while (j < 10);
    
    for (int k = 0; k < 5; k++) {
        if (k < ctx->data_value) {
            result += k * 2;
        }
    }
    
    ctx->counter = result;
    ctx->current_state = FINISHED;
}

void state_finished(struct context* ctx) {
    printf("Processing complete. Result: %d\n", ctx->counter);
    printf("Continue? (y/n): ");
    
    char response[16];
    if (fgets(response, sizeof(response), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (response[0] == 'y' || response[0] == 'Y') {
        ctx->current_state = INIT;
    } else if (response[0] == 'n' || response[0] == 'N') {
        ctx->current_state = ERROR;
    } else {
        ctx->current_state = ERROR;
    }
}

void state_error(struct context* ctx) {
    printf("Error occurred. Exiting.\n");
    ctx->current_state = ERROR;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    while (ctx.current_state != ERROR) {
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
            case FINISHED:
                state_finished(&ctx);
                break;
            case ERROR:
                break;
        }
    }
    
    return 0;
}