//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

enum vending_state {
    IDLE,
    SELECTING,
    PROCESSING,
    DISPENSING,
    ERROR
};

struct vending_machine {
    enum vending_state state;
    int selection;
    int balance;
    int stock[5];
};

void machine_init(struct vending_machine *vm) {
    vm->state = IDLE;
    vm->selection = -1;
    vm->balance = 0;
    for (int i = 0; i < 5; i++) {
        vm->stock[i] = 3;
    }
}

int handle_idle(struct vending_machine *vm, int input) {
    if (input == 1) {
        vm->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *vm, int input) {
    if (input >= 0 && input <= 4) {
        if (vm->stock[input] > 0) {
            vm->selection = input;
            vm->state = PROCESSING;
            return 1;
        }
    }
    vm->state = ERROR;
    return 0;
}

int handle_processing(struct vending_machine *vm, int input) {
    if (input == 5) {
        if (vm->balance < INT_MAX) {
            vm->balance += 1;
        }
        if (vm->balance >= 2) {
            vm->state = DISPENSING;
            return 1;
        }
    } else if (input == 6) {
        vm->state = IDLE;
        vm->selection = -1;
        vm->balance = 0;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *vm, int input) {
    if (input == 7) {
        if (vm->selection >= 0 && vm->selection <= 4) {
            if (vm->stock[vm->selection] > 0) {
                vm->stock[vm->selection] -= 1;
            }
        }
        vm->selection = -1;
        vm->balance = 0;
        vm->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_error(struct vending_machine *vm, int input) {
    if (input == 8) {
        vm->state = IDLE;
        vm->selection = -1;
        vm->balance = 0;
        return 1;
    }
    return 0;
}

int process_input(struct vending_machine *vm, int input) {
    switch (vm->state) {
        case IDLE:
            return handle_idle(vm, input);
        case SELECTING:
            return handle_selecting(vm, input);
        case PROCESSING:
            return handle_processing(vm, input);
        case DISPENSING:
            return handle_dispensing(vm, input);
        case ERROR:
            return handle_error(vm, input);
        default:
            vm->state = ERROR;
            return 0;
    }
}

void print_state(struct vending_machine *vm) {
    const char *state_names[] = {
        "IDLE", "SELECTING", "PROCESSING", "DISPENSING", "ERROR"
    };
    printf("State: %s | Balance: %d | Selection: %d\n", 
           state_names[vm->state], vm->balance, vm->selection);
    printf("Stock: [%d][%d][%d][%d][%d]\n", 
           vm->stock[0], vm->stock[1], vm->stock[2], vm->stock[3], vm->stock[4]);
}

int main(void) {
    struct vending_machine vm;
    machine_init(&vm);
    
    printf("Vending Machine Simulator\n");
    printf("Commands: 1=Start 0-4=Select 5=Insert 6=Cancel 7=Collect 8=Reset 9=Exit\n");
    
    char input_buffer[32];
    int input;
    
    while (1) {
        print_state(&vm);
        printf("Enter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
            continue;
        }
        
        char *endptr;
        long val = strtol(input_buffer, &endptr, 10);
        if (endptr == input_buffer) {
            printf("Invalid input\n");
            continue;
        }
        
        if (*endptr != '\n' && *endptr != '\0') {
            printf("Invalid input\n");
            continue;
        }
        
        if (val < INT_MIN || val > INT_MAX) {
            printf("Input out of range\n");
            continue;
        }
        
        input = (int)val;
        
        if (input == 9) {
            break;
        }
        
        if (!process_input(&vm, input)) {
            printf("Invalid command for current state\n");
        }
    }
    
    return 0;
}