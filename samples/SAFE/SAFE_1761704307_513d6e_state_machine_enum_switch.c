//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    INIT,
    READY,
    PROCESSING,
    FINISHED,
    ERROR
};

enum Event {
    START,
    DATA_RECEIVED,
    PROCESS_COMPLETE,
    ERROR_OCCURRED,
    RESET
};

struct Context {
    int data_value;
    int process_count;
    char buffer[64];
};

static enum State handle_init(enum Event event, struct Context *ctx) {
    switch (event) {
        case START:
            printf("State machine started.\n");
            ctx->process_count = 0;
            return READY;
        default:
            return ERROR;
    }
}

static enum State handle_ready(enum Event event, struct Context *ctx) {
    switch (event) {
        case DATA_RECEIVED:
            printf("Enter data value (0-100): ");
            if (scanf("%d", &ctx->data_value) != 1) {
                printf("Invalid input.\n");
                return ERROR;
            }
            if (ctx->data_value < 0 || ctx->data_value > 100) {
                printf("Value out of range.\n");
                return ERROR;
            }
            snprintf(ctx->buffer, sizeof(ctx->buffer), "Processing value: %d", ctx->data_value);
            return PROCESSING;
        case RESET:
            printf("Resetting to INIT.\n");
            return INIT;
        default:
            return ERROR;
    }
}

static enum State handle_processing(enum Event event, struct Context *ctx) {
    switch (event) {
        case PROCESS_COMPLETE:
            printf("%s\n", ctx->buffer);
            ctx->process_count++;
            printf("Process count: %d\n", ctx->process_count);
            return FINISHED;
        case ERROR_OCCURRED:
            printf("Processing error occurred.\n");
            return ERROR;
        default:
            return ERROR;
    }
}

static enum State handle_finished(enum Event event, struct Context *ctx) {
    switch (event) {
        case DATA_RECEIVED:
            return READY;
        case RESET:
            printf("Resetting to INIT.\n");
            return INIT;
        default:
            return ERROR;
    }
}

static enum State handle_error(enum Event event, struct Context *ctx) {
    switch (event) {
        case RESET:
            printf("Resetting from error state.\n");
            return INIT;
        default:
            return ERROR;
    }
}

int main(void) {
    enum State current_state = INIT;
    struct Context ctx = {0};
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA_RECEIVED, 2=PROCESS_COMPLETE, 3=ERROR_OCCURRED, 4=RESET, 5=EXIT\n");
    
    while (running) {
        int input;
        printf("Current state: %d\n", current_state);
        printf("Enter event: ");
        
        if (scanf("%d", &input) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input == 5) {
            running = 0;
            continue;
        }
        
        if (input < 0 || input > 4) {
            printf("Invalid event.\n");
            continue;
        }
        
        enum Event event = (enum Event)input;
        enum State next_state;
        
        switch (current_state) {
            case INIT:
                next_state = handle_init(event, &ctx);
                break;
            case READY:
                next_state = handle_ready(event, &ctx);
                break;
            case PROCESSING:
                next_state = handle_processing(event, &ctx);
                break;
            case FINISHED:
                next_state = handle_finished(event, &ctx);
                break;
            case ERROR:
                next_state = handle_error(event, &ctx);
                break;
            default:
                next_state = ERROR;
                break;
        }
        
        current_state = next_state;
    }
    
    printf("State machine terminated.\n");
    return 0;
}