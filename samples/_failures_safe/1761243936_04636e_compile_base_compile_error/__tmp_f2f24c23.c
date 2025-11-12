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

typedef struct {
    State current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
} StateMachine;

typedef State (*StateHandler)(StateMachine*, Event);

State handle_idle(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            printf("Starting data processing\n");
            sm->buffer_len = 0;
            sm->data_value = 0;
            return STATE_READING;
        case EVENT_RESET:
            printf("Reset in idle state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for idle state\n");
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_DATA_READY:
            if (sm->buffer_len >= sizeof(sm->buffer)) {
                printf("Buffer overflow\n");
                return STATE_ERROR;
            }
            printf("Enter data (max 255 chars): ");
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
                sm->buffer_len = strlen(sm->buffer);
                if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
                    sm->buffer[sm->buffer_len - 1] = '\0';
                    sm->buffer_len--;
                }
                printf("Read %zu characters\n", sm->buffer_len);
                return STATE_PROCESSING;
            } else {
                printf("Read error\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("Reset during reading\n");
            sm->buffer_len = 0;
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Error during reading\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for reading state\n");
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->buffer_len == 0) {
                sm->data_value = 0;
            } else {
                int sum = 0;
                for (size_t i = 0; i < sm->buffer_len; i++) {
                    sum += (int)sm->buffer[i];
                }
                sm->data_value = sum;
            }
            printf("Processing complete. Data value: %d\n", sm->data_value);
            return STATE_WRITING;
        case EVENT_RESET:
            printf("Reset during processing\n");
            sm->buffer_len = 0;
            sm->data_value = 0;
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for processing state\n");
            return STATE_ERROR;
    }
}

State handle_writing(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_WRITE_COMPLETE:
            printf("Writing data value: %d\n", sm->data_value);
            printf("Data written successfully\n");
            return STATE_IDLE;
        case EVENT_RESET:
            printf("Reset during writing\n");
            sm->buffer_len = 0;
            sm->data_value = 0;
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Error during writing\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for writing state\n");
            return STATE_ERROR;
    }
}

State handle_error(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            sm->buffer_len = 0;
            sm->data_value = 0;
            return STATE_IDLE;
        default:
            printf("Can only reset from error state\n");
            return STATE_ERROR;
    }
}

StateMachine* create_state_machine(void) {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm != NULL) {
        sm->current_state = STATE_IDLE;
        sm->buffer_len = 0;
        sm->data_value = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
    }
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm != NULL) {
        free(sm);
    }
}

int main(void) {
    StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    printf("State Machine Demo\n