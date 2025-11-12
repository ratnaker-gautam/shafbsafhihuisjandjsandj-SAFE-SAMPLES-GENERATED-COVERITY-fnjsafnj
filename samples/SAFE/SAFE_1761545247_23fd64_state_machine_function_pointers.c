//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    WRITING,
    DONE,
    ERROR
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
};

typedef void (*StateHandler)(struct Context*);

void init_handler(struct Context* ctx) {
    if (ctx == NULL) return;
    
    ctx->current_state = READING;
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void reading_handler(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Enter text (max 255 chars, empty line to finish): ");
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
        len--;
    }
    
    ctx->buffer_len = len;
    
    if (ctx->buffer_len == 0) {
        ctx->current_state = DONE;
    } else {
        ctx->current_state = PROCESSING;
    }
}

void processing_handler(struct Context* ctx) {
    if (ctx == NULL || ctx->buffer_len >= sizeof(ctx->buffer)) {
        if (ctx != NULL) ctx->current_state = ERROR;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
    ctx->current_state = WRITING;
}

void writing_handler(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Processed text: %s\n", ctx->buffer);
    printf("Total processed: %d\n", ctx->processed_count);
    ctx->current_state = READING;
}

void error_handler(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Error occurred in state machine\n");
    ctx->current_state = DONE;
}

void done_handler(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("State machine completed. Final count: %d\n", ctx->processed_count);
}

int main(void) {
    struct Context ctx;
    StateHandler handlers[] = {
        init_handler,
        reading_handler,
        processing_handler,
        writing_handler,
        done_handler,
        error_handler
    };
    
    if (sizeof(handlers) / sizeof(handlers[0]) != (ERROR + 1)) {
        return EXIT_FAILURE;
    }
    
    ctx.current_state = INIT;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        if (ctx.current_state < 0 || ctx.current_state >= (int)(sizeof(handlers) / sizeof(handlers[0]))) {
            break;
        }
        
        handlers[ctx.current_state](&ctx);
    }
    
    if (ctx.current_state == ERROR) {
        error_handler(&ctx);
    }
    
    return EXIT_SUCCESS;
}