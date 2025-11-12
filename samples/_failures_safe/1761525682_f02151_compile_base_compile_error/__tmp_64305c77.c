//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_EVENTS 10

typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED,
    STATE_ERROR
} system_state_t;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_STOP,
    EVENT_ERROR,
    EVENT_RESET
} system_event_t;

typedef system_state_t (*state_handler_t)(system_event_t);

system_state_t handle_idle(system_event_t event);
system_state_t handle_running(system_event_t event);
system_state_t handle_paused(system_event_t event);
system_state_t handle_stopped(system_event_t event);
system_state_t handle_error(system_event_t event);

state_handler_t state_handlers[] = {
    handle_idle,
    handle_running,
    handle_paused,
    handle_stopped,
    handle_error
};

system_state_t current_state = STATE_IDLE;

system_state_t handle_idle(system_event_t event) {
    switch (event) {
        case EVENT_START:
            printf("System starting...\n");
            return STATE_RUNNING;
        case EVENT_RESET:
            printf("System already idle.\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for idle state.\n");
            return STATE_IDLE;
    }
}

system_state_t handle_running(system_event_t event) {
    switch (event) {
        case EVENT_PAUSE:
            printf("System pausing...\n");
            return STATE_PAUSED;
        case EVENT_STOP:
            printf("System stopping...\n");
            return STATE_STOPPED;
        case EVENT_ERROR:
            printf("System error occurred.\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for running state.\n");
            return STATE_RUNNING;
    }
}

system_state_t handle_paused(system_event_t event) {
    switch (event) {
        case EVENT_RESUME:
            printf("System resuming...\n");
            return STATE_RUNNING;
        case EVENT_STOP:
            printf("System stopping...\n");
            return STATE_STOPPED;
        case EVENT_ERROR:
            printf("System error occurred.\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for paused state.\n");
            return STATE_PAUSED;
    }
}

system_state_t handle_stopped(system_event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("System resetting...\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for stopped state.\n");
            return STATE_STOPPED;
    }
}

system_state_t handle_error(system_event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("System resetting from error...\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for error state.\n");
            return STATE_ERROR;
    }
}

system_event_t parse_event(const char* input) {
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "pause") == 0) return EVENT_PAUSE;
    if (strcmp(input, "resume") == 0) return EVENT_RESUME;
    if (strcmp(input, "stop") == 0) return EVENT_STOP;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    return -1;
}

void print_state(system_state_t state) {
    switch (state) {
        case STATE_IDLE: printf("Current state: IDLE\n"); break;
        case STATE_RUNNING: printf("Current state: RUNNING\n"); break;
        case STATE_PAUSED: printf("Current state: PAUSED\n"); break;
        case STATE_STOPPED: printf("Current state: STOPPED\n"); break;
        case STATE_ERROR: printf("Current state: ERROR\n"); break;
        default: printf("Current state: UNKNOWN\n"); break;
    }
}

void print_help(void) {
    printf("Available commands: start, pause, resume, stop, error, reset, quit\n");
}

int main(void) {
    char input[MAX_INPUT_LEN];
    system_event_t event;
    
    printf("State Machine System\n");
    printf("Type 'help' for available commands or 'quit' to exit.\n");
    
    while (1) {
        print_state(current_state);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error.\n");
            continue;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (strcmp(input, "quit") == 0) {
            printf("Exiting system.\n");
            break;
        }
        
        if (strcmp(input, "help") == 0) {
            print_help();
            continue;
        }
        
        event = parse_event(input);
        if (event < 0 || event >= (sizeof(state_handlers