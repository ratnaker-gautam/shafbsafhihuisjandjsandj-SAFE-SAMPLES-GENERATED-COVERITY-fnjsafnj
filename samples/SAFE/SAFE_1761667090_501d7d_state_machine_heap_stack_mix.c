//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_EVENTS 10
#define BUFFER_SIZE 256

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PROCESSING,
    STATE_ERROR,
    STATE_DONE
} StateType;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_ERROR,
    EVENT_RESET
} EventType;

typedef struct {
    StateType current_state;
    EventType event;
    StateType next_state;
} Transition;

typedef struct {
    StateType state;
    char* name;
    int data_count;
    int* data_values;
} StateMachine;

StateMachine* create_state_machine(const char* name) {
    if (name == NULL || strlen(name) == 0) return NULL;
    
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->name = malloc(strlen(name) + 1);
    if (sm->name == NULL) {
        free(sm);
        return NULL;
    }
    strcpy(sm->name, name);
    
    sm->state = STATE_IDLE;
    sm->data_count = 0;
    sm->data_values = NULL;
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    
    free(sm->name);
    free(sm->data_values);
    free(sm);
}

int add_data_to_state(StateMachine* sm, int value) {
    if (sm == NULL) return 0;
    
    if (sm->data_count >= BUFFER_SIZE) return 0;
    
    int* new_data = realloc(sm->data_values, (sm->data_count + 1) * sizeof(int));
    if (new_data == NULL) return 0;
    
    sm->data_values = new_data;
    sm->data_values[sm->data_count] = value;
    sm->data_count++;
    
    return 1;
}

StateType process_event(StateMachine* sm, EventType event, Transition* transitions, int transition_count) {
    if (sm == NULL || transitions == NULL || transition_count <= 0) return sm ? sm->state : STATE_ERROR;
    
    for (int i = 0; i < transition_count; i++) {
        if (transitions[i].current_state == sm->state && transitions[i].event == event) {
            sm->state = transitions[i].next_state;
            break;
        }
    }
    
    return sm->state;
}

void print_state_info(const StateMachine* sm) {
    if (sm == NULL) return;
    
    const char* state_names[] = {"IDLE", "ACTIVE", "PROCESSING", "ERROR", "DONE"};
    printf("State Machine: %s\n", sm->name);
    printf("Current State: %s\n", state_names[sm->state]);
    printf("Data Count: %d\n", sm->data_count);
    
    if (sm->data_count > 0) {
        printf("Data Values: ");
        for (int i = 0; i < sm->data_count; i++) {
            printf("%d ", sm->data_values[i]);
        }
        printf("\n");
    }
}

int main() {
    Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_ACTIVE},
        {STATE_ACTIVE, EVENT_DATA, STATE_PROCESSING},
        {STATE_ACTIVE, EVENT_ERROR, STATE_ERROR},
        {STATE_PROCESSING, EVENT_SUCCESS, STATE_DONE},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE},
        {STATE_DONE, EVENT_RESET, STATE_IDLE}
    };
    int transition_count = sizeof(transitions) / sizeof(transitions[0]);
    
    StateMachine* sm = create_state_machine("DataProcessor");
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    printf("Initial State:\n");
    print_state_info(sm);
    
    printf("\nProcessing events...\n");
    
    process_event(sm, EVENT_START, transitions, transition_count);
    printf("After START: ");
    print_state_info(sm);
    
    add_data_to_state(sm, 42);
    add_data_to_state(sm, 100);
    
    process_event(sm, EVENT_DATA, transitions, transition_count);
    printf("After DATA: ");
    print_state_info(sm);
    
    process_event(sm, EVENT_SUCCESS, transitions, transition_count);
    printf("After SUCCESS: ");
    print_state_info(sm);
    
    process_event(sm, EVENT_RESET, transitions, transition_count);
    printf("After RESET: ");
    print_state_info(sm);
    
    destroy_state_machine(sm);
    
    return 0;
}