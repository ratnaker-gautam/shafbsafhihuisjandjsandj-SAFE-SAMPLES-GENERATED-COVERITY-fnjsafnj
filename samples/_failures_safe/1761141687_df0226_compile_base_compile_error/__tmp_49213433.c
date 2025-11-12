//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
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
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_event(StateMachine* sm, Event event, const char* data) {
    switch (sm->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                sm->buffer_len = 0;
                sm->data_count = 0;
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA && data != NULL) {
                size_t data_len = strlen(data);
                if (data_len > 0 && sm->buffer_len + data_len < MAX_INPUT_LEN) {
                    memcpy(sm->buffer + sm->buffer_len, data, data_len);
                    sm->buffer_len += data_len;
                    sm->data_count++;
                    return STATE_READING;
                } else if (event == EVENT_PROCESS && sm->buffer_len > 0) {
                    return STATE_PROCESSING;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_COMPLETE) {
                return STATE_FINISHED;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_FINISHED:
        case STATE_ERROR:
            if (event == EVENT_START) {
                return STATE_START;
            }
            break;
    }
    
    return sm->current_state;
}

void process_data(StateMachine* sm) {
    if (sm->buffer_len == 0) {
        return;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
}

void print_state_info(StateMachine* sm) {
    const char* state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    printf("Current state: %s\n", state_names[sm->current_state]);
    printf("Data count: %d\n", sm->data_count);
    printf("Buffer length: %zu\n", sm->buffer_len);
    
    if (sm->buffer_len > 0 && sm->buffer_len < MAX_INPUT_LEN) {
        printf("Buffer content: ");
        for (size_t i = 0; i < sm->buffer_len; i++) {
            putchar(sm->buffer[i]);
        }
        putchar('\n');
    }
}

int get_user_choice(void) {
    char input[10];
    int choice = -1;
    
    printf("\nOptions:\n");
    printf("1. Start/Reset\n");
    printf("2. Add data\n");
    printf("3. Process data\n");
    printf("4. Complete processing\n");
    printf("5. Simulate error\n");
    printf("6. Exit\n");
    printf("Enter choice: ");
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (sscanf(input, "%d", &choice) == 1) {
            return choice;
        }
    }
    
    return -1;
}

int main(void) {
    StateMachine sm = {STATE_START, {0}, 0, 0};
    int running = 1;
    
    printf("State Machine Demo\n");
    
    while (running) {
        print_state_info(&sm);
        
        int choice = get_user_choice();
        if (choice < 1 || choice > 6) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                sm.current_state = handle_event(&sm, EVENT_START, NULL);
                break;
                
            case 2:
                if (sm.current_state == STATE_READING) {
                    char data[MAX_INPUT_LEN];
                    printf("Enter data to add: ");
                    if (fgets(data, sizeof(data), stdin) != NULL) {
                        size_t len = strlen(data);
                        if (len > 0 && data[len-1] == '\n') {
                            data[len-1] = '\0';
                            len--;
                        }
                        if (len > 0) {
                            sm.current_state = handle_event(&sm, EVENT_DATA, data);
                        }
                    }
                } else {
                    printf("Cannot add data in current state.\n");
                }
                break;
                
            case 3:
                if (sm.current_state == STATE_READING) {
                    sm.current_state = handle_event(&sm, EVENT_PROCESS, NULL);
                    if (sm.current_state == STATE_PROCESSING) {
                        process_data(&sm);
                    }
                } else {
                    printf("Cannot process data in current state.\n");
                }
                break;
                
            case 4:
                if (sm.current_state == STATE_PROC