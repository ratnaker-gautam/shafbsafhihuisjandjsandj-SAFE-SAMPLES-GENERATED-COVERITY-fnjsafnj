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

typedef state_t (*state_handler_t)(event_t, const char*);

static state_t handle_idle(event_t event, const char* data);
static state_t handle_ready(event_t event, const char* data);
static state_t handle_processing(event_t event, const char* data);
static state_t handle_complete(event_t event, const char* data);
static state_t handle_error(event_t event, const char* data);

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

static state_t handle_idle(event_t event, const char* data) {
    if (event == EVENT_START) {
        printf("Transition: IDLE -> READY\n");
        return STATE_READY;
    }
    printf("Invalid event %s for state IDLE\n", event_names[event]);
    return STATE_IDLE;
}

static state_t handle_ready(event_t event, const char* data) {
    if (event == EVENT_DATA) {
        if (data != NULL && strlen(data) > 0 && strlen(data) < 100) {
            printf("Data received: %s\n", data);
            printf("Transition: READY -> PROCESSING\n");
            return STATE_PROCESSING;
        } else {
            printf("Invalid data\n");
            return STATE_ERROR;
        }
    } else if (event == EVENT_RESET) {
        printf("Transition: READY -> IDLE\n");
        return STATE_IDLE;
    }
    printf("Invalid event %s for state READY\n", event_names[event]);
    return STATE_READY;
}

static state_t handle_processing(event_t event, const char* data) {
    if (event == EVENT_SUCCESS) {
        printf("Processing successful\n");
        printf("Transition: PROCESSING -> COMPLETE\n");
        return STATE_COMPLETE;
    } else if (event == EVENT_FAILURE) {
        printf("Processing failed\n");
        printf("Transition: PROCESSING -> ERROR\n");
        return STATE_ERROR;
    } else if (event == EVENT_RESET) {
        printf("Transition: PROCESSING -> IDLE\n");
        return STATE_IDLE;
    }
    printf("Invalid event %s for state PROCESSING\n", event_names[event]);
    return STATE_PROCESSING;
}

static state_t handle_complete(event_t event, const char* data) {
    if (event == EVENT_RESET) {
        printf("Transition: COMPLETE -> IDLE\n");
        return STATE_IDLE;
    }
    printf("Invalid event %s for state COMPLETE\n", event_names[event]);
    return STATE_COMPLETE;
}

static state_t handle_error(event_t event, const char* data) {
    if (event == EVENT_RESET) {
        printf("Transition: ERROR -> IDLE\n");
        return STATE_IDLE;
    }
    printf("Invalid event %s for state ERROR\n", event_names[event]);
    return STATE_ERROR;
}

static event_t get_user_event(void) {
    char input[32];
    printf("\nAvailable events: ");
    for (int i = 0; i < EVENT_COUNT; i++) {
        printf("%d=%s ", i, event_names[i]);
    }
    printf("\nEnter event number: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_COUNT;
    }
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return EVENT_COUNT;
    }
    
    if (choice >= 0 && choice < EVENT_COUNT) {
        return (event_t)choice;
    }
    
    return EVENT_COUNT;
}

static char* get_user_data(void) {
    static char buffer[100];
    printf("Enter data (max 99 chars): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NULL;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        return NULL;
    }
    
    return buffer;
}

int main(void) {
    state_t current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Current state: %s\n", state_names[current_state]);
    
    while (running) {
        event_t event = get_user_event();
        if (event >= EVENT_COUNT) {
            printf("Invalid event selection\n");
            continue;
        }
        
        const char* data = NULL;
        if (event == EVENT_DATA