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
    int data_buffer[16];
    int data_count;
    int processed_data[16];
    int processed_count;
} StateMachine;

typedef State (*StateHandler)(StateMachine*, Event);

State handle_idle(StateMachine* sm, Event event);
State handle_reading(StateMachine* sm, Event event);
State handle_processing(StateMachine* sm, Event event);
State handle_writing(StateMachine* sm, Event event);
State handle_error(StateMachine* sm, Event event);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

State handle_idle(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data processing pipeline\n");
            sm->data_count = 0;
            sm->processed_count = 0;
            return STATE_READING;
        case EVENT_RESET:
            printf("System reset\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            if (sm->data_count < 16) {
                sm->data_buffer[sm->data_count] = sm->data_count * 10;
                sm->data_count++;
                printf("Read data item %d\n", sm->data_count);
                
                if (sm->data_count >= 5) {
                    return STATE_PROCESSING;
                }
                return STATE_READING;
            } else {
                printf("Buffer full, moving to processing\n");
                return STATE_PROCESSING;
            }
        case EVENT_ERROR:
            printf("Error during data reading\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during reading\n");
            sm->data_count = 0;
            return STATE_IDLE;
        default:
            printf("Invalid event for READING state\n");
            return STATE_READING;
    }
}

State handle_processing(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (sm->data_count > 0 && sm->processed_count < 16) {
                for (int i = 0; i < sm->data_count; i++) {
                    if (sm->processed_count < 16) {
                        sm->processed_data[sm->processed_count] = sm->data_buffer[i] + 5;
                        sm->processed_count++;
                    }
                }
                printf("Processed %d items\n", sm->data_count);
                sm->data_count = 0;
                return STATE_WRITING;
            }
            return STATE_PROCESSING;
        case EVENT_ERROR:
            printf("Error during data processing\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during processing\n");
            sm->data_count = 0;
            sm->processed_count = 0;
            return STATE_IDLE;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

State handle_writing(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            if (sm->processed_count > 0) {
                printf("Writing processed data: ");
                for (int i = 0; i < sm->processed_count; i++) {
                    printf("%d ", sm->processed_data[i]);
                }
                printf("\n");
                sm->processed_count = 0;
                
                if (sm->data_count > 0) {
                    return STATE_PROCESSING;
                }
                return STATE_IDLE;
            }
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("Error during data writing\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during writing\n");
            sm->processed_count = 0;
            return STATE_IDLE;
        default:
            printf("Invalid event for WRITING state\n");
            return STATE_WRITING;
    }
}

State handle_error(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            sm->data_count = 0;
            sm->processed_count = 0;
            return STATE_IDLE;
        default:
            printf("System in error state - reset required\n");
            return STATE_ERROR;
    }
}

void process_event(StateMachine* sm, Event event) {
    if (sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return;
    }
    
    if (sm->current_state < 0 || sm->current_state >=