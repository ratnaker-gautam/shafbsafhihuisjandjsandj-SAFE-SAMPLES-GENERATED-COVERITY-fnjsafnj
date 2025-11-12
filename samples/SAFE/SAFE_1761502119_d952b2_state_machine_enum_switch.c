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

static int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

static int process_data(const char* data, size_t len) {
    if (data == NULL || len == 0) {
        return -1;
    }
    int sum = 0;
    for (size_t i = 0; i < len; i++) {
        if (sum > 1000 - (int)data[i]) {
            return -1;
        }
        sum += (int)data[i];
    }
    return sum % 1000;
}

static enum State handle_event(enum State current, enum Event event, struct Context* ctx) {
    switch (current) {
        case INIT:
            switch (event) {
                case START_READ:
                    printf("Enter text to process (max 255 chars): ");
                    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                        ctx->bytes_read = strlen(ctx->buffer);
                        if (ctx->bytes_read > 0 && ctx->buffer[ctx->bytes_read - 1] == '\n') {
                            ctx->buffer[--ctx->bytes_read] = '\0';
                        }
                        if (validate_input(ctx->buffer, ctx->bytes_read)) {
                            return READING;
                        }
                    }
                    return ERROR;
                default:
                    return ERROR;
            }
        
        case READING:
            switch (event) {
                case READ_COMPLETE:
                    ctx->processed_value = process_data(ctx->buffer, ctx->bytes_read);
                    if (ctx->processed_value >= 0) {
                        return PROCESSING;
                    }
                    return ERROR;
                default:
                    return ERROR;
            }
        
        case PROCESSING:
            switch (event) {
                case PROCESS_COMPLETE:
                    printf("Processing complete. Result: %d\n", ctx->processed_value);
                    return WRITING;
                default:
                    return ERROR;
            }
        
        case WRITING:
            switch (event) {
                case WRITE_COMPLETE:
                    printf("Data written successfully.\n");
                    return DONE;
                default:
                    return ERROR;
            }
        
        case DONE:
            switch (event) {
                case RESET:
                    memset(ctx, 0, sizeof(*ctx));
                    return INIT;
                default:
                    return DONE;
            }
        
        case ERROR:
            switch (event) {
                case RESET:
                    memset(ctx, 0, sizeof(*ctx));
                    return INIT;
                default:
                    return ERROR;
            }
        
        default:
            return ERROR;
    }
}

int main(void) {
    enum State current_state = INIT;
    struct Context ctx = {0};
    int running = 1;
    
    while (running) {
        enum Event event;
        
        switch (current_state) {
            case INIT:
                event = START_READ;
                break;
            
            case READING:
                event = READ_COMPLETE;
                break;
            
            case PROCESSING:
                event = PROCESS_COMPLETE;
                break;
            
            case WRITING:
                event = WRITE_COMPLETE;
                break;
            
            case DONE:
                printf("Process completed. Enter 'r' to reset or 'q' to quit: ");
                char input[16];
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    if (input[0] == 'r') {
                        event = RESET;
                    } else if (input[0] == 'q') {
                        running = 0;
                        continue;
                    } else {
                        event = FAILURE;
                    }
                } else {
                    event = FAILURE;
                }
                break;
            
            case ERROR:
                printf("Error occurred. Enter 'r' to reset or 'q' to quit: ");
                char error_input[16];
                if (fgets(error_input, sizeof(error_input), stdin) != NULL) {
                    if (error_input[0] == 'r') {
                        event = RESET;
                    } else if (error_input[0] == 'q') {
                        running = 0;
                        continue;
                    } else {
                        event = FAILURE;
                    }
                } else {
                    event = FAILURE;
                }
                break;
            
            default:
                event = FAILURE;
                break;
        }
        
        enum State next_state = handle_event(current_state, event, &ctx);
        if (next_state == ERROR && current_state != ERROR) {
            printf("State transition error from %d with event %d\n", current_state, event);
        }
        current_state = next_state;
    }
    
    return 0;
}