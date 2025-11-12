//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    START_READ,
    READ_COMPLETE,
    PROCESS_COMPLETE,
    WRITE_COMPLETE,
    FAILURE,
    RESET
};

struct Context {
    char buffer[256];
    size_t bytes_read;
    int processed_value;
};

int read_input(struct Context *ctx) {
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
    
    return ctx->bytes_read > 0 ? 0 : -1;
}

int process_data(struct Context *ctx) {
    if (ctx == NULL || ctx->bytes_read == 0) return -1;
    
    int sum = 0;
    for (size_t i = 0; i < ctx->bytes_read; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            sum += ctx->buffer[i] - '0';
        }
    }
    
    ctx->processed_value = sum;
    return 0;
}

int write_output(const struct Context *ctx) {
    if (ctx == NULL) return -1;
    
    printf("Input: %s\n", ctx->buffer);
    printf("Processed value: %d\n", ctx->processed_value);
    return 0;
}

enum Event get_next_event(enum State current_state) {
    switch (current_state) {
        case INIT:
            return START_READ;
        case READING:
            return READ_COMPLETE;
        case PROCESSING:
            return PROCESS_COMPLETE;
        case WRITING:
            return WRITE_COMPLETE;
        case ERROR:
            return RESET;
        default:
            return FAILURE;
    }
}

int main(void) {
    enum State current_state = INIT;
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    int cycles = 0;
    const int max_cycles = 3;
    
    while (cycles < max_cycles && current_state != DONE) {
        enum Event event = get_next_event(current_state);
        
        switch (current_state) {
            case INIT:
                switch (event) {
                    case START_READ:
                        current_state = READING;
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                break;
                
            case READING:
                switch (event) {
                    case READ_COMPLETE:
                        if (read_input(&ctx) == 0) {
                            current_state = PROCESSING;
                        } else {
                            current_state = ERROR;
                        }
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                break;
                
            case PROCESSING:
                switch (event) {
                    case PROCESS_COMPLETE:
                        if (process_data(&ctx) == 0) {
                            current_state = WRITING;
                        } else {
                            current_state = ERROR;
                        }
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                break;
                
            case WRITING:
                switch (event) {
                    case WRITE_COMPLETE:
                        if (write_output(&ctx) == 0) {
                            current_state = DONE;
                        } else {
                            current_state = ERROR;
                        }
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                break;
                
            case ERROR:
                switch (event) {
                    case RESET:
                        printf("Resetting state machine\n");
                        memset(&ctx, 0, sizeof(ctx));
                        current_state = INIT;
                        cycles++;
                        break;
                    default:
                        current_state = ERROR;
                        break;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == DONE) {
        printf("State machine completed successfully\n");
    } else {
        printf("State machine terminated with errors\n");
    }
    
    return current_state == DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}