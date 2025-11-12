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
        ctx->buffer[ctx->bytes_read - 1] = '\0';
        ctx->bytes_read--;
    }
    return 0;
}

int process_data(struct Context *ctx) {
    if (ctx->bytes_read == 0) {
        return -1;
    }
    ctx->processed_value = 0;
    for (size_t i = 0; i < ctx->bytes_read; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            ctx->processed_value += ctx->buffer[i] - '0';
        }
    }
    return 0;
}

int write_data(struct Context *ctx) {
    if (ctx->write_count >= 3) {
        return -1;
    }
    printf("Processed value: %d\n", ctx->processed_value);
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
                case FAILURE:
                    return ERROR;
                default:
                    return current;
            }
        case PROCESSING:
            switch (event) {
                case PROCESS_COMPLETE:
                    if (process_data(ctx) == 0) {
                        return WRITING;
                    } else {
                        return ERROR;
                    }
                case FAILURE:
                    return ERROR;
                default:
                    return current;
            }
        case WRITING:
            switch (event) {
                case WRITE_COMPLETE:
                    if (write_data(ctx) == 0) {
                        return DONE;
                    } else {
                        return ERROR;
                    }
                case FAILURE:
                    return ERROR;
                default:
                    return current;
            }
        case DONE:
            switch (event) {
                case RESET:
                    memset(ctx, 0, sizeof(*ctx));
                    return INIT;
                default:
                    return current;
            }
        case ERROR:
            switch (event) {
                case RESET:
                    memset(ctx, 0, sizeof(*ctx));
                    return INIT;
                default:
                    return current;
            }
        default:
            return ERROR;
    }
}

int main(void) {
    enum State current_state = INIT;
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    printf("State Machine Demo - Data Processing Pipeline\n");
    printf("Commands: start, read, process, write, reset, quit\n");
    
    char input[32];
    while (current_state != DONE && current_state != ERROR) {
        printf("Current state: ");
        switch (current_state) {
            case INIT: printf("INIT"); break;
            case READING: printf("READING"); break;
            case PROCESSING: printf("PROCESSING"); break;
            case WRITING: printf("WRITING"); break;
            case DONE: printf("DONE"); break;
            case ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        enum Event event;
        if (strcmp(input, "start") == 0) {
            event = START;
        } else if (strcmp(input, "read") == 0) {
            event = DATA_READY;
        } else if (strcmp(input, "process") == 0) {
            event = PROCESS_COMPLETE;
        } else if (strcmp(input, "write") == 0) {
            event = WRITE_COMPLETE;
        } else if (strcmp(input, "reset") == 0) {
            event = RESET;
        } else if (strcmp(input, "quit") == 0) {
            break;
        } else {
            printf("Invalid command\n");
            continue;
        }
        
        current_state = handle_event(current_state, event, &ctx);