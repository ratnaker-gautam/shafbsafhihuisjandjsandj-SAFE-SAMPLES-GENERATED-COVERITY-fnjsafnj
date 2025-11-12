//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define EVENT_START 0
#define EVENT_DATA 1
#define EVENT_PROCESS 2
#define EVENT_FINISH 3
#define EVENT_ERROR 4
#define EVENT_RESET 5

#define MAX_INPUT_LEN 100
#define MAX_DATA_LEN 50

typedef struct {
    int current_state;
    char data_buffer[MAX_DATA_LEN];
    size_t data_len;
} state_machine_t;

static int state_transition(int current_state, int event) {
    static const int transition_table[5][6] = {
        {STATE_READING, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE},
        {STATE_ERROR, STATE_READING, STATE_PROCESSING, STATE_DONE, STATE_ERROR, STATE_IDLE},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_DONE, STATE_ERROR, STATE_IDLE},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE}
    };
    
    if (current_state < 0 || current_state >= 5) return STATE_ERROR;
    if (event < 0 || event >= 6) return STATE_ERROR;
    
    return transition_table[current_state][event];
}

static void state_machine_init(state_machine_t *sm) {
    if (!sm) return;
    sm->current_state = STATE_IDLE;
    memset(sm->data_buffer, 0, MAX_DATA_LEN);
    sm->data_len = 0;
}

static int state_machine_handle_event(state_machine_t *sm, int event, const char *data) {
    if (!sm) return -1;
    if (event < 0 || event >= 6) return -1;
    
    int new_state = state_transition(sm->current_state, event);
    if (new_state == STATE_ERROR) return -1;
    
    switch (event) {
        case EVENT_START:
            if (sm->current_state != STATE_IDLE) return -1;
            memset(sm->data_buffer, 0, MAX_DATA_LEN);
            sm->data_len = 0;
            break;
            
        case EVENT_DATA:
            if (sm->current_state != STATE_READING) return -1;
            if (!data) return -1;
            size_t data_len = strlen(data);
            if (data_len == 0 || data_len >= MAX_DATA_LEN) return -1;
            if (sm->data_len + data_len >= MAX_DATA_LEN) return -1;
            memcpy(sm->data_buffer + sm->data_len, data, data_len);
            sm->data_len += data_len;
            break;
            
        case EVENT_PROCESS:
            if (sm->current_state != STATE_READING) return -1;
            if (sm->data_len == 0) return -1;
            for (size_t i = 0; i < sm->data_len; i++) {
                sm->data_buffer[i] = toupper(sm->data_buffer[i]);
            }
            break;
            
        case EVENT_FINISH:
            if (sm->current_state != STATE_PROCESSING) return -1;
            break;
            
        case EVENT_RESET:
            memset(sm->data_buffer, 0, MAX_DATA_LEN);
            sm->data_len = 0;
            break;
            
        default:
            return -1;
    }
    
    sm->current_state = new_state;
    return 0;
}

static void print_state_info(const state_machine_t *sm) {
    if (!sm) return;
    
    static const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "DONE", "ERROR"
    };
    
    static const char *event_names[] = {
        "START", "DATA", "PROCESS", "FINISH", "ERROR", "RESET"
    };
    
    printf("Current state: %s\n", state_names[sm->current_state]);
    if (sm->data_len > 0) {
        printf("Data buffer: %s\n", sm->data_buffer);
    }
    printf("Available events:\n");
    
    for (int i = 0; i < 6; i++) {
        int next_state = state_transition(sm->current_state, i);
        if (next_state != STATE_ERROR) {
            printf("  %d: %s -> %s\n", i, event_names[i], state_names[next_state]);
        }
    }
}

int main(void) {
    state_machine_t sm;
    state_machine_init(&sm);
    
    char input_buffer[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Enter commands: start, data <text>, process, finish, reset, quit\n");
    
    while (running) {
        print_state_info(&sm);
        printf("\nEnter command: ");
        
        if (!fgets(input_buffer, MAX_INPUT_LEN, stdin)) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        if (strcmp