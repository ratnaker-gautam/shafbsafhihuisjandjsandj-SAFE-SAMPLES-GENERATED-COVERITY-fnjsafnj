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
    int data_buffer[16];
    size_t data_count;
    int processed_data[16];
    size_t processed_count;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->data_count = 0;
    ctx->processed_count = 0;
    memset(ctx->data_buffer, 0, sizeof(ctx->data_buffer));
    memset(ctx->processed_data, 0, sizeof(ctx->processed_data));
}

int read_data(struct Context *ctx) {
    if (ctx == NULL || ctx->data_count >= 16) return -1;
    
    printf("Enter an integer (0-100) or -1 to stop: ");
    int value;
    if (scanf("%d", &value) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    
    if (value < -1 || value > 100) return -1;
    if (value == -1) return 0;
    
    ctx->data_buffer[ctx->data_count] = value;
    ctx->data_count++;
    return 1;
}

int process_data(struct Context *ctx) {
    if (ctx == NULL || ctx->data_count == 0 || ctx->processed_count >= 16) return -1;
    
    ctx->processed_count = 0;
    for (size_t i = 0; i < ctx->data_count; i++) {
        if (ctx->processed_count >= 16) break;
        int result = ctx->data_buffer[i] * 2;
        if (result > 1000) result = 1000;
        ctx->processed_data[ctx->processed_count] = result;
        ctx->processed_count++;
    }
    return 0;
}

int write_data(struct Context *ctx) {
    if (ctx == NULL || ctx->processed_count == 0) return -1;
    
    printf("Processed data: ");
    for (size_t i = 0; i < ctx->processed_count; i++) {
        printf("%d ", ctx->processed_data[i]);
    }
    printf("\n");
    return 0;
}

enum Event get_user_event(void) {
    printf("Available events:\n");
    printf("0: START, 1: DATA_READY, 2: PROCESS_COMPLETE, 3: WRITE_COMPLETE\n");
    printf("4: ERROR, 5: RESET\n");
    printf("Choose event (0-5): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return EVENT_ERROR;
    }
    
    if (choice < 0 || choice > 5) return EVENT_ERROR;
    return (enum Event)choice;
}

enum State handle_state_machine(struct Context *ctx, enum Event event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            switch (event) {
                case EVENT_START:
                    printf("Transition: IDLE -> READING\n");
                    return STATE_READING;
                case EVENT_RESET:
                    printf("Already in IDLE state\n");
                    return STATE_IDLE;
                default:
                    printf("Invalid event for IDLE state\n");
                    return STATE_IDLE;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA_READY:
                    if (read_data(ctx) > 0) {
                        printf("Data read successfully\n");
                        return STATE_READING;
                    } else {
                        printf("Transition: READING -> PROCESSING\n");
                        return STATE_PROCESSING;
                    }
                case EVENT_RESET:
                    printf("Transition: READING -> IDLE\n");
                    initialize_context(ctx);
                    return STATE_IDLE;
                case EVENT_ERROR:
                    printf("Transition: READING -> ERROR\n");
                    return STATE_ERROR;
                default:
                    printf("Invalid event for READING state\n");
                    return STATE_READING;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS_COMPLETE:
                    if (process_data(ctx) == 0) {
                        printf("Transition: PROCESSING -> WRITING\n");
                        return STATE_WRITING;
                    } else {
                        printf("Transition: PROCESSING -> ERROR\n");
                        return STATE_ERROR;
                    }
                case EVENT_RESET:
                    printf("Transition: PROCESSING -> IDLE\n");
                    initialize_context(ctx);
                    return STATE_IDLE;
                case EVENT_ERROR:
                    printf("Transition: PROCESSING -> ERROR\n");
                    return STATE_ERROR;
                default:
                    printf("Invalid event for PROCESSING state\n");
                    return STATE_PROCESSING;
            }
            
        case STATE_WRITING:
            switch (event) {
                case EVENT_WRITE_COMPLETE:
                    if (write_data(ctx) == 0) {
                        printf("Transition: WRITING -> DONE\n");
                        return STATE_DONE;
                    } else {
                        printf("Transition: WRITING -> ERROR\n");
                        return STATE_ERROR;
                    }
                case EVENT_RESET: