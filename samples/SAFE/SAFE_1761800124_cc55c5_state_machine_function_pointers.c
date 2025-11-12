//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event event);
State handle_reading(Event event);
State handle_processing(Event event);
State handle_writing(Event event);
State handle_error(Event event);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

char* state_names[STATE_COUNT] = {
    "IDLE",
    "READING",
    "PROCESSING",
    "WRITING",
    "ERROR"
};

char* event_names[EVENT_COUNT] = {
    "START",
    "DATA_READY",
    "PROCESS_COMPLETE",
    "WRITE_COMPLETE",
    "ERROR",
    "RESET"
};

State current_state = STATE_IDLE;
int data_buffer[10];
unsigned int data_count = 0;

State handle_idle(Event event) {
    if (event == EVENT_START) {
        printf("Transition: IDLE -> READING\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(Event event) {
    if (event == EVENT_DATA_READY) {
        if (data_count < 10) {
            data_buffer[data_count] = data_count * 10;
            data_count++;
            printf("Read data: %d\n", data_buffer[data_count-1]);
        }
        if (data_count >= 5) {
            printf("Transition: READING -> PROCESSING\n");
            return STATE_PROCESSING;
        }
        return STATE_READING;
    } else if (event == EVENT_ERROR) {
        printf("Transition: READING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        int sum = 0;
        for (unsigned int i = 0; i < data_count; i++) {
            sum += data_buffer[i];
        }
        printf("Processing complete. Sum: %d\n", sum);
        printf("Transition: PROCESSING -> WRITING\n");
        return STATE_WRITING;
    } else if (event == EVENT_ERROR) {
        printf("Transition: PROCESSING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Data written successfully.\n");
        printf("Transition: WRITING -> IDLE\n");
        data_count = 0;
        return STATE_IDLE;
    } else if (event == EVENT_ERROR) {
        printf("Transition: WRITING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(Event event) {
    if (event == EVENT_RESET) {
        printf("System reset.\n");
        printf("Transition: ERROR -> IDLE\n");
        data_count = 0;
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        return;
    }
    State new_state = state_handlers[current_state](event);
    if (new_state >= 0 && new_state < STATE_COUNT) {
        current_state = new_state;
    }
}

Event get_user_event() {
    printf("\nCurrent state: %s\n", state_names[current_state]);
    printf("Available events:\n");
    for (int i = 0; i < EVENT_COUNT; i++) {
        printf("%d: %s\n", i, event_names[i]);
    }
    printf("Enter event number (0-%d): ", EVENT_COUNT - 1);
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_COUNT;
    }
    
    if (input >= 0 && input < EVENT_COUNT) {
        return (Event)input;
    }
    return EVENT_COUNT;
}

int main() {
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (1) {
        Event event = get_user_event();
        if (event == EVENT_COUNT) {
            printf("Invalid event. Please try again.\n");
            continue;
        }
        
        process_event(event);
        
        if (current_state == STATE_IDLE && event == EVENT_START) {
            process_event(EVENT_DATA_READY);
            process_event(EVENT_DATA_READY);
            process_event(EVENT_DATA_READY);
            process_event(EVENT_DATA_READY);
            process_event(EVENT_DATA_READY);
            process_event(EVENT_PROCESS_COMPLETE);
            process_event(EVENT_WRITE_COMPLETE);
        }
    }
    
    return 0;
}