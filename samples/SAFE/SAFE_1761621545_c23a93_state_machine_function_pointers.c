//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, RUNNING, PAUSED, STOPPED, ERROR };

typedef void (*StateHandler)(enum State*);

void handle_idle(enum State* state) {
    printf("IDLE: Enter command (start/quit): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "start") == 0) {
            *state = RUNNING;
        } else if (strcmp(input, "quit") == 0) {
            *state = STOPPED;
        } else {
            printf("Invalid command\n");
        }
    }
}

void handle_running(enum State* state) {
    printf("RUNNING: Processing... Enter command (pause/stop): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "pause") == 0) {
            *state = PAUSED;
        } else if (strcmp(input, "stop") == 0) {
            *state = STOPPED;
        } else {
            printf("Invalid command\n");
        }
    }
}

void handle_paused(enum State* state) {
    printf("PAUSED: Enter command (resume/stop): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "resume") == 0) {
            *state = RUNNING;
        } else if (strcmp(input, "stop") == 0) {
            *state = STOPPED;
        } else {
            printf("Invalid command\n");
        }
    }
}

void handle_stopped(enum State* state) {
    printf("STOPPED: Exiting state machine\n");
}

void handle_error(enum State* state) {
    printf("ERROR: Invalid state transition\n");
    *state = STOPPED;
}

StateHandler get_handler(enum State state) {
    static StateHandler handlers[] = {
        handle_idle,
        handle_running,
        handle_paused,
        handle_stopped,
        handle_error
    };
    if (state >= IDLE && state <= ERROR) {
        return handlers[state];
    }
    return handle_error;
}

int main(void) {
    enum State current_state = IDLE;
    
    printf("State Machine Demo\n");
    printf("Available commands: start, pause, resume, stop, quit\n");
    
    while (current_state != STOPPED) {
        StateHandler handler = get_handler(current_state);
        if (handler != NULL) {
            handler(&current_state);
        } else {
            current_state = ERROR;
        }
    }
    
    printf("Program terminated\n");
    return 0;
}