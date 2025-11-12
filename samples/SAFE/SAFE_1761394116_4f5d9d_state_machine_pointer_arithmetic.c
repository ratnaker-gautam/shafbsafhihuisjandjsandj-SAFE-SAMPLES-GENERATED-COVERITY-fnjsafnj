//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    EVENT_NONE,
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_FINALIZE_COMPLETE,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_length;
    int processed_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_finalizing_state(StateMachine *sm, Event event);
State handle_done_state(StateMachine *sm, Event event);

State (*state_handlers[STATE_COUNT])(StateMachine *, Event) = {
    handle_start_state,
    handle_reading_state,
    handle_processing_state,
    handle_finalizing_state,
    handle_done_state
};

State handle_start_state(StateMachine *sm, Event event) {
    if (event == EVENT_NONE) {
        printf("State machine initialized. Enter text to process:\n");
        sm->input_length = 0;
        sm->processed_count = 0;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (event == EVENT_INPUT_RECEIVED) {
        if (sm->input_length > 0 && sm->input_length < MAX_INPUT_LEN) {
            return STATE_PROCESSING;
        }
    }
    
    char *buffer_ptr = sm->input_buffer;
    if (fgets(buffer_ptr, MAX_INPUT_LEN, stdin) != NULL) {
        char *newline_ptr = buffer_ptr;
        while (*newline_ptr != '\0' && *newline_ptr != '\n') {
            newline_ptr++;
        }
        if (*newline_ptr == '\n') {
            *newline_ptr = '\0';
        }
        
        sm->input_length = newline_ptr - buffer_ptr;
        if (sm->input_length > 0) {
            return STATE_PROCESSING;
        }
    }
    return STATE_READING;
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        return STATE_FINALIZING;
    }
    
    if (sm->input_length > 0) {
        char *start_ptr = sm->input_buffer;
        char *end_ptr = start_ptr + sm->input_length - 1;
        int count = 0;
        
        while (start_ptr < end_ptr) {
            char temp = *start_ptr;
            *start_ptr = *end_ptr;
            *end_ptr = temp;
            start_ptr++;
            end_ptr--;
            count++;
        }
        
        sm->processed_count = count;
        printf("Processed string: %s (swapped %d character pairs)\n", sm->input_buffer, count);
        return STATE_FINALIZING;
    }
    return STATE_PROCESSING;
}

State handle_finalizing_state(StateMachine *sm, Event event) {
    if (event == EVENT_FINALIZE_COMPLETE) {
        return STATE_DONE;
    }
    
    printf("Processing complete. Characters swapped: %d\n", sm->processed_count);
    printf("Continue? (y/n): ");
    
    char response[16];
    if (fgets(response, sizeof(response), stdin) != NULL) {
        char *resp_ptr = response;
        while (*resp_ptr != '\0' && *resp_ptr != '\n') {
            resp_ptr++;
        }
        if (response[0] == 'y' || response[0] == 'Y') {
            sm->input_length = 0;
            return STATE_READING;
        }
    }
    return STATE_DONE;
}

State handle_done_state(StateMachine *sm, Event event) {
    printf("State machine terminated.\n");
    return STATE_DONE;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_START;
    sm.input_length = 0;
    sm.processed_count = 0;
    
    while (sm.current_state != STATE_DONE) {
        Event current_event = EVENT_NONE;
        
        if (sm.current_state == STATE_READING) {
            current_event = EVENT_INPUT_RECEIVED;
        } else if (sm.current_state == STATE_PROCESSING) {
            current_event = EVENT_PROCESS_COMPLETE;
        } else if (sm.current_state == STATE_FINALIZING) {
            current_event = EVENT_FINALIZE_COMPLETE;
        }
        
        State next_state = state_handlers[sm.current_state](&sm, current_event);
        sm.current_state = next_state;
    }
    
    return 0;
}