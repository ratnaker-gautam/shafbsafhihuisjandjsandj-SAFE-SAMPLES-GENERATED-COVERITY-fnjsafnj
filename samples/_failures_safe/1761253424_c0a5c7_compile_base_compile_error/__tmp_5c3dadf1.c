//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_END
} State;

typedef enum {
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_OUTPUT_SENT,
    EVENT_TERMINATE
} Event;

State handle_start(Event event, const char* input, char* output);
State handle_reading(Event event, const char* input, char* output);
State handle_processing(Event event, const char* input, char* output);
State handle_output(Event event, const char* input, char* output);
State handle_end(Event event, const char* input, char* output);

State (*state_handlers[MAX_STATES])(Event, const char*, char*) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_output,
    handle_end
};

State handle_start(Event event, const char* input, char* output) {
    if (event == EVENT_INPUT_RECEIVED) {
        printf("State machine started. Enter text to process:\n");
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(Event event, const char* input, char* output) {
    if (event == EVENT_INPUT_RECEIVED) {
        if (input != NULL && strlen(input) > 0 && strlen(input) < MAX_INPUT_LEN) {
            strncpy(output, input, MAX_INPUT_LEN - 1);
            output[MAX_INPUT_LEN - 1] = '\0';
            return STATE_PROCESSING;
        } else {
            printf("Invalid input. Please enter text (1-%d characters):\n", MAX_INPUT_LEN - 1);
            return STATE_READING;
        }
    }
    return STATE_READING;
}

State handle_processing(Event event, const char* input, char* output) {
    if (event == EVENT_PROCESS_COMPLETE) {
        if (output != NULL) {
            size_t len = strlen(output);
            for (size_t i = 0; i < len; i++) {
                if (islower((unsigned char)output[i])) {
                    output[i] = toupper((unsigned char)output[i]);
                } else if (isupper((unsigned char)output[i])) {
                    output[i] = tolower((unsigned char)output[i]);
                }
            }
            return STATE_OUTPUT;
        }
    }
    return STATE_PROCESSING;
}

State handle_output(Event event, const char* input, char* output) {
    if (event == EVENT_OUTPUT_SENT) {
        if (output != NULL) {
            printf("Processed text: %s\n", output);
            printf("Continue? (y/n): ");
            return STATE_READING;
        }
    }
    return STATE_OUTPUT;
}

State handle_end(Event event, const char* input, char* output) {
    if (event == EVENT_TERMINATE) {
        printf("State machine terminated.\n");
        return STATE_END;
    }
    return STATE_END;
}

int main(void) {
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_INPUT_LEN];
    Event current_event = EVENT_INPUT_RECEIVED;
    
    while (current_state != STATE_END) {
        switch (current_state) {
            case STATE_START:
                current_state = state_handlers[STATE_START](current_event, NULL, NULL);
                current_event = EVENT_INPUT_RECEIVED;
                break;
                
            case STATE_READING:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    size_t len = strlen(input_buffer);
                    if (len > 0 && input_buffer[len - 1] == '\n') {
                        input_buffer[len - 1] = '\0';
                    }
                    current_state = state_handlers[STATE_READING](EVENT_INPUT_RECEIVED, input_buffer, output_buffer);
                    if (current_state == STATE_PROCESSING) {
                        current_event = EVENT_PROCESS_COMPLETE;
                    }
                } else {
                    current_event = EVENT_TERMINATE;
                    current_state = STATE_END;
                }
                break;
                
            case STATE_PROCESSING:
                current_state = state_handlers[STATE_PROCESSING](current_event, NULL, output_buffer);
                if (current_state == STATE_OUTPUT) {
                    current_event = EVENT_OUTPUT_SENT;
                }
                break;
                
            case STATE_OUTPUT:
                current_state = state_handlers[STATE_OUTPUT](current_event, NULL, output_buffer);
                if (current_state == STATE_READING) {
                    char response[10];
                    if (fgets(response, sizeof(response), stdin) != NULL) {
                        if (response[0] == 'n' || response[0] == 'N') {
                            current_event = EVENT_TERMINATE;
                            current_state = STATE_END;
                        } else if (response[0] == 'y' || response[0] == 'Y') {
                            current_event = EVENT_INPUT_RECEIVED;
                            printf("Enter text to process:\n");
                        } else {
                            printf("Invalid response. Continue? (y/n): ");
                            current_state =