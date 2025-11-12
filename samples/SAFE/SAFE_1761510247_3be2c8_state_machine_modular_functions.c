//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

State handle_start(Event event, const char* data);
State handle_reading(Event event, const char* data);
State handle_processing(Event event, const char* data);
State handle_finished(Event event, const char* data);
State handle_error(Event event, const char* data);

State (*state_handlers[])(Event, const char*) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

State current_state = STATE_START;
char buffer[MAX_INPUT_LEN + 1];
size_t buffer_pos = 0;

State handle_start(Event event, const char* data) {
    if (event == EVENT_START) {
        printf("State machine started.\n");
        buffer_pos = 0;
        memset(buffer, 0, sizeof(buffer));
        return STATE_READING;
    }
    return STATE_ERROR;
}

State handle_reading(Event event, const char* data) {
    if (event == EVENT_DATA && data != NULL) {
        size_t data_len = strlen(data);
        if (data_len > 0 && data_len <= MAX_INPUT_LEN - buffer_pos) {
            strncpy(buffer + buffer_pos, data, data_len);
            buffer_pos += data_len;
            buffer[buffer_pos] = '\0';
            printf("Data received: %s\n", data);
            printf("Buffer: %s\n", buffer);
            return STATE_READING;
        } else if (event == EVENT_PROCESS && buffer_pos > 0) {
            return STATE_PROCESSING;
        }
    }
    return STATE_ERROR;
}

State handle_processing(Event event, const char* data) {
    if (event == EVENT_PROCESS) {
        printf("Processing data: %s\n", buffer);
        for (size_t i = 0; i < buffer_pos; i++) {
            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                buffer[i] = buffer[i] - 'a' + 'A';
            }
        }
        printf("Processed data: %s\n", buffer);
        return STATE_FINISHED;
    }
    return STATE_ERROR;
}

State handle_finished(Event event, const char* data) {
    if (event == EVENT_COMPLETE) {
        printf("Processing complete.\n");
        return STATE_FINISHED;
    } else if (event == EVENT_RESET) {
        return STATE_START;
    }
    return STATE_ERROR;
}

State handle_error(Event event, const char* data) {
    if (event == EVENT_RESET) {
        printf("Error state reset.\n");
        return STATE_START;
    }
    printf("Error occurred.\n");
    return STATE_ERROR;
}

Event get_next_event(void) {
    static int step = 0;
    const char* input_options[] = {"hello", "world", "test", "data"};
    
    if (step == 0) {
        step++;
        return EVENT_START;
    } else if (step == 1 || step == 2) {
        step++;
        return EVENT_DATA;
    } else if (step == 3) {
        step++;
        return EVENT_PROCESS;
    } else if (step == 4) {
        step++;
        return EVENT_COMPLETE;
    } else {
        step = 0;
        return EVENT_RESET;
    }
}

const char* get_event_data(Event event) {
    static int data_index = 0;
    const char* data_items[] = {"hello", "world", "test", "data"};
    
    if (event == EVENT_DATA) {
        if (data_index < 4) {
            return data_items[data_index++];
        }
    }
    return NULL;
}

int main(void) {
    int cycles = 0;
    const int max_cycles = 3;
    
    while (cycles < max_cycles) {
        Event event = get_next_event();
        const char* data = NULL;
        
        if (event == EVENT_DATA) {
            data = get_event_data(event);
        }
        
        if (current_state < MAX_STATES && state_handlers[current_state] != NULL) {
            State new_state = state_handlers[current_state](event, data);
            if (new_state != current_state) {
                current_state = new_state;
            }
        } else {
            current_state = STATE_ERROR;
        }
        
        if (event == EVENT_RESET) {
            cycles++;
        }
        
        if (current_state == STATE_ERROR && event != EVENT_RESET) {
            current_state = state_handlers[STATE_ERROR](EVENT_RESET, NULL);
        }
    }
    
    return 0;
}