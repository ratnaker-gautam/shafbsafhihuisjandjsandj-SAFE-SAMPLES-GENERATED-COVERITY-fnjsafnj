//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READY, PROCESSING, COMPLETE, ERROR };

typedef void (*StateHandler)(enum State *current, int *data);

void handle_idle(enum State *current, int *data) {
    printf("IDLE: Enter command (1=ready, 2=exit): ");
    int cmd;
    if (scanf("%d", &cmd) != 1) {
        while (getchar() != '\n');
        *current = ERROR;
        return;
    }
    
    if (cmd == 1) {
        *current = READY;
        *data = 0;
    } else if (cmd == 2) {
        exit(0);
    } else {
        *current = ERROR;
    }
}

void handle_ready(enum State *current, int *data) {
    printf("READY: Enter number to process (1-100): ");
    int num;
    if (scanf("%d", &num) != 1) {
        while (getchar() != '\n');
        *current = ERROR;
        return;
    }
    
    if (num >= 1 && num <= 100) {
        *data = num;
        *current = PROCESSING;
    } else {
        *current = ERROR;
    }
}

void handle_processing(enum State *current, int *data) {
    printf("PROCESSING: Calculating factorial of %d\n", *data);
    
    long long result = 1;
    int n = *data;
    
    for (int i = 1; i <= n; i++) {
        if (result > 1000000000 / i) {
            *current = ERROR;
            return;
        }
        result *= i;
    }
    
    printf("Result: %lld\n", result);
    *current = COMPLETE;
}

void handle_complete(enum State *current, int *data) {
    printf("COMPLETE: Continue? (1=yes, 0=no): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        *current = ERROR;
        return;
    }
    
    if (choice == 1) {
        *current = IDLE;
    } else if (choice == 0) {
        exit(0);
    } else {
        *current = ERROR;
    }
}

void handle_error(enum State *current, int *data) {
    printf("ERROR: Invalid input detected. Resetting to IDLE.\n");
    while (getchar() != '\n');
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
    
    printf("State Machine Demo - Factorial Calculator\n");
    printf("=========================================\n");
    
    while (1) {
        if (current >= 0 && current <= ERROR) {
            handlers[current](&current, &data);
        } else {
            current = ERROR;
        }
    }
    
    return 0;
}