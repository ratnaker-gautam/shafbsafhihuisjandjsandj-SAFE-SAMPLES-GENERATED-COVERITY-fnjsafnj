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
    uint32_t processed_value;
    uint8_t error_count;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->error_count = 0;
}

enum State handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    enum State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            switch (event) {
                case EVENT_START:
                    next_state = STATE_READING;
                    break;
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    break;
            }
            break;
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA_READY:
                    if (sm->data_value < 1000) {
                        next_state = STATE_PROCESSING;
                    } else {
                        next_state = STATE_ERROR;
                    }
                    break;
                case EVENT_ERROR:
                    next_state = STATE_ERROR;
                    break;
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    break;
            }
            break;
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS_COMPLETE:
                    if (sm->processed_value > 0 && sm->processed_value < 10000) {
                        next_state = STATE_WRITING;
                    } else {
                        next_state = STATE_ERROR;
                    }
                    break;
                case EVENT_ERROR:
                    next_state = STATE_ERROR;
                    break;
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    break;
            }
            break;
            
        case STATE_WRITING:
            switch (event) {
                case EVENT_WRITE_COMPLETE:
                    next_state = STATE_DONE;
                    break;
                case EVENT_ERROR:
                    next_state = STATE_ERROR;
                    break;
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    break;
            }
            break;
            
        case STATE_ERROR:
            switch (event) {
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    break;
            }
            break;
            
        case STATE_DONE:
            switch (event) {
                case EVENT_RESET:
                    state_machine_init(sm);
                    break;
                default:
                    break;
            }
            break;
    }
    
    return next_state;
}

void simulate_data_processing(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_READING:
            sm->data_value = 42;
            sm->current_state = handle_event(sm, EVENT_DATA_READY);
            break;
            
        case STATE_PROCESSING:
            if (sm->data_value > 0 && sm->data_value < 1000) {
                sm->processed_value = sm->data_value * 10;
                sm->current_state = handle_event(sm, EVENT_PROCESS_COMPLETE);
            } else {
                sm->current_state = handle_event(sm, EVENT_ERROR);
            }
            break;
            
        case STATE_WRITING:
            if (sm->processed_value > 0 && sm->processed_value < 10000) {
                printf("Writing value: %u\n", sm->processed_value);
                sm->current_state = handle_event(sm, EVENT_WRITE_COMPLETE);
            } else {
                sm->current_state = handle_event(sm, EVENT_ERROR);
            }
            break;
            
        default:
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("Starting state machine simulation\n");
    
    sm.current_state = handle_event(&sm, EVENT_START);
    
    int step = 0;
    while (sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR && step < 10) {
        simulate_data_processing(&sm);
        step++;
        
        if (sm.current_state == STATE_ERROR) {
            printf("Error occurred at step %d\n", step);
            sm.current_state = handle_event(&sm, EVENT_RESET);
            break;
        }
    }
    
    if (sm.current_state == STATE_DONE) {
        printf("State machine completed successfully\n");
    } else if (step >= 10) {
        printf("State machine timeout\n");
    }
    
    return 0;
}