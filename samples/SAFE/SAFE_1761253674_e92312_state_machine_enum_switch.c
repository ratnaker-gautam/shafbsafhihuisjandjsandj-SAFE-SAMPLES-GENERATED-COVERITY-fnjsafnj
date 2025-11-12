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
    
    printf("Enter up to 255 characters: ");
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
    printf("Character count: %zu\n", ctx->bytes_read);
    printf("Sum of digits: %d\n", ctx->processed_value);
    
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
        case DONE:
            return RESET;
        case ERROR:
            return RESET;
    }
    return FAILURE;
}

int main(void) {
    enum State current_state = INIT;
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    int iterations = 0;
    const int max_iterations = 3;
    
    while (iterations < max_iterations) {
        enum Event event = get_next_event(current_state);
        
        switch (current_state) {
            case INIT:
                if (event == START_READ) {
                    current_state = READING;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING:
                if (event == READ_COMPLETE) {
                    if (read_input(&ctx) == 0) {
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
                    if (write_output(&ctx) == 0) {
                        current_state = DONE;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case DONE:
                if (event == RESET) {
                    memset(&ctx, 0, sizeof(ctx));
                    current_state = INIT;
                    iterations++;
                    if (iterations < max_iterations) {
                        printf("\n--- Next iteration ---\n");
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("State machine error occurred\n");
                if (event == RESET) {
                    memset(&ctx, 0, sizeof(ctx));
                    current_state = INIT;
                    iterations++;
                    if (iterations < max_iterations) {
                        printf("\n--- Next iteration ---\n");
                    }
                } else {
                    return EXIT_FAILURE;
                }
                break;
        }
    }
    
    printf("Completed %d iterations\n", max_iterations);
    return EXIT_SUCCESS;
}