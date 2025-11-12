//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
    int processed_value;
};

int validate_input(const char* input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

enum event get_user_event(void) {
    char input[16];
    printf("Enter event (0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return EVENT_ERROR;
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) != 1) return EVENT_ERROR;
    char event_char = input[0];
    if (event_char < '0' || event_char > '5') return EVENT_ERROR;
    return (enum event)(event_char - '0');
}

int read_data(struct context* ctx) {
    printf("Enter numeric data: ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) return 0;
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    ctx->buffer_len = strlen(ctx->buffer);
    if (!validate_input(ctx->buffer, sizeof(ctx->buffer))) return 0;
    ctx->data_value = atoi(ctx->buffer);
    return 1;
}

void process_data(struct context* ctx) {
    if (ctx->data_value > 1000) {
        ctx->processed_value = ctx->data_value / 10;
    } else {
        ctx->processed_value = ctx->data_value * 2;
    }
}

void write_data(const struct context* ctx) {
    printf("Processed value: %d\n", ctx->processed_value);
}

enum state handle_event(enum state current, enum event ev, struct context* ctx) {
    switch (current) {
        case STATE_IDLE:
            switch (ev) {
                case EVENT_START:
                    printf("State: IDLE -> READING\n");
                    return STATE_READING;
                default:
                    printf("Invalid event for IDLE state\n");
                    return STATE_ERROR;
            }
        case STATE_READING:
            switch (ev) {
                case EVENT_DATA_READY:
                    if (read_data(ctx)) {
                        printf("State: READING -> PROCESSING\n");
                        return STATE_PROCESSING;
                    } else {
                        printf("Failed to read data\n");
                        return STATE_ERROR;
                    }
                case EVENT_RESET:
                    printf("State: READING -> IDLE\n");
                    return STATE_IDLE;
                default:
                    printf("Invalid event for READING state\n");
                    return STATE_ERROR;
            }
        case STATE_PROCESSING:
            switch (ev) {
                case EVENT_PROCESS_COMPLETE:
                    process_data(ctx);
                    printf("State: PROCESSING -> WRITING\n");
                    return STATE_WRITING;
                case EVENT_RESET:
                    printf("State: PROCESSING -> IDLE\n");
                    return STATE_IDLE;
                default:
                    printf("Invalid event for PROCESSING state\n");
                    return STATE_ERROR;
            }
        case STATE_WRITING:
            switch (ev) {
                case EVENT_WRITE_COMPLETE:
                    write_data(ctx);
                    printf("State: WRITING -> DONE\n");
                    return STATE_DONE;
                case EVENT_RESET:
                    printf("State: WRITING -> IDLE\n");
                    return STATE_IDLE;
                default:
                    printf("Invalid event for WRITING state\n");
                    return STATE_ERROR;
            }
        case STATE_ERROR:
            switch (ev) {
                case EVENT_RESET:
                    printf("State: ERROR -> IDLE\n");
                    return STATE_IDLE;
                default:
                    printf("Reset required from ERROR state\n");
                    return STATE_ERROR;
            }
        case STATE_DONE:
            switch (ev) {
                case EVENT_RESET:
                    printf("State: DONE -> IDLE\n");
                    return STATE_IDLE;
                default:
                    printf("Reset required from DONE state\n");
                    return STATE_DONE;
            }
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx = {STATE_IDLE, {0}, 0, 0, 0};
    enum event user_event;
    
    printf("State Machine Demo - Data Processing Pipeline\n");
    
    while (1) {
        user_event = get_user_event();
        if (user_event == EVENT_ERROR) {
            printf("Invalid event input\n");
            continue;
        }
        
        ctx.current_state = handle_event(ctx.current_state, user_event, &ctx);
        
        if (ctx.current_state == STATE_DONE) {
            printf("Processing complete. Use RESET to start over.\n");