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
    int processed_value;
    int write_count;
};

int read_data(struct Context *ctx) {
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return -1;
    }
    ctx->bytes_read = strlen(ctx->buffer);
    if (ctx->bytes_read > 0 && ctx->buffer[ctx->bytes_read - 1] == '\n') {
        ctx->buffer[--ctx->bytes_read] = '\0';
    }
    return 0;
}

int process_data(struct Context *ctx) {
    if (ctx->bytes_read == 0) {
        return -1;
    }
    ctx->processed_value = 0;
    for (size_t i = 0; i < ctx->bytes_read; i++) {
        ctx->processed_value += (int)ctx->buffer[i];
    }
    return 0;
}

int write_data(struct Context *ctx) {
    if (ctx->bytes_read == 0) {
        return -1;
    }
    printf("Processed value: %d\n", ctx->processed_value);
    printf("Original data: %s\n", ctx->buffer);
    ctx->write_count++;
    return 0;
}

enum State handle_event(enum State current, enum Event event, struct Context *ctx) {
    switch (current) {
        case INIT:
            switch (event) {
                case START:
                    return READING;
                default:
                    return ERROR;
            }
        case READING:
            switch (event) {
                case DATA_READY:
                    if (read_data(ctx) == 0) {
                        return PROCESSING;
                    } else {
                        return ERROR;
                    }
                case RESET:
                    memset(ctx, 0, sizeof(*ctx));
                    return INIT;
                default:
                    return ERROR;
            }
        case PROCESSING:
            switch (event) {
                case PROCESS_COMPLETE:
                    if (process_data(ctx) == 0) {
                        return WRITING;
                    } else {
                        return ERROR;
                    }
                case RESET:
                    memset(ctx, 0, sizeof(*ctx));
                    return INIT;
                default:
                    return ERROR;
            }
        case WRITING:
            switch (event) {
                case WRITE_COMPLETE:
                    if (write_data(ctx) == 0) {
                        return DONE;
                    } else {
                        return ERROR;
                    }
                case RESET:
                    memset(ctx, 0, sizeof(*ctx));
                    return INIT;
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
    enum State current = INIT;
    struct Context ctx = {0};
    char input[32];
    
    printf("State Machine Demo - Commands: start, reset, quit\n");
    
    while (current != DONE && current != ERROR) {
        printf("Current state: %d - Enter command: ", current);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        enum Event event;
        if (strcmp(input, "start") == 0) {
            event = START;
        } else if (strcmp(input, "reset") == 0) {
            event = RESET;
        } else if (strcmp(input, "quit") == 0) {
            break;
        } else {
            printf("Invalid command\n");
            continue;
        }
        
        current = handle_event(current, event, &ctx);
        
        if (current == READING) {
            current = handle_event(current, DATA_READY, &ctx);
        } else if (current == PROCESSING) {
            current = handle_event(current, PROCESS_COMPLETE, &ctx);
        } else if (current == WRITING) {
            current = handle_event(current, WRITE_COMPLETE, &ctx);
        }
    }
    
    if (current == ERROR) {
        printf("State machine entered error state\n");
        return 1;
    }
    
    printf("State machine completed successfully\n");
    return 0;
}