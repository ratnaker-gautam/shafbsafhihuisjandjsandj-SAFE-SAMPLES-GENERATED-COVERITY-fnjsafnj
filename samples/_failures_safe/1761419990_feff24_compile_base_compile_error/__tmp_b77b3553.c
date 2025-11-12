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

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

State current_state = STATE_IDLE;
char buffer[256];
size_t buffer_pos = 0;

State handle_idle(Event evt) {
    switch (evt) {
        case EVENT_START:
            printf("Starting data processing...\n");
            buffer_pos = 0;
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(Event evt) {
    switch (evt) {
        case EVENT_DATA_READY:
            if (buffer_pos < sizeof(buffer) - 1) {
                printf("Enter data (max %zu chars): ", sizeof(buffer) - 1 - buffer_pos);
                if (fgets(buffer + buffer_pos, sizeof(buffer) - buffer_pos, stdin)) {
                    size_t len = strlen(buffer + buffer_pos);
                    if (len > 0 && buffer[buffer_pos + len - 1] == '\n') {
                        buffer[buffer_pos + len - 1] = '\0';
                    }
                    buffer_pos += len;
                    if (buffer_pos >= sizeof(buffer) - 1) {
                        printf("Buffer full, processing...\n");
                        return STATE_PROCESSING;
                    }
                    return STATE_READING;
                } else {
                    return STATE_ERROR;
                }
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            buffer_pos = 0;
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(Event evt) {
    switch (evt) {
        case EVENT_PROCESS_DONE:
            printf("Processing complete. Data: %s\n", buffer);
            return STATE_WRITING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            buffer_pos = 0;
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(Event evt) {
    switch (evt) {
        case EVENT_WRITE_DONE:
            printf("Data written successfully.\n");
            buffer_pos = 0;
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            buffer_pos = 0;
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(Event evt) {
    switch (evt) {
        case EVENT_RESET:
            printf("Resetting from error state.\n");
            buffer_pos = 0;
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

void process_event(Event evt) {
    if (evt < 0 || evt >= EVENT_COUNT) {
        return;
    }
    State new_state = state_handlers[current_state](evt);
    if (new_state >= 0 && new_state < STATE_COUNT) {
        current_state = new_state;
    }
}

int main() {
    char input[32];
    int running = 1;

    printf("State Machine Demo\n");
    printf("Commands: start, data, process, write, error, reset, quit\n");

    while (running) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n> ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "start") == 0) {
            process_event(EVENT_START);
        } else if (strcmp(input, "data") == 0) {
            process_event(EVENT_DATA_READY);
        } else if (strcmp(input, "process") == 0) {
            process_event(EVENT_PROCESS_DONE);
        } else if (strcmp(input, "write") == 0) {
            process_event(EVENT_WRITE_DONE);
        } else if (strcmp(input, "error") == 0) {
            process_event(EVENT_ERROR);
        } else if (strcmp(input, "reset") == 0) {
            process_event(EVENT_RESET);
        } else if (strcmp(input, "quit") ==