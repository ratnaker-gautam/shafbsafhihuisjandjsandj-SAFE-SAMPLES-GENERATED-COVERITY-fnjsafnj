//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    int32_t *data_buffer;
    uint32_t process_counter;
};

struct StateMachine *create_state_machine(uint32_t max_data) {
    if (max_data == 0 || max_data > 1000) {
        return NULL;
    }
    
    struct StateMachine *sm = malloc(sizeof(struct StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    
    sm->data_buffer = malloc(max_data * sizeof(int32_t));
    if (sm->data_buffer == NULL) {
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = max_data;
    sm->process_counter = 0;
    
    return sm;
}

void destroy_state_machine(struct StateMachine *sm) {
    if (sm != NULL) {
        free(sm->data_buffer);
        free(sm);
    }
}

int add_data(struct StateMachine *sm, int32_t value) {
    if (sm == NULL || sm->current_state != STATE_READY) {
        return 0;
    }
    
    if (sm->data_count >= sm->max_data) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    
    sm->data_buffer[sm->data_count] = value;
    sm->data_count++;
    
    if (sm->data_count == sm->max_data) {
        sm->current_state = STATE_PROCESSING;
    }
    
    return 1;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) {
        return 0;
    }
    
    if (sm->process_counter >= sm->data_count) {
        sm->current_state = STATE_COMPLETE;
        return 0;
    }
    
    if (sm->data_buffer[sm->process_counter] > 0) {
        sm->data_buffer[sm->process_counter] *= 2;
    } else {
        sm->data_buffer[sm->process_counter] /= 2;
    }
    
    sm->process_counter++;
    
    if (sm->process_counter == sm->data_count) {
        sm->current_state = STATE_COMPLETE;
    }
    
    return 1;
}

void reset_state_machine(struct StateMachine *sm) {
    if (sm == NULL) {
        return;
    }
    
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->process_counter = 0;
}

int start_processing(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_IDLE) {
        return 0;
    }
    
    sm->current_state = STATE_READY;
    return 1;
}

void print_state_machine(const struct StateMachine *sm) {
    if (sm == NULL) {
        return;
    }
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("State: %s\n", state_names[sm->current_state]);
    printf("Data count: %u/%u\n", sm->data_count, sm->max_data);
    printf("Processed: %u\n", sm->process_counter);
    
    if (sm->data_count > 0) {
        printf("Data: ");
        for (uint32_t i = 0; i < sm->data_count; i++) {
            printf("%d ", sm->data_buffer[i]);
        }
        printf("\n");
    }
}

int main(void) {
    struct StateMachine *sm = create_state_machine(5);
    if (sm == NULL) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    printf("Initial state:\n");
    print_state_machine(sm);
    
    if (!start_processing(sm)) {
        printf("Failed to start processing\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("\nAfter starting:\n");
    print_state_machine(sm);
    
    int32_t test_data[] = {10, -5, 8, -3, 15};
    for (int i = 0; i < 5; i++) {
        if (!add_data(sm, test_data[i])) {
            printf("Failed to add data %d\n", test_data[i]);
            break;
        }
    }
    
    printf("\nAfter adding data:\n");
    print_state_machine(sm);
    
    while (process_data(sm)) {
    }
    
    printf("\nAfter processing:\n");
    print_state_machine(sm);
    
    reset_state_machine(sm);
    printf("\nAfter reset:\n");
    print_state_machine(sm);
    
    destroy_state_machine(sm);
    return 0;
}