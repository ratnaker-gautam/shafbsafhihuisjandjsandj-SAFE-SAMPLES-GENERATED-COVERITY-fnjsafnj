//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    int data_count;
    int values[10];
    int processed[10];
    int output_count;
};

int read_input(struct context *ctx) {
    if (ctx == NULL) return -1;
    
    printf("Enter number of values (1-10): ");
    int count;
    if (scanf("%d", &count) != 1) return -1;
    if (count < 1 || count > 10) return -1;
    
    ctx->data_count = count;
    
    for (int i = 0; i < count; i++) {
        printf("Enter value %d: ", i + 1);
        if (scanf("%d", &ctx->values[i]) != 1) return -1;
    }
    
    return 0;
}

int process_data(struct context *ctx) {
    if (ctx == NULL || ctx->data_count <= 0) return -1;
    
    for (int i = 0; i < ctx->data_count; i++) {
        if (ctx->values[i] > 1000 || ctx->values[i] < -1000) return -1;
        ctx->processed[i] = ctx->values[i] * 2;
    }
    
    return 0;
}

int output_results(struct context *ctx) {
    if (ctx == NULL || ctx->data_count <= 0) return -1;
    
    printf("Processed values:\n");
    for (int i = 0; i < ctx->data_count; i++) {
        printf("Value %d: %d -> %d\n", i + 1, ctx->values[i], ctx->processed[i]);
    }
    
    ctx->output_count = ctx->data_count;
    return 0;
}

void state_machine_while(struct context *ctx) {
    if (ctx == NULL) return;
    
    ctx->current_state = INIT;
    
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                ctx->current_state = READING;
                break;
                
            case READING:
                if (read_input(ctx) == 0) {
                    ctx->current_state = PROCESSING;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (process_data(ctx) == 0) {
                    ctx->current_state = OUTPUT;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case OUTPUT:
                if (output_results(ctx) == 0) {
                    ctx->current_state = DONE;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
}

void state_machine_for(struct context *ctx) {
    if (ctx == NULL) return;
    
    ctx->current_state = INIT;
    
    for (int step = 0; step < 100; step++) {
        if (ctx->current_state == DONE || ctx->current_state == ERROR) {
            break;
        }
        
        switch (ctx->current_state) {
            case INIT:
                ctx->current_state = READING;
                break;
                
            case READING:
                if (read_input(ctx) == 0) {
                    ctx->current_state = PROCESSING;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (process_data(ctx) == 0) {
                    ctx->current_state = OUTPUT;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case OUTPUT:
                if (output_results(ctx) == 0) {
                    ctx->current_state = DONE;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
}

void state_machine_do_while(struct context *ctx) {
    if (ctx == NULL) return;
    
    ctx->current_state = INIT;
    
    do {
        switch (ctx->current_state) {
            case INIT:
                ctx->current_state = READING;
                break;
                
            case READING:
                if (read_input(ctx) == 0) {
                    ctx->current_state = PROCESSING;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (process_data(ctx) == 0) {
                    ctx->current_state = OUTPUT;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case OUTPUT:
                if (output_results(ctx) == 0) {
                    ctx->current_state = DONE;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
    } while (ctx->current_state != DONE && ctx->current_state != ERROR);
}

int main(void) {
    struct context ctx = {0};
    
    printf("State Machine Demo - Loop Variants\n");
    printf("1. While loop state machine\n");
    printf("2. For loop state machine\n");
    printf("3. Do-while loop state machine\n");
    printf("Choose variant (1-3): ");
    
    int choice;
    if (