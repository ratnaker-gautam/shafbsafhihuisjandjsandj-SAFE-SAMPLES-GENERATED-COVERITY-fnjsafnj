//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} event_t;

typedef state_t (*state_handler_t)(event_t);

static state_t handle_idle(event_t event);
static state_t handle_reading(event_t event);
static state_t handle_processing(event_t event);
static state_t handle_writing(event_t event);
static state_t handle_error(event_t event);

static state_handler_t state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

static const char* state_names[STATE_COUNT] = {
    "IDLE",
    "READING",
    "PROCESSING",
    "WRITING",
    "ERROR"
};

static const char* event_names[EVENT_COUNT] = {
    "START",
    "DATA_READY",
    "PROCESS_COMPLETE",
    "WRITE_COMPLETE",
    "ERROR",
    "RESET"
};

static state_t current_state = STATE_IDLE;
static uint32_t data_buffer[16];
static size_t data_count = 0;

static state_t handle_idle(event_t event) {
    if (event == EVENT_START) {
        printf("Starting data acquisition\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

static state_t handle_reading(event_t event) {
    if (event == EVENT_DATA_READY) {
        if (data_count < 16) {
            data_buffer[data_count] = data_count * 10;
            data_count++;
            printf("Read data point: %u\n", data_count * 10);
        }
        if (data_count >= 8) {
            printf("Data reading complete\n");
            return STATE_PROCESSING;
        }
        return STATE_READING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_READING;
}

static state_t handle_processing(event_t event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        uint32_t sum = 0;
        for (size_t i = 0; i < data_count; i++) {
            sum += data_buffer[i];
        }
        printf("Processing complete. Sum: %u\n", sum);
        return STATE_WRITING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

static state_t handle_writing(event_t event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Data written successfully\n");
        return STATE_IDLE;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

static state_t handle_error(event_t event) {
    if (event == EVENT_RESET) {
        data_count = 0;
        printf("System reset\n");
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

static event_t get_next_event(void) {
    static int event_sequence[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE
    };
    static size_t event_index = 0;
    
    if (event_index < sizeof(event_sequence) / sizeof(event_sequence[0])) {
        return event_sequence[event_index++];
    }
    return EVENT_RESET;
}

int main(void) {
    printf("State Machine Demo Started\n");
    
    for (int cycle = 0; cycle < 3; cycle++) {
        event_t event = get_next_event();
        
        if (event < 0 || event >= EVENT_COUNT) {
            printf("Invalid event detected\n");
            continue;
        }
        
        printf("Event: %s -> ", event_names[event]);
        
        state_t new_state = state_handlers[current_state](event);
        
        if (new_state < 0 || new_state >= STATE_COUNT) {
            printf("Invalid state transition\n");
            new_state = STATE_ERROR;
        }
        
        if (new_state != current_state) {
            printf("State: %s -> %s\n", state_names[current_state], state_names[new_state]);
            current_state = new_state;
        } else {
            printf("State: %s (unchanged)\n", state_names[current_state]);
        }
        
        if (current_state == STATE_ERROR && event == EVENT_RESET) {
            break;
        }
    }
    
    printf("State Machine Demo Completed\n");
    return 0;
}