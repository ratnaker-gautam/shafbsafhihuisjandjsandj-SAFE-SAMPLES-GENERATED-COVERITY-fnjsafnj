//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: enum_switch ; Variation: state_machine
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

enum Event {
    START,
    DATA_READY,
    PROCESS_COMPLETE,
    WRITE_COMPLETE,
    FAILURE,
    RESET
};

struct Context {
    char buffer[256];
    size_t bytes_read;
    int processed_count;
    int max_iterations;
};

static int read_data(struct Context *ctx) {
    if (ctx == NULL) return -1;
    
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return -1;
    }
    
    ctx->bytes_read = strlen(ctx->buffer);
    if (ctx->bytes_read > 0 && ctx->buffer[ctx->bytes_read - 1] == '\n') {
        ctx->buffer[ctx->bytes_read - 1] = '\0';
        ctx->bytes_read--;
    }
    
    if (ctx->bytes_read == 0) {
        return -1;
    }
    
    return 0;
}

static int process_data(struct Context *ctx) {
    if (ctx == NULL || ctx->bytes_read == 0) return -1;
    
    for (size_t i = 0; i < ctx->bytes_read; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
    return 0;
}

static int write_data(struct Context *ctx) {
    if (ctx == NULL || ctx->bytes_read == 0) return -1;
    
    printf("Processed data: %s\n", ctx->buffer);
    return 0;
}

static enum Event get_next_event(enum State current_state) {
    switch (current_state) {
        case INIT:
            return START;
        case READING:
            return DATA_READY;
        case PROCESSING:
            return PROCESS_COMPLETE;
        case WRITING:
            return WRITE_COMPLETE;
        case ERROR:
            return FAILURE;
        default:
            return FAILURE;
    }
}

int main(void) {
    enum State current_state = INIT;
    struct Context ctx = {0};
    ctx.max_iterations = 3;
    int iteration = 0;
    
    while (current_state != DONE && iteration < ctx.max_iterations) {
        enum Event event = get_next_event(current_state);
        
        switch (current_state) {
            case INIT:
                if (event == START) {
                    printf("State machine started\n");
                    current_state = READING;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING:
                if (event == DATA_READY) {
                    if (read_data(&ctx) == 0) {
                        current_state = PROCESSING;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (event == PROCESS_COMPLETE) {
                    if (process_data(&ctx) == 0) {
                        current_state = WRITING;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case WRITING:
                if (event == WRITE_COMPLETE) {
                    if (write_data(&ctx) == 0) {
                        iteration++;
                        if (iteration < ctx.max_iterations) {
                            current_state = READING;
                        } else {
                            current_state = DONE;
                        }
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error occurred in state machine\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("State machine completed after %d iterations\n", iteration);
    return 0;
}