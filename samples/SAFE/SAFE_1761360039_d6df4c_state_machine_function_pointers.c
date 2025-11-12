//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current_state;
    char buffer[256];
    size_t buffer_pos;
    int data_value;
} Context;

typedef void (*StateHandler)(Context*);

void handle_idle(Context* ctx) {
    printf("Enter command (read/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->buffer_pos = 0;
        ctx->current_state = STATE_READING;
    } else if (strcmp(input, "quit") == 0) {
        ctx->current_state = STATE_DONE;
    } else {
        printf("Unknown command\n");
    }
}

void handle_reading(Context* ctx) {
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->buffer_pos = strlen(ctx->buffer);
    if (ctx->buffer_pos > 0 && ctx->buffer[ctx->buffer_pos - 1] == '\n') {
        ctx->buffer[--ctx->buffer_pos] = '\0';
    }
    
    if (ctx->buffer_pos == 0) {
        printf("No data entered\n");
        ctx->current_state = STATE_IDLE;
    } else {
        ctx->current_state = STATE_PROCESSING;
    }
}

void handle_processing(Context* ctx) {
    int sum = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        sum += (int)ctx->buffer[i];
    }
    
    if (sum < 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->data_value = sum % 1000;
    printf("Processed value: %d\n", ctx->data_value);
    ctx->current_state = STATE_WRITING;
}

void handle_writing(Context* ctx) {
    printf("Writing value %d to output\n", ctx->data_value);
    
    FILE* output = fopen("output.txt", "a");
    if (output == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (fprintf(output, "%d\n", ctx->data_value) < 0) {
        fclose(output);
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (fclose(output) != 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    printf("Data written successfully\n");
    ctx->current_state = STATE_IDLE;
}

void handle_error(Context* ctx) {
    printf("Error occurred in state machine\n");
    ctx->current_state = STATE_DONE;
}

void handle_done(Context* ctx) {
    printf("State machine finished\n");
}

int main(void) {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_pos = 0;
    ctx.data_value = 0;
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_done
    };
    
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state >= 0 && ctx.current_state < (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            handlers[ctx.current_state](&ctx);
        } else {
            break;
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        handle_error(&ctx);
    }
    
    return 0;
}