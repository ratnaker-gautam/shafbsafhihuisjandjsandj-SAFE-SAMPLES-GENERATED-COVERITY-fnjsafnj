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
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event evt);
State handle_reading(Event evt);
State handle_processing(Event evt);
State handle_writing(Event evt);
State handle_error(Event evt);

State handle_idle(Event evt) {
    if (evt == EVENT_START) {
        printf("Transition: IDLE -> READING\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(Event evt) {
    if (evt == EVENT_DATA_READY) {
        printf("Transition: READING -> PROCESSING\n");
        return STATE_PROCESSING;
    } else if (evt == EVENT_ERROR) {
        printf("Transition: READING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(Event evt) {
    if (evt == EVENT_PROCESS_DONE) {
        printf("Transition: PROCESSING -> WRITING\n");
        return STATE_WRITING;
    } else if (evt == EVENT_ERROR) {
        printf("Transition: PROCESSING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event evt) {
    if (evt == EVENT_WRITE_DONE) {
        printf("Transition: WRITING -> IDLE\n");
        return STATE_IDLE;
    } else if (evt == EVENT_ERROR) {
        printf("Transition: WRITING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(Event evt) {
    if (evt == EVENT_RESET) {
        printf("Transition: ERROR -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int main(void) {
    StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };

    State current_state = STATE_IDLE;
    int running = 1;
    char input[32];
    Event event;

    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA_READY, 2=PROCESS_DONE, 3=WRITE_DONE, 4=ERROR, 5=RESET\n");
    printf("Enter event number (or 'q' to quit):\n");

    while (running) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }

        char *endptr;
        long val = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Enter a number 0-5 or 'q'.\n");
            continue;
        }

        if (val < 0 || val >= EVENT_COUNT) {
            printf("Event out of range. Use 0-5.\n");
            continue;
        }

        event = (Event)val;
        State new_state = handlers[current_state](event);

        if (new_state >= STATE_COUNT) {
            printf("Invalid state transition\n");
            new_state = STATE_ERROR;
        }

        current_state = new_state;
        printf("Current state: %d\n", current_state);
    }

    return 0;
}