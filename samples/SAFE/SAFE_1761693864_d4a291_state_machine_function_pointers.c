//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} event_t;

typedef state_t (*state_handler_t)(event_t);

static state_t handle_idle(event_t event);
static state_t handle_ready(event_t event);
static state_t handle_processing(event_t event);
static state_t handle_complete(event_t event);
static state_t handle_error(event_t event);

static const state_handler_t state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

static const char* state_names[STATE_COUNT] = {
    "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
};

static const char* event_names[EVENT_COUNT] = {
    "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
};

static state_t current_state = STATE_IDLE;
static int data_value = 0;

static state_t handle_idle(event_t event) {
    if (event == EVENT_START) {
        printf("State: IDLE -> READY\n");
        return STATE_READY;
    }
    return STATE_IDLE;
}

static state_t handle_ready(event_t event) {
    if (event == EVENT_DATA) {
        data_value = 42;
        printf("State: READY -> READY (Data loaded: %d)\n", data_value);
        return STATE_READY;
    } else if (event == EVENT_PROCESS && data_value != 0) {
        printf("State: READY -> PROCESSING\n");
        return STATE_PROCESSING;
    }
    return STATE_READY;
}

static state_t handle_processing(event_t event) {
    if (event == EVENT_SUCCESS) {
        printf("State: PROCESSING -> COMPLETE\n");
        return STATE_COMPLETE;
    } else if (event == EVENT_FAILURE) {
        printf("State: PROCESSING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

static state_t handle_complete(event_t event) {
    if (event == EVENT_RESET) {
        data_value = 0;
        printf("State: COMPLETE -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_COMPLETE;
}

static state_t handle_error(event_t event) {
    if (event == EVENT_RESET) {
        data_value = 0;
        printf("State: ERROR -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

static void process_event(event_t event) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    state_t new_state = state_handlers[current_state](event);
    
    if (new_state < 0 || new_state >= STATE_COUNT) {
        printf("Invalid state transition\n");
        return;
    }
    
    current_state = new_state;
}

static event_t get_user_event(void) {
    printf("\nAvailable events:\n");
    for (int i = 0; i < EVENT_COUNT; i++) {
        printf("%d: %s\n", i, event_names[i]);
    }
    
    printf("Current state: %s\n", state_names[current_state]);
    printf("Enter event number (0-%d) or -1 to exit: ", EVENT_COUNT - 1);
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_COUNT;
    }
    
    if (input == -1) {
        return EVENT_COUNT;
    }
    
    if (input < 0 || input >= EVENT_COUNT) {
        printf("Invalid event number\n");
        return EVENT_COUNT;
    }
    
    return (event_t)input;
}

int main(void) {
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (1) {
        event_t event = get_user_event();
        if (event == EVENT_COUNT) {
            break;
        }
        process_event(event);
    }
    
    printf("Exiting state machine demo.\n");
    return 0;
}