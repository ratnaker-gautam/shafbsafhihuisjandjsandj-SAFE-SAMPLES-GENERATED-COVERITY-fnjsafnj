//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    DONE,
    ERROR
};

struct context {
    enum state current_state;
    int data[10];
    int count;
    int sum;
    int processed;
};

int read_input(struct context *ctx) {
    if (ctx == NULL) return -1;
    
    printf("Enter up to 10 integers (non-integer to stop): ");
    ctx->count = 0;
    
    while (ctx->count < 10) {
        int value;
        if (scanf("%d", &value) != 1) {
            break;
        }
        ctx->data[ctx->count] = value;
        ctx->count++;
    }
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    return (ctx->count > 0) ? 0 : -1;
}

int process_data(struct context *ctx) {
    if (ctx == NULL || ctx->count <= 0) return -1;
    
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->data[i] > 0 && ctx->sum > INT_MAX - ctx->data[i]) {
            return -1;
        }
        if (ctx->data[i] < 0 && ctx->sum < INT_MIN - ctx->data[i]) {
            return -1;
        }
        ctx->sum += ctx->data[i];
    }
    
    ctx->processed = 1;
    return 0;
}

void output_results(struct context *ctx) {
    if (ctx == NULL || !ctx->processed) return;
    
    printf("Processed %d numbers\n", ctx->count);
    printf("Sum: %d\n", ctx->sum);
    
    printf("Values: ");
    for (int i = 0; i < ctx->count; i++) {
        printf("%d", ctx->data[i]);
        if (i < ctx->count - 1) printf(", ");
    }
    printf("\n");
}

int main(void) {
    struct context ctx = {INIT, {0}, 0, 0, 0};
    int running = 1;
    
    while (running) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = READING;
                break;
                
            case READING:
                if (read_input(&ctx) == 0) {
                    ctx.current_state = PROCESSING;
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (process_data(&ctx) == 0) {
                    ctx.current_state = OUTPUT;
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case OUTPUT:
                output_results(&ctx);
                ctx.current_state = DONE;
                break;
                
            case DONE:
                running = 0;
                break;
                
            case ERROR:
                printf("Error occurred during processing\n");
                running = 0;
                break;
        }
    }
    
    return 0;
}