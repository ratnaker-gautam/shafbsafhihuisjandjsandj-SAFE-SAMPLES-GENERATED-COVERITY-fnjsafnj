//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_EXIT
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int error_code;
};

typedef void (*state_handler)(struct context*);

void handle_idle(struct context* ctx) {
    printf("Enter command (read, process, write, exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->error_code = 1;
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->current_state = STATE_READING;
    } else if (strcmp(input, "process") == 0) {
        ctx->current_state = STATE_PROCESSING;
    } else if (strcmp(input, "write") == 0) {
        ctx->current_state = STATE_WRITING;
    } else if (strcmp(input, "exit") == 0) {
        ctx->current_state = STATE_EXIT;
    } else {
        printf("Invalid command\n");
    }
}

void handle_reading(struct context* ctx) {
    printf("Enter text to read (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->error_code = 2;
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    printf("Read %zu characters: %s\n", ctx->buffer_len, ctx->buffer);
    ctx->current_state = STATE_IDLE;
}

void handle_processing(struct context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to process\n");
        ctx->current_state = STATE_IDLE;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: %s\n", ctx->buffer);
    ctx->current_state = STATE_IDLE;
}

void handle_writing(struct context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to write\n");
        ctx->current_state = STATE_IDLE;
        return;
    }
    
    printf("Writing to output: %s\n", ctx->buffer);
    ctx->current_state = STATE_IDLE;
}

void handle_error(struct context* ctx) {
    printf("Error occurred (code: %d)\n", ctx->error_code);
    ctx->current_state = STATE_EXIT;
}

state_handler get_handler(enum state s) {
    switch (s) {
        case STATE_IDLE: return handle_idle;
        case STATE_READING: return handle_reading;
        case STATE_PROCESSING: return handle_processing;
        case STATE_WRITING: return handle_writing;
        case STATE_ERROR: return handle_error;
        case STATE_EXIT: return NULL;
        default: return handle_error;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_len = 0;
    ctx.error_code = 0;
    
    while (ctx.current_state != STATE_EXIT) {
        state_handler handler = get_handler(ctx.current_state);
        if (handler != NULL) {
            handler(&ctx);
        } else {
            break;
        }
    }
    
    printf("Program terminated\n");
    return 0;
}