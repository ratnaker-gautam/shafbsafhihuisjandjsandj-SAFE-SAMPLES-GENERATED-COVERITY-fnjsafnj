//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>

enum State { IDLE, READY, PROCESSING, ERROR, DONE };

struct Context {
    enum State current_state;
    int data_value;
    int error_code;
    int step_count;
};

typedef void (*StateHandler)(struct Context*);

void handle_idle(struct Context* ctx) {
    if (ctx == NULL) return;
    if (ctx->current_state != IDLE) return;
    
    printf("Enter initial data value (0-100): ");
    int input;
    if (scanf("%d", &input) != 1) {
        ctx->error_code = 1;
        ctx->current_state = ERROR;
        return;
    }
    
    if (input < 0 || input > 100) {
        ctx->error_code = 2;
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->data_value = input;
    ctx->step_count = 0;
    ctx->current_state = READY;
}

void handle_ready(struct Context* ctx) {
    if (ctx == NULL) return;
    if (ctx->current_state != READY) return;
    
    printf("Ready to process. Press Enter to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    if (c == EOF) {
        ctx->error_code = 3;
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->current_state = PROCESSING;
}

void handle_processing(struct Context* ctx) {
    if (ctx == NULL) return;
    if (ctx->current_state != PROCESSING) return;
    
    if (ctx->step_count >= 10) {
        ctx->current_state = DONE;
        return;
    }
    
    if (ctx->data_value > 1000) {
        ctx->error_code = 4;
        ctx->current_state = ERROR;
        return;
    }
    
    if (ctx->data_value > 1073741823) {
        ctx->error_code = 5;
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->data_value = ctx->data_value * 2;
    ctx->step_count++;
    
    printf("Step %d: Data value = %d\n", ctx->step_count, ctx->data_value);
    
    if (ctx->step_count >= 10) {
        ctx->current_state = DONE;
    }
}

void handle_error(struct Context* ctx) {
    if (ctx == NULL) return;
    if (ctx->current_state != ERROR) return;
    
    printf("Error %d occurred. Resetting...\n", ctx->error_code);
    ctx->current_state = IDLE;
    ctx->data_value = 0;
    ctx->error_code = 0;
    ctx->step_count = 0;
}

void handle_done(struct Context* ctx) {
    if (ctx == NULL) return;
    if (ctx->current_state != DONE) return;
    
    printf("Processing complete. Final value: %d\n", ctx->data_value);
    ctx->current_state = IDLE;
    ctx->data_value = 0;
    ctx->step_count = 0;
}

StateHandler get_handler(enum State state) {
    switch (state) {
        case IDLE: return handle_idle;
        case READY: return handle_ready;
        case PROCESSING: return handle_processing;
        case ERROR: return handle_error;
        case DONE: return handle_done;
        default: return handle_error;
    }
}

int main(void) {
    struct Context ctx = {IDLE, 0, 0, 0};
    int cycle_count = 0;
    const int max_cycles = 100;
    
    while (cycle_count < max_cycles) {
        StateHandler handler = get_handler(ctx.current_state);
        if (handler == NULL) {
            ctx.current_state = ERROR;
            ctx.error_code = 6;
            continue;
        }
        
        handler(&ctx);
        
        if (ctx.current_state == IDLE && cycle_count > 0) {
            printf("Returned to idle state. Continue? (y/n): ");
            int c = getchar();
            while (getchar() != '\n');
            if (c == 'n' || c == 'N') {
                break;
            }
        }
        
        cycle_count++;
    }
    
    printf("Program terminated after %d cycles.\n", cycle_count);
    return 0;
}