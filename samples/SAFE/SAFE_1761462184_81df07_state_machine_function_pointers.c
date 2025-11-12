//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state {
    IDLE,
    RUNNING,
    PAUSED,
    STOPPED,
    ERROR
};

struct context {
    enum state current_state;
    int counter;
    char buffer[64];
};

typedef void (*state_handler)(struct context*);

void idle_handler(struct context* ctx) {
    if (ctx == NULL) return;
    
    printf("Enter command (start/exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "start") == 0) {
        ctx->current_state = RUNNING;
        ctx->counter = 0;
    } else if (strcmp(ctx->buffer, "exit") == 0) {
        ctx->current_state = STOPPED;
    } else {
        printf("Unknown command\n");
    }
}

void running_handler(struct context* ctx) {
    if (ctx == NULL) return;
    
    ctx->counter++;
    printf("Running... Counter: %d\n", ctx->counter);
    
    if (ctx->counter >= 10) {
        ctx->current_state = PAUSED;
        return;
    }
    
    printf("Enter command (pause/stop): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "pause") == 0) {
        ctx->current_state = PAUSED;
    } else if (strcmp(ctx->buffer, "stop") == 0) {
        ctx->current_state = STOPPED;
    } else if (strcmp(ctx->buffer, "") != 0) {
        printf("Unknown command\n");
    }
}

void paused_handler(struct context* ctx) {
    if (ctx == NULL) return;
    
    printf("Paused at counter: %d\n", ctx->counter);
    
    printf("Enter command (resume/stop): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "resume") == 0) {
        ctx->current_state = RUNNING;
    } else if (strcmp(ctx->buffer, "stop") == 0) {
        ctx->current_state = STOPPED;
    } else {
        printf("Unknown command\n");
    }
}

void stopped_handler(struct context* ctx) {
    if (ctx == NULL) return;
    printf("Final counter value: %d\n", ctx->counter);
    printf("Goodbye!\n");
}

void error_handler(struct context* ctx) {
    if (ctx == NULL) return;
    printf("Error occurred in state machine\n");
    ctx->current_state = STOPPED;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.counter = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    state_handler handlers[] = {
        idle_handler,
        running_handler,
        paused_handler,
        stopped_handler,
        error_handler
    };
    
    if (sizeof(handlers) / sizeof(handlers[0]) != ERROR + 1) {
        return EXIT_FAILURE;
    }
    
    while (ctx.current_state != STOPPED) {
        if (ctx.current_state < IDLE || ctx.current_state > ERROR) {
            ctx.current_state = ERROR;
        }
        
        handlers[ctx.current_state](&ctx);
    }
    
    return EXIT_SUCCESS;
}