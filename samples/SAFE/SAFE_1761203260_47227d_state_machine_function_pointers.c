//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    INIT,
    READY,
    PROCESSING,
    FINISHED,
    ERROR,
    NUM_STATES
};

enum Event {
    START,
    DATA_RECEIVED,
    PROCESS_COMPLETE,
    ERROR_OCCURRED,
    RESET,
    NUM_EVENTS
};

typedef void (*StateHandler)(enum Event);

void init_handler(enum Event event);
void ready_handler(enum Event event);
void processing_handler(enum Event event);
void finished_handler(enum Event event);
void error_handler(enum Event event);

StateHandler state_table[NUM_STATES][NUM_EVENTS] = {
    {ready_handler, NULL, NULL, error_handler, NULL},
    {NULL, processing_handler, NULL, error_handler, init_handler},
    {NULL, NULL, finished_handler, error_handler, init_handler},
    {NULL, NULL, NULL, error_handler, init_handler},
    {NULL, NULL, NULL, NULL, init_handler}
};

enum State current_state = INIT;
int data_value = 0;
int process_count = 0;

void init_handler(enum Event event) {
    if (event == START) {
        printf("Initializing system...\n");
        data_value = 0;
        process_count = 0;
        current_state = READY;
        printf("System ready. Enter data values (0-100) or commands (start, reset, quit):\n");
    }
}

void ready_handler(enum Event event) {
    if (event == DATA_RECEIVED) {
        if (data_value >= 0 && data_value <= 100) {
            printf("Data received: %d\n", data_value);
            current_state = PROCESSING;
        } else {
            printf("Invalid data value: %d\n", data_value);
            current_state = ERROR;
        }
    } else if (event == ERROR_OCCURRED) {
        printf("Error in ready state\n");
        current_state = ERROR;
    } else if (event == RESET) {
        current_state = INIT;
    }
}

void processing_handler(enum Event event) {
    if (event == PROCESS_COMPLETE) {
        int result = data_value * 2;
        if (result > 200) {
            result = 200;
        }
        printf("Processing complete. Result: %d\n", result);
        process_count++;
        current_state = FINISHED;
    } else if (event == ERROR_OCCURRED) {
        printf("Error during processing\n");
        current_state = ERROR;
    } else if (event == RESET) {
        current_state = INIT;
    }
}

void finished_handler(enum Event event) {
    if (event == DATA_RECEIVED) {
        current_state = PROCESSING;
    } else if (event == ERROR_OCCURRED) {
        printf("Error in finished state\n");
        current_state = ERROR;
    } else if (event == RESET) {
        current_state = INIT;
    }
}

void error_handler(enum Event event) {
    if (event == RESET) {
        printf("Resetting from error state\n");
        current_state = INIT;
    } else {
        printf("System in error state. Use 'reset' to recover.\n");
    }
}

void handle_event(enum Event event) {
    if (current_state < NUM_STATES && event < NUM_EVENTS) {
        StateHandler handler = state_table[current_state][event];
        if (handler != NULL) {
            handler(event);
        } else {
            printf("Invalid event %d for current state %d\n", event, current_state);
        }
    }
}

enum Event parse_input(const char* input) {
    if (strcmp(input, "start") == 0) {
        return START;
    } else if (strcmp(input, "reset") == 0) {
        return RESET;
    } else if (strcmp(input, "quit") == 0) {
        return NUM_EVENTS;
    } else {
        char* endptr;
        long value = strtol(input, &endptr, 10);
        if (endptr != input && *endptr == '\0') {
            if (value >= 0 && value <= 100) {
                data_value = (int)value;
                return DATA_RECEIVED;
            }
        }
        return ERROR_OCCURRED;
    }
}

int main() {
    char input[64];
    
    printf("State Machine System\n");
    printf("Available commands: start, reset, quit, or enter data (0-100)\n");
    
    while (1) {
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        enum Event event = parse_input(input);
        
        if (event == NUM_EVENTS) {
            printf("Total processes completed: %d\n", process_count);
            printf("Goodbye!\n");
            break;
        }
        
        handle_event(event);
        
        if (current_state == PROCESSING) {
            printf("Processing data...\n");
            handle_event(PROCESS_COMPLETE);
        }
    }
    
    return 0;
}