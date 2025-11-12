//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
#define EVENT_END 2
#define EVENT_RESET 3
#define EVENT_INVALID 4

#define MAX_INPUT_LEN 100
#define MAX_DATA_LEN 50

typedef struct {
    int state;
    char data[MAX_DATA_LEN];
    size_t data_len;
} StateMachine;

const char* state_names[] = {"IDLE", "READING", "PROCESSING", "DONE", "ERROR"};
const char* event_names[] = {"START", "DATA", "END", "RESET", "INVALID"};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') return 0;
    }
    return 1;
}

int process_event(StateMachine* sm, int event, const char* input, size_t len) {
    if (sm == NULL || !validate_input(input, len)) return STATE_ERROR;
    
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->data_len = 0;
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len + len < MAX_DATA_LEN) {
                    memcpy(sm->data + sm->data_len, input, len);
                    sm->data_len += len;
                    return STATE_READING;
                } else {
                    return STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    return STATE_PROCESSING;
                } else {
                    return STATE_ERROR;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                return STATE_IDLE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                return STATE_IDLE;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                return STATE_IDLE;
            }
            break;
    }
    
    return STATE_ERROR;
}

void process_data(StateMachine* sm) {
    if (sm == NULL || sm->state != STATE_PROCESSING) return;
    
    printf("Processing data: ");
    for (size_t i = 0; i < sm->data_len; i++) {
        if (isalpha((unsigned char)sm->data[i])) {
            printf("%c", toupper((unsigned char)sm->data[i]));
        } else if (isdigit((unsigned char)sm->data[i])) {
            printf("%c", sm->data[i]);
        }
    }
    printf("\n");
    sm->state = STATE_DONE;
}

int get_event_from_input(const char* input) {
    if (input == NULL) return EVENT_INVALID;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "end") == 0) return EVENT_END;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_INVALID;
}

int main(void) {
    StateMachine sm = {STATE_IDLE, {0}, 0};
    char input[MAX_INPUT_LEN];
    
    printf("State Machine Demo\n");
    printf("Available commands: start, data <text>, end, reset, quit\n");
    
    while (1) {
        printf("\nCurrent state: %s\n", state_names[sm.state]);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') input[--len] = '\0';
        if (len == 0) continue;
        
        if (strcmp(input, "quit") == 0) break;
        
        char* space = strchr(input, ' ');
        int event;
        const char* event_data = "";
        size_t data_len = 0;
        
        if (space != NULL) {
            *space = '\0';
            event_data = space + 1;
            data_len = strlen(event_data);
            event = get_event_from_input(input);
        } else {
            event = get_event_from_input(input);
        }
        
        if (event == EVENT_INVALID) {
            printf("Invalid command\n");
            continue;
        }
        
        int new_state = process_event(&sm, event, event_data, data_len);
        
        if (new_state == STATE_ERROR) {
            printf("Invalid transition from %s with event %s\n", 
                   state_names[sm.state], event_names[event]);
            sm.state = STATE_ERROR;
        } else {
            sm.state = new_state;
            printf("Transitioned to state: %s\n", state_names[sm.state]);
        }