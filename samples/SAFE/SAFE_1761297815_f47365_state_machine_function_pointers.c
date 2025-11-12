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

static state_handler_t handlers[STATE_COUNT] = {
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
        printf("Transition: IDLE -> READY\n");
        return STATE_READY;
    }
    printf("Invalid event %s for state IDLE\n", event_names[event]);
    return STATE_IDLE;
}

static state_t handle_ready(event_t event) {
    if (event == EVENT_DATA) {
        data_value = 42;
        printf("Data loaded: %d\n", data_value);
        printf("Transition: READY -> PROCESSING\n");
        return STATE_PROCESSING;
    }
    if (event == EVENT_RESET) {
        data_value = 0;
        printf("Transition: READY -> IDLE\n");
        return STATE_IDLE;
    }
    printf("Invalid event %s for state READY\n", event_names[event]);
    return STATE_READY;
}

static state_t handle_processing(event_t event) {
    if (event == EVENT_SUCCESS) {
        printf("Processing successful\n");
        printf("Transition: PROCESSING -> COMPLETE\n");
        return STATE_COMPLETE;
    }
    if (event == EVENT_FAILURE) {
        printf("Processing failed\n");
        printf("Transition: PROCESSING -> ERROR\n");
        return STATE_ERROR;
    }
    if (event == EVENT_RESET) {
        data_value = 0;
        printf("Transition: PROCESSING -> IDLE\n");
        return STATE_IDLE;
    }
    printf("Invalid event %s for state PROCESSING\n", event_names[event]);
    return STATE_PROCESSING;
}

static state_t handle_complete(event_t event) {
    if (event == EVENT_RESET) {
        data_value = 0;
        printf("Transition: COMPLETE -> IDLE\n");
        return STATE_IDLE;
    }
    printf("Invalid event %s for state COMPLETE\n", event_names[event]);
    return STATE_COMPLETE;
}

static state_t handle_error(event_t event) {
    if (event == EVENT_RESET) {
        data_value = 0;
        printf("Transition: ERROR -> IDLE\n");
        return STATE_IDLE;
    }
    printf("Invalid event %s for state ERROR\n", event_names[event]);
    return STATE_ERROR;
}

static void process_event(event_t event) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event code: %d\n", event);
        return;
    }
    state_t new_state = handlers[current_state](event);
    if (new_state < 0 || new_state >= STATE_COUNT) {
        printf("Invalid state transition\n");
        return;
    }
    current_state = new_state;
}

static event_t get_user_event(void) {
    char input[32];
    printf("\nCurrent state: %s\n", state_names[current_state]);
    printf("Available events:\n");
    for (int i = 0; i < EVENT_COUNT; i++) {
        printf("%d: %s\n", i, event_names[i]);
    }
    printf("Enter event number (0-%d) or -1 to exit: ", EVENT_COUNT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        printf("Invalid input\n");
        return -2;
    }
    
    if (choice == -1) {
        return -1;
    }
    
    if (choice < 0 || choice >= EVENT_COUNT) {
        printf("Event out of range\n");
        return -2;
    }
    
    return (event_t)choice;
}

int main(void) {
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (1) {
        event_t event = get_user_event();
        if (event == -1) {
            break;
        }
        if (event == -2) {
            continue;
        }
        process_event(event);
    }
    
    printf("Exiting state machine\n");
    return 0;
}