//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
            printf("Processed %d items.\n", ctx->process_count);
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

static enum State transition(enum State current, enum Event event, struct Context *ctx) {
    switch (current) {
        case INIT: return handle_init(event, ctx);
        case READY: return handle_ready(event, ctx);
        case PROCESSING: return handle_processing(event, ctx);
        case FINISHED: return handle_finished(event, ctx);
        case ERROR: return handle_error(event, ctx);
        default: return ERROR;
    }
}

static void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA_RECEIVED\n");
    printf("2 - PROCESS_COMPLETE\n");
    printf("3 - ERROR_OCCURRED\n");
    printf("4 - RESET\n");
    printf("5 - EXIT\n");
    printf("Select event: ");
}

int main(void) {
    enum State current_state = INIT;
    struct Context context = {0};
    int running = 1;

    while (running) {
        print_menu();
        int input;
        if (scanf("%d", &input) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }

        if (input < 0 || input > 5) {
            printf("Invalid event selection.\n");
            continue;
        }

        if (input == 5) {
            running = 0;
            continue;
        }

        enum Event event = (enum Event)input;
        enum State new_state = transition(current_state, event, &context);

        if (new_state == ERROR && current_state != ERROR) {
            printf("Invalid transition from current state.\n");
        }

        current_state = new_state;
    }

    printf("State machine terminated.\n");
    return 0;
}