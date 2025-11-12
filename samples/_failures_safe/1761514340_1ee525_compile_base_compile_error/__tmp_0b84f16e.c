//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t error_count;
    char buffer[256];
    size_t buffer_len;
};

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
    int (*action)(struct StateMachine*);
};

int action_idle_start(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_count = 0;
    sm->error_count = 0;
    sm->buffer_len = 0;
    printf("State machine initialized\n");
    return 1;
}

int action_reading_data(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Enter data to process: ");
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        return 0;
    }
    sm->buffer_len = strlen(sm->buffer);
    if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
        sm->buffer[sm->buffer_len - 1] = '\0';
        sm->buffer_len--;
    }
    if (sm->buffer_len == 0) {
        return 0;
    }
    sm->data_count++;
    return 1;
}

int action_processing_complete(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    printf("Processed data: %s\n", sm->buffer);
    return 1;
}

int action_writing_complete(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Data written successfully. Total processed: %u\n", sm->data_count);
    return 1;
}

int action_error(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->error_count++;
    printf("Error occurred. Total errors: %u\n", sm->error_count);
    return 1;
}

int action_reset(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_count = 0;
    sm->error_count = 0;
    sm->buffer_len = 0;
    printf("State machine reset\n");
    return 1;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_start},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_data},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_complete},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING, action_writing_complete},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset},
    {STATE_IDLE, EVENT_RESET, STATE_IDLE, action_reset}
};

const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return 0;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                if (!transitions[i].action(sm)) {
                    return 0;
                }
            }
            sm->current_state = transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("1. START\n");
    printf("2. DATA_READY\n");
    printf("3. PROCESS_COMPLETE\n");
    printf("4. WRITE_COMPLETE\n");
    printf("5. ERROR\n");
    printf("6. RESET\n");
    printf("0. EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_count = 0;
    sm.error_count = 0;
    sm.buffer_len = 0;
    
    int running = 1;
    
    while (running) {
        printf("\nCurrent state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESS