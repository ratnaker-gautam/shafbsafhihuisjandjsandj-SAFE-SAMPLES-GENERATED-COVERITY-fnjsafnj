//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_EXIT
} event_t;

typedef struct {
    state_t current_state;
    int data_value;
    int process_count;
} context_t;

static state_t handle_idle(event_t event, context_t *ctx);
static state_t handle_ready(event_t event, context_t *ctx);
static state_t handle_processing(event_t event, context_t *ctx);
static state_t handle_done(event_t event, context_t *ctx);
static state_t handle_error(event_t event, context_t *ctx);

typedef state_t (*state_handler_t)(event_t, context_t*);

static const state_handler_t state_handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

static state_t handle_idle(event_t event, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            ctx->data_value = 0;
            ctx->process_count = 0;
            printf("Transition: IDLE -> READY\n");
            return STATE_READY;
        case EVENT_RESET:
            printf("Reset in IDLE state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

static state_t handle_ready(event_t event, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->data_value < 100) {
                ctx->data_value += 10;
                printf("Data loaded: %d\n", ctx->data_value);
                return STATE_READY;
            } else {
                printf("Data limit reached\n");
                return STATE_READY;
            }
        case EVENT_PROCESS:
            if (ctx->data_value > 0) {
                printf("Transition: READY -> PROCESSING\n");
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_READY;
            }
        case EVENT_RESET:
            ctx->data_value = 0;
            printf("Reset in READY state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for READY state\n");
            return STATE_READY;
    }
}

static state_t handle_processing(event_t event, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_SUCCESS:
            ctx->process_count++;
            printf("Processing successful. Count: %d\n", ctx->process_count);
            if (ctx->process_count >= 3) {
                printf("Transition: PROCESSING -> DONE\n");
                return STATE_DONE;
            } else {
                printf("Transition: PROCESSING -> READY\n");
                return STATE_READY;
            }
        case EVENT_FAILURE:
            printf("Processing failed\n");
            return STATE_ERROR;
        case EVENT_RESET:
            ctx->data_value = 0;
            ctx->process_count = 0;
            printf("Reset in PROCESSING state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

static state_t handle_done(event_t event, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            ctx->data_value = 0;
            ctx->process_count = 0;
            printf("Reset in DONE state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for DONE state\n");
            return STATE_DONE;
    }
}

static state_t handle_error(event_t event, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            ctx->data_value = 0;
            ctx->process_count = 0;
            printf("Reset in ERROR state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

static event_t get_user_event(void) {
    char input[32];
    int choice;
    
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET, 6: EXIT\n");
    printf("Enter event number: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_RESET;
    }
    
    if (sscanf(input, "%d", &choice) != 1) {
        return EVENT_RESET;
    }
    
    if (choice >= 0 && choice <= 6) {
        return (event_t)choice;
    }
    
    return EVENT_RESET;
}

int main(void) {
    context_t ctx = {STATE_IDLE, 0, 0};
    int running = 1;
    
    printf("State Machine Demo Started\n");
    printf("Initial state: IDLE\n");
    
    while (running) {
        event_t event = get_user_event();