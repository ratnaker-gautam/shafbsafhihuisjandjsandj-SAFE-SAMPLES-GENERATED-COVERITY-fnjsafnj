//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
    uint32_t data_value;
    uint8_t buffer[16];
    size_t buffer_pos;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
}

enum State handle_idle(struct StateMachine *sm, enum Event event) {
    switch (event) {
        case EVENT_START:
            printf("Starting data processing\n");
            return STATE_READING;
        case EVENT_RESET:
            printf("Already in idle state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for idle state\n");
            return STATE_IDLE;
    }
}

enum State handle_reading(struct StateMachine *sm, enum Event event) {
    switch (event) {
        case EVENT_DATA_READY:
            if (sm->buffer_pos < sizeof(sm->buffer)) {
                sm->buffer[sm->buffer_pos] = (uint8_t)(sm->data_value % 256);
                sm->buffer_pos++;
                sm->data_value++;
                printf("Read data byte: %u\n", sm->buffer[sm->buffer_pos - 1]);
                if (sm->buffer_pos >= 8) {
                    return STATE_PROCESSING;
                }
                return STATE_READING;
            } else {
                printf("Buffer full during reading\n");
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            printf("Error during reading\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during reading\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for reading state\n");
            return STATE_READING;
    }
}

enum State handle_processing(struct StateMachine *sm, enum Event event) {
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            printf("Processing complete, data transformed\n");
            for (size_t i = 0; i < sm->buffer_pos; i++) {
                sm->buffer[i] = sm->buffer[i] ^ 0xFF;
            }
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during processing\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for processing state\n");
            return STATE_PROCESSING;
    }
}

enum State handle_writing(struct StateMachine *sm, enum Event event) {
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Writing complete. Data written: ");
            for (size_t i = 0; i < sm->buffer_pos; i++) {
                printf("%02X ", sm->buffer[i]);
            }
            printf("\n");
            return STATE_DONE;
        case EVENT_ERROR:
            printf("Error during writing\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during writing\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for writing state\n");
            return STATE_WRITING;
    }
}

enum State handle_error(struct StateMachine *sm, enum Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return STATE_IDLE;
        default:
            printf("Must reset from error state\n");
            return STATE_ERROR;
    }
}

enum State handle_done(struct StateMachine *sm, enum Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from done state\n");
            return STATE_IDLE;
        default:
            printf("Must reset from done state\n");
            return STATE_DONE;
    }
}

void state_machine_step(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return;
    
    enum State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            next_state = handle_idle(sm, event);
            break;
        case STATE_READING:
            next_state = handle_reading(sm, event);
            break;
        case STATE_PROCESSING:
            next_state = handle_processing(sm, event);
            break;
        case STATE_WRITING:
            next_state = handle_writing(sm, event);
            break;
        case STATE_ERROR:
            next_state = handle_error(sm, event);
            break;
        case STATE_DONE:
            next_state = handle_done(sm, event);
            break;
    }
    
    sm->current_state = next_state;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo - Enter commands: start, read, process, write, error, reset, quit\n");
    
    char input[32];
    while (1) {
        printf("Current state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READ