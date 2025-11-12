//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 20

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_READING},
    {STATE_READING, EVENT_PROCESS, STATE_PROCESSING},
    {STATE_PROCESSING, EVENT_COMPLETE, STATE_DONE},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_ERROR, EVENT_START, STATE_READING},
    {STATE_DONE, EVENT_START, STATE_READING}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    sm->data_count = 0;
}

State state_machine_handle_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == sm->current_state && 
            transitions[i].event == event) {
            
            State next_state = transitions[i].next_state;
            
            switch (event) {
                case EVENT_START:
                    sm->buffer[0] = '\0';
                    sm->buffer_len = 0;
                    sm->data_count = 0;
                    break;
                    
                case EVENT_DATA:
                    if (data != NULL && sm->buffer_len < MAX_INPUT_LEN - 1) {
                        size_t data_len = strlen(data);
                        if (data_len > 0 && data_len < MAX_INPUT_LEN - sm->buffer_len) {
                            strcat(sm->buffer, data);
                            sm->buffer_len += data_len;
                            sm->data_count++;
                        }
                    }
                    break;
                    
                case EVENT_PROCESS:
                    if (sm->buffer_len > 0) {
                        for (size_t j = 0; j < sm->buffer_len; j++) {
                            sm->buffer[j] = toupper((unsigned char)sm->buffer[j]);
                        }
                    }
                    break;
                    
                case EVENT_COMPLETE:
                    printf("Processing complete. Processed %d data items.\n", sm->data_count);
                    break;
                    
                case EVENT_ERROR:
                    printf("Error occurred in processing.\n");
                    break;
            }
            
            sm->current_state = next_state;
            return next_state;
        }
    }
    
    return sm->current_state;
}

void print_state_info(State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "DONE", "ERROR"
    };
    
    if (state < STATE_IDLE || state > STATE_ERROR) {
        printf("Unknown state\n");
        return;
    }
    
    printf("Current state: %s\n", state_names[state]);
}

int get_user_choice(void) {
    char input[10];
    int choice = -1;
    
    printf("\nAvailable events:\n");
    printf("1. START\n");
    printf("2. DATA\n");
    printf("3. PROCESS\n");
    printf("4. COMPLETE\n");
    printf("5. ERROR\n");
    printf("0. EXIT\n");
    printf("Enter choice: ");
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (sscanf(input, "%d", &choice) == 1) {
            return choice;
        }
    }
    
    return -1;
}

int main(void) {
    StateMachine sm;
    int running = 1;
    
    state_machine_init(&sm);
    
    printf("State Machine Demo - Text Processing\n");
    
    while (running) {
        print_state_info(sm.current_state);
        
        if (sm.current_state == STATE_DONE || sm.current_state == STATE_ERROR) {
            printf("Buffer content: %s\n", sm.buffer);
        }
        
        int choice = get_user_choice();
        
        switch (choice) {
            case 0:
                running = 0;
                break;
                
            case 1:
                state_machine_handle_event(&sm, EVENT_START, NULL);
                break;
                
            case 2:
                if (sm.current_state == STATE_READING) {
                    char data[MAX_INPUT_LEN];
                    printf("Enter data to add: ");
                    if (fgets(data, sizeof(data), stdin) != NULL) {
                        size_t len = strlen(data);
                        if (len > 0 && data[len-1] == '\n') {
                            data[len-1] = '\0';