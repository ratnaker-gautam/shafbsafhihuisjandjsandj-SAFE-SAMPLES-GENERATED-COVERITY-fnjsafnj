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
    int data[10];
    int count;
    int processed_count;
    int write_count;
};

int read_data(struct Context *ctx) {
    if (ctx == NULL) return -1;
    if (ctx->count >= 10) return -1;
    
    printf("Enter integer (0-100): ");
    int value;
    if (scanf("%d", &value) != 1) return -1;
    if (value < 0 || value > 100) return -1;
    
    ctx->data[ctx->count] = value;
    ctx->count++;
    return 0;
}

int process_data(struct Context *ctx) {
    if (ctx == NULL) return -1;
    if (ctx->processed_count >= ctx->count) return -1;
    
    ctx->data[ctx->processed_count] *= 2;
    if (ctx->data[ctx->processed_count] > 200) {
        ctx->data[ctx->processed_count] = 200;
    }
    ctx->processed_count++;
    return 0;
}

int write_data(struct Context *ctx) {
    if (ctx == NULL) return -1;
    if (ctx->write_count >= ctx->processed_count) return -1;
    
    printf("Output: %d\n", ctx->data[ctx->write_count]);
    ctx->write_count++;
    return 0;
}

enum Event get_next_event(enum State current_state) {
    switch (current_state) {
        case INIT:
            return START;
        case READING:
            if (getchar() == '\n') {
                return DATA_READY;
            }
            return FAILURE;
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
    
    while (1) {
        enum Event event = get_next_event(current_state);
        
        switch (current_state) {
            case INIT:
                if (event == START) {
                    printf("Starting data processing pipeline\n");
                    current_state = READING;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING:
                if (event == DATA_READY) {
                    if (read_data(&ctx) == 0) {
                        if (ctx.count < 3) {
                            current_state = READING;
                        } else {
                            current_state = PROCESSING;
                        }
                    } else {
                        current_state = ERROR;
                    }
                } else if (event == FAILURE) {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (event == PROCESS_COMPLETE) {
                    if (process_data(&ctx) == 0) {
                        if (ctx.processed_count < ctx.count) {
                            current_state = PROCESSING;
                        } else {
                            current_state = WRITING;
                        }
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
                        if (ctx.write_count < ctx.processed_count) {
                            current_state = WRITING;
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
                
            case DONE:
                if (event == RESET) {
                    printf("Pipeline complete. Resetting.\n");
                    memset(&ctx, 0, sizeof(ctx));
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                if (event == RESET) {
                    printf("Error occurred. Resetting.\n");
                    memset(&ctx, 0, sizeof(ctx));
                    current_state = INIT;
                }
                break;
        }
        
        if (current_state == DONE) {
            break;
        }
    }
    
    return 0;
}