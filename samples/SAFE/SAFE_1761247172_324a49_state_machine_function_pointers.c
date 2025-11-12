//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READY, PROCESSING, COMPLETE, ERROR };

typedef void (*StateHandler)(enum State *current, int *data);

void handle_idle(enum State *current, int *data) {
    printf("IDLE: Enter command (1=load, 2=quit): ");
    int cmd;
    if (scanf("%d", &cmd) != 1) {
        *current = ERROR;
        return;
    }
    if (cmd == 1) {
        printf("Enter data value: ");
        if (scanf("%d", data) != 1) {
            *current = ERROR;
            return;
        }
        *current = READY;
    } else if (cmd == 2) {
        *current = COMPLETE;
    } else {
        *current = ERROR;
    }
}

void handle_ready(enum State *current, int *data) {
    printf("READY: Data=%d. Process? (1=yes, 0=no): ", *data);
    int cmd;
    if (scanf("%d", &cmd) != 1) {
        *current = ERROR;
        return;
    }
    if (cmd == 1) {
        *current = PROCESSING;
    } else if (cmd == 0) {
        *current = IDLE;
    } else {
        *current = ERROR;
    }
}

void handle_processing(enum State *current, int *data) {
    if (*data < 0) {
        *current = ERROR;
        return;
    }
    int result = *data * 2;
    if (result < *data) {
        *current = ERROR;
        return;
    }
    printf("PROCESSING: Result=%d\n", result);
    *data = result;
    *current = COMPLETE;
}

void handle_complete(enum State *current, int *data) {
    printf("COMPLETE: Final data=%d. Continue? (1=yes, 0=no): ", *data);
    int cmd;
    if (scanf("%d", &cmd) != 1) {
        *current = ERROR;
        return;
    }
    if (cmd == 1) {
        *data = 0;
        *current = IDLE;
    } else if (cmd == 0) {
        *current = COMPLETE;
    } else {
        *current = ERROR;
    }
}

void handle_error(enum State *current, int *data) {
    printf("ERROR: Resetting.\n");
    *data = 0;
    *current = IDLE;
}

int main() {
    enum State current = IDLE;
    int data = 0;
    
    StateHandler handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    while (current != COMPLETE) {
        if (current < 0 || current > ERROR) {
            current = ERROR;
        }
        handlers[current](&current, &data);
    }
    
    printf("Program terminated.\n");
    return 0;
}