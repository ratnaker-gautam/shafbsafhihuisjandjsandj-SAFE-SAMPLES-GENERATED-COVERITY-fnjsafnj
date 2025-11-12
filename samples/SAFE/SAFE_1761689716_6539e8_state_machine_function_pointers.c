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
int data_count = 0;

State handle_idle(Event event) {
    if (event == EVENT_START) {
        printf("Starting data processing pipeline\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(Event event) {
    if (event == EVENT_DATA_READY) {
        if (data_count < 10) {
            data_buffer[data_count] = data_count * 10;
            data_count++;
            printf("Read data item: %d\n", data_buffer[data_count - 1]);
        }
        
        if (data_count >= 5) {
            printf("Finished reading data\n");
            return STATE_PROCESSING;
        }
        return STATE_READING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        printf("Processing data: ");
        for (int i = 0; i < data_count && i < 10; i++) {
            data_buffer[i] += 5;
            printf("%d ", data_buffer[i]);
        }
        printf("\n");
        return STATE_WRITING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Writing data: ");
        for (int i = 0; i < data_count && i < 10; i++) {
            printf("%d ", data_buffer[i]);
        }
        printf("\nPipeline complete\n");
        return STATE_IDLE;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(Event event) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine\n");
        data_count = 0;
        memset(data_buffer, 0, sizeof(data_buffer));
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    printf("Processing event: %s in state: %s\n", 
           event_names[event], state_names[current_state]);
    
    State new_state = state_handlers[current_state](event);
    
    if (new_state < 0 || new_state >= STATE_COUNT) {
        printf("Invalid state transition\n");
        return;
    }
    
    if (new_state != current_state) {
        printf("State transition: %s -> %s\n", 
               state_names[current_state], state_names[new_state]);
        current_state = new_state;
    }
}

int main() {
    printf("State Machine Demo - Data Processing Pipeline\n");
    printf("Available commands: 0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET, 6=EXIT\n");
    
    int running = 1;
    while (running) {
        printf("Enter command: ");
        
        int command;
        if (scanf("%d", &command) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (command < 0 || command > 6) {
            printf("Invalid command. Use 0-6.\n");
            continue;
        }
        
        if (command == 6) {
            running = 0;
            continue;
        }
        
        process_event((Event)command);
    }
    
    printf("State machine terminated\n");
    return 0;
}