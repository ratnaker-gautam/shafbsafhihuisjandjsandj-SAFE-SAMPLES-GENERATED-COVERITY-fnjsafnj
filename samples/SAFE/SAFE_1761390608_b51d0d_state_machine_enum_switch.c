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
    machine->item_count = 5;
    machine->max_items = 10;
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
    if (input >= 1 && input <= 3) {
        machine->selection = input;
        machine->state = PROCESSING;
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
            return 1;
        } else {
            machine->state = ERROR;
            return 0;
        }
    } else if (input == 2) {
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
            machine->state = ERROR;
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
            printf("State: SELECTING - Choose item (1-3)\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING - Item %d selected. Press 1 to dispense, 2 to cancel\n", machine->selection);
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
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int input;
    int result;
    
    printf("Vending Machine Simulator\n");
    
    while (1) {
        print_state(&machine);
        printf("Enter input (0 to exit): ");
        
        result = scanf("%d", &input);
        if (result != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == 0) {
            break;
        }
        
        if (!process_input(&machine, input)) {
            printf("Invalid input for current state.\n");
        }
        
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}