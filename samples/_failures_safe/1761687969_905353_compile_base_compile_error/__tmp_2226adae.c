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
    enum State current_state;
    char buffer[256];
    size_t data_size;
    int error_code;
};

int validate_input(const char* input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void process_data(struct Context* ctx) {
    if (ctx == NULL || ctx->data_size == 0) return;
    for (size_t i = 0; i < ctx->data_size; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
}

enum State handle_event(struct Context* ctx, enum Event event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            switch (event) {
                case EVENT_START:
                    printf("Enter data to process: ");
                    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                        size_t len = strlen(ctx->buffer);
                        if (len > 0 && ctx->buffer[len-1] == '\n') {
                            ctx->buffer[len-1] = '\0';
                            len--;
                        }
                        if (validate_input(ctx->buffer, sizeof(ctx->buffer) - 1)) {
                            ctx->data_size = len;
                            return STATE_READING;
                        }
                    }
                    ctx->error_code = 1;
                    return STATE_ERROR;
                default:
                    return STATE_IDLE;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA_READY:
                    process_data(ctx);
                    return STATE_PROCESSING;
                case EVENT_ERROR:
                    ctx->error_code = 2;
                    return STATE_ERROR;
                default:
                    return STATE_READING;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS_COMPLETE:
                    printf("Processed data: %s\n", ctx->buffer);
                    return STATE_WRITING;
                case EVENT_ERROR:
                    ctx->error_code = 3;
                    return STATE_ERROR;
                default:
                    return STATE_PROCESSING;
            }
            
        case STATE_WRITING:
            switch (event) {
                case EVENT_WRITE_COMPLETE:
                    printf("Data written successfully.\n");
                    return STATE_DONE;
                case EVENT_ERROR:
                    ctx->error_code = 4;
                    return STATE_ERROR;
                default:
                    return STATE_WRITING;
            }
            
        case STATE_ERROR:
            switch (event) {
                case EVENT_RESET:
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    ctx->data_size = 0;
                    ctx->error_code = 0;
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_DONE:
            switch (event) {
                case EVENT_RESET:
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    ctx->data_size = 0;
                    return STATE_IDLE;
                default:
                    return STATE_DONE;
            }
            
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = STATE_IDLE;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    ctx.data_size = 0;
    ctx.error_code = 0;
    
    int running = 1;
    while (running) {
        enum Event event = EVENT_RESET;
        
        switch (ctx.current_state) {
            case STATE_IDLE:
                printf("State: IDLE - Press 1 to start, 0 to exit: ");
                break;
            case STATE_READING:
                printf("State: READING - Press 1 to confirm data ready: ");
                break;
            case STATE_PROCESSING:
                printf("State: PROCESSING - Press 1 to complete processing: ");
                break;
            case STATE_WRITING:
                printf("State: WRITING - Press 1 to complete writing: ");
                break;
            case STATE_ERROR:
                printf("State: ERROR (code: %d) - Press 1 to reset: ", ctx.error_code);
                break;
            case STATE_DONE:
                printf("State: DONE - Press 1 to reset, 0 to exit: ");
                break;
        }
        
        char input[16];
        if (fgets(input, sizeof(input), stdin) != NULL) {
            int choice = atoi(input);
            
            switch (ctx.current_state) {
                case STATE_IDLE:
                    if (choice == 1) event = EVENT_START;
                    else if (choice == 0) running = 0;
                    break;
                case STATE_READING:
                    if (choice == 1)