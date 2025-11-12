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
    char input[32];
    int running = 1;

    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA_READY, 2=PROCESS_DONE, 3=WRITE_DONE, 4=ERROR, 5=RESET, 6=QUIT\n");

    while (running) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: break;
        }
        printf("\nEnter event: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        char *endptr;
        long choice = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (choice < 0 || choice > 6) {
            printf("Invalid event. Please enter 0-6.\n");
            continue;
        }

        if (choice == 6) {
            running = 0;
            continue;
        }

        Event evt = (Event)choice;
        State new_state = handlers[current_state](evt);

        if (new_state >= STATE_COUNT) {
            printf("Invalid state transition.\n");
            continue;
        }

        current_state = new_state;
    }

    printf("State machine terminated.\n");
    return 0;
}