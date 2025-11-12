//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    int item_count;
    int max_items;
};

void init_machine(struct vending_machine *machine) {
    if (machine == NULL) return;
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->item_count = 10;
    machine->max_items = 20;
}

int handle_idle(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input >= 0 && input <= 9) {
        machine->selection = input;
        machine->state = PROCESSING;
        return 1;
    } else if (input == 99) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        if (machine->item_count > 0) {
            machine->item_count--;
            machine->state = DISPENSING;
        } else {
            machine->state = ERROR;
        }
        return 1;
    } else if (input == 99) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

int handle_error(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

int process_input(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    switch (machine->state) {
        case IDLE:
            return handle_idle(machine, input);
        case SELECTING:
            return handle_selecting(machine, input);
        case PROCESSING:
            return handle_processing(machine, input);
        case DISPENSING:
            return handle_dispensing(machine, input);
        case ERROR:
            return handle_error(machine, input);
        default:
            return 0;
    }
}

void print_state(struct vending_machine *machine) {
    if (machine == NULL) return;
    
    switch (machine->state) {
        case IDLE:
            printf("State: IDLE - Press 1 to select item\n");
            break;
        case SELECTING:
            printf("State: SELECTING - Choose item (0-9) or 99 to cancel\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING - Item %d selected. Press 1 to confirm, 99 to cancel\n", machine->selection);
            break;
        case DISPENSING:
            printf("State: DISPENSING - Item dispensed. Press 1 to continue\n");
            break;
        case ERROR:
            printf("State: ERROR - Out of stock. Press 1 to continue\n");
            break;
        default:
            printf("State: UNKNOWN\n");
            break;
    }
    printf("Items remaining: %d\n\n", machine->item_count);
}

int main(void) {
    struct vending_machine machine;
    int input;
    char buffer[32];
    
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Enter numbers to interact with the machine\n");
    printf("Enter 0 to exit\n\n");
    
    while (1) {
        print_state(&machine);
        
        printf("Enter input: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        char *endptr;
        input = strtol(buffer, &endptr, 10);
        if (endptr == buffer) {
            printf("Invalid input. Please enter a number.\n\n");
            continue;
        }
        
        if (input == 0) {
            break;
        }
        
        if (!process_input(&machine, input)) {
            printf("Invalid input for current state.\n\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}