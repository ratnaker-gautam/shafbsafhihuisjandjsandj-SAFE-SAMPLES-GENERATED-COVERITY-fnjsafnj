//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_pos;
    uint32_t processed_count;
};

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, enum Event event, const struct Transition *transitions, size_t num_transitions) {
    if (sm == NULL || transitions == NULL) return 0;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return 1;
        }
    }
    return 0;
}

int read_data(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->data_count >= sm->max_data) return 0;
    
    printf("Enter data item %u: ", sm->data_count + 1);
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) return 0;
    
    size_t len = strlen(sm->buffer);
    if (len > 0 && sm->buffer[len - 1] == '\n') {
        sm->buffer[len - 1] = '\0';
    }
    
    if (strlen(sm->buffer) == 0) return 0;
    
    sm->data_count++;
    return 1;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos >= sizeof(sm->buffer)) return 0;
    
    for (char *p = sm->buffer; *p != '\0'; p++) {
        if (isalpha((unsigned char)*p)) {
            *p = toupper((unsigned char)*p);
        }
    }
    
    sm->processed_count++;
    return 1;
}

int write_data(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    printf("Processed data: %s\n", sm->buffer);
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 1;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    const struct Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READING},
        {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
        {STATE_READING, EVENT_ERROR, STATE_ERROR},
        {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
        {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING},
        {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE},
        {STATE_READING, EVENT_RESET, STATE_IDLE},
        {STATE_PROCESSING, EVENT_RESET, STATE_IDLE},
        {STATE_WRITING, EVENT_RESET, STATE_IDLE}
    };
    const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    
    printf("State Machine Data Processor\n");
    printf("Commands: start, reset, quit\n");
    
    char input[64];
    int running = 1;
    
    while (running) {
        printf("Current state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            case STATE_DONE: printf("DONE"); break;
        }
        printf("\n> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
            continue;
        }
        
        enum Event event = EVENT_ERROR;
        
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "reset