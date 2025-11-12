//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct Context {
    char buffer[256];
    size_t bytes_read;
    size_t bytes_processed;
    size_t bytes_written;
};

int read_data(struct Context *ctx) {
    if (ctx == NULL) return -1;
    
    printf("Enter data to process (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return -1;
    }
    
    ctx->bytes_read = strlen(ctx->buffer);
    if (ctx->bytes_read > 0 && ctx->buffer[ctx->bytes_read - 1] == '\n') {
        ctx->buffer[ctx->bytes_read - 1] = '\0';
        ctx->bytes_read--;
    }
    
    return ctx->bytes_read > 0 ? 0 : -1;
}

int process_data(struct Context *ctx) {
    if (ctx == NULL) return -1;
    
    for (size_t i = 0; i < ctx->bytes_read; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->bytes_processed = ctx->bytes_read;
    return 0;
}

int write_data(struct Context *ctx) {
    if (ctx == NULL) return -1;
    
    printf("Processed data: %s\n", ctx->buffer);
    ctx->bytes_written = ctx->bytes_processed;
    return 0;
}

enum Event get_next_event(enum State current_state) {
    char input[32];
    
    switch (current_state) {
        case STATE_IDLE:
            printf("State: IDLE - Enter 'start' to begin or 'quit' to exit: ");
            break;
        case STATE_READING:
            printf("State: READING - Enter 'data' when ready or 'error' for error: ");
            break;
        case STATE_PROCESSING:
            printf("State: PROCESSING - Enter 'done' when complete or 'error' for error: ");
            break;
        case STATE_WRITING:
            printf("State: WRITING - Enter 'written' when complete or 'error' for error: ");
            break;
        case STATE_ERROR:
            printf("State: ERROR - Enter 'reset' to restart or 'quit' to exit: ");
            break;
        case STATE_DONE:
            printf("State: DONE - Enter 'reset' to restart or 'quit' to exit: ");
            break;
    }
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "quit") == 0) {
        exit(0);
    }
    
    switch (current_state) {
        case STATE_IDLE:
            if (strcmp(input, "start") == 0) return EVENT_START;
            break;
        case STATE_READING:
            if (strcmp(input, "data") == 0) return EVENT_DATA_READY;
            if (strcmp(input, "error") == 0) return EVENT_ERROR;
            break;
        case STATE_PROCESSING:
            if (strcmp(input, "done") == 0) return EVENT_PROCESS_COMPLETE;
            if (strcmp(input, "error") == 0) return EVENT_ERROR;
            break;
        case STATE_WRITING:
            if (strcmp(input, "written") == 0) return EVENT_WRITE_COMPLETE;
            if (strcmp(input, "error") == 0) return EVENT_ERROR;
            break;
        case STATE_ERROR:
        case STATE_DONE:
            if (strcmp(input, "reset") == 0) return EVENT_RESET;
            break;
    }
    
    return EVENT_ERROR;
}

int main(void) {
    enum State current_state = STATE_IDLE;
    struct Context ctx = {0};
    int result;
    
    printf("Text Processing State Machine\n");
    printf("Available commands: start, data, done, written, reset, error, quit\n\n");
    
    while (1) {
        enum Event event = get_next_event(current_state);
        
        switch (current_state) {
            case STATE_IDLE:
                switch (event) {
                    case EVENT_START:
                        current_state = STATE_READING;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING:
                switch (event) {
                    case EVENT_DATA_READY:
                        result = read_data(&ctx);
                        if (result == 0) {
                            current_state = STATE_PROCESSING;
                        } else {
                            current_state = STATE_ERROR;
                        }
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_PROCESSING:
                switch (event) {
                    case EVENT_PROCESS_COMPLETE:
                        result = process_data(&ctx);
                        if (result == 0) {