//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

static const Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_READING},
    {STATE_READING, EVENT_PROCESS, STATE_PROCESSING},
    {STATE_READING, EVENT_ERROR, STATE_ERROR},
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, Event event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return current;
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') return 0;
    }
    return 1;
}

void process_data(const char* data) {
    if (data == NULL) return;
    size_t len = strlen(data);
    printf("Processing %zu characters: ", len);
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)data[i])) {
            printf("%c", toupper((unsigned char)data[i]));
        }
    }
    printf("\n");
}

Event get_user_event(State current_state) {
    printf("Current state: ");
    switch (current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf("\nAvailable events:\n");
    
    switch (current_state) {
        case STATE_IDLE:
            printf("1. START\n");
            break;
        case STATE_READING:
            printf("2. DATA\n3. PROCESS\n4. ERROR\n");
            break;
        case STATE_PROCESSING:
            printf("5. SUCCESS\n6. ERROR\n");
            break;
        case STATE_COMPLETE:
        case STATE_ERROR:
            printf("7. RESET\n");
            break;
    }
    
    printf("Enter choice: ");
    
    char buffer[10];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    char* endptr;
    long choice = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return EVENT_ERROR;
    }
    
    if (choice < 1 || choice > 7) {
        return EVENT_ERROR;
    }
    
    switch (choice) {
        case 1: return EVENT_START;
        case 2: return EVENT_DATA;
        case 3: return EVENT_PROCESS;
        case 4: return EVENT_ERROR;
        case 5: return EVENT_SUCCESS;
        case 6: return EVENT_ERROR;
        case 7: return EVENT_RESET;
        default: return EVENT_ERROR;
    }
}

int main(void) {
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 2];
    char data_buffer[MAX_INPUT_LEN + 1];
    data_buffer[0] = '\0';
    
    printf("State Machine Demo - Text Processor\n");
    printf("===================================\n");
    
    while (1) {
        Event event = get_user_event(current_state);
        State next_state = get_next_state(current_state, event);
        
        if (next_state == current_state && event != EVENT_DATA) {
            printf("Invalid transition!\n");
            continue;
        }
        
        switch (event) {
            case EVENT_START:
                printf("Starting data collection...\n");
                data_buffer[0] = '\0';
                break;
                
            case EVENT_DATA:
                printf("Enter data (max %d alphanumeric chars): ", MAX_INPUT_LEN);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error\n");
                    break;
                }
                
                size_t len = strlen(input_buffer);
                if (len > 0 && input_buffer[len - 1] == '\n') {
                    input_buffer[len - 1] = '\0