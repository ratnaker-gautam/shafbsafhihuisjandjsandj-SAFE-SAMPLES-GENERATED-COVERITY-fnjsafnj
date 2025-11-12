//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 20

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
    EVENT_FINISH,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

static Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_READING},
    {STATE_READING, EVENT_PROCESS, STATE_PROCESSING},
    {STATE_PROCESSING, EVENT_FINISH, STATE_COMPLETE},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_READING, EVENT_ERROR, STATE_ERROR},
    {STATE_ERROR, EVENT_START, STATE_READING}
};
static const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static State current_state = STATE_IDLE;
static char buffer[MAX_INPUT_LEN + 1];
static size_t buffer_pos = 0;

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i])) return 0;
    }
    return 1;
}

static State handle_event(Event event, const char* data) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current_state && 
            transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return current_state;
}

static void process_buffer(void) {
    if (buffer_pos == 0) return;
    
    int sum = 0;
    for (size_t i = 0; i < buffer_pos; i++) {
        if (buffer[i] >= '0' && buffer[i] <= '9') {
            sum += buffer[i] - '0';
        }
    }
    printf("Processed data: sum of digits = %d\n", sum);
}

static void reset_buffer(void) {
    memset(buffer, 0, sizeof(buffer));
    buffer_pos = 0;
}

static void run_state_machine(void) {
    char input[MAX_INPUT_LEN + 2];
    
    printf("State Machine Controller\n");
    printf("Commands: start, data <text>, process, finish, error, quit\n");
    
    while (1) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\n> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (!validate_input(input)) {
            printf("Invalid input\n");
            continue;
        }
        
        Event event;
        char* data = NULL;
        
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strncmp(input, "data ", 5) == 0) {
            event = EVENT_DATA;
            data = input + 5;
            if (strlen(data) == 0) {
                printf("Data cannot be empty\n");
                continue;
            }
        } else if (strcmp(input, "process") == 0) {
            event = EVENT_PROCESS;
        } else if (strcmp(input, "finish") == 0) {
            event = EVENT_FINISH;
        } else if (strcmp(input, "error") == 0) {
            event = EVENT_ERROR;
        } else if (strcmp(input, "quit") == 0) {
            break;
        } else {
            printf("Unknown command\n");
            continue;
        }
        
        State new_state = handle_event(event, data);
        
        if (event == EVENT_DATA && current_state == STATE_READING) {
            size_t data_len = strlen(data);
            if (buffer_pos + data_len < MAX_INPUT_LEN) {
                strncpy(buffer + buffer_pos, data, data_len);
                buffer_pos += data_len;
                buffer[buffer_pos] = '\0';
                printf("Data stored: %s\n", data);
            } else {
                printf("Buffer full\n");
                new_state = STATE_ERROR;
            }
        }
        
        if (event == EVENT_PROCESS && new_state == STATE_PROCESSING) {
            process_buffer();
        }
        
        if (event == EVENT_START && new_state == STATE_READING) {
            reset_buffer();
        }
        
        current_state = new_state;
        
        if (current_state == STATE_COMPLETE) {
            printf("Operation completed successfully\n");
            current_state = STATE_ID