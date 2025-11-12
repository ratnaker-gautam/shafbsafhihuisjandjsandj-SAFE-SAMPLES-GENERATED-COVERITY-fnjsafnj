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
        if (machine->balance >= 2) {
            machine->balance -= 2;
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
        if (machine->item_count > 0) {
            machine->item_count--;
            machine->state = IDLE;
            machine->selection = -1;
            return 1;
        } else {
            machine->state = ERROR;
            return 0;
        }
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
    
    printf("Current state: ");
    switch (machine->state) {
        case IDLE:
            printf("IDLE");
            break;
        case SELECTING:
            printf("SELECTING");
            break;
        case PROCESSING:
            printf("PROCESSING");
            break;
        case DISPENSING:
            printf("DISPENSING");
            break;
        case ERROR:
            printf("ERROR");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
    printf("\nBalance: %d\n", machine->balance);
    printf("Items remaining: %d\n", machine->item_count);
    if (machine->selection != -1) {
        printf("Selected item: %d\n", machine->selection);
    }
}

int main() {
    struct vending_machine machine;
    init_machine(&machine);
    int input;
    
    printf("Vending Machine Simulator\n");
    printf("Commands: 1=Start, 2=Add coin, 3=Select item, 4=Confirm, 5=Cancel, 6=Reset, 0=Exit\n");
    
    while (1) {
        print_state(&machine);
        printf("Enter command: ");
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == 0) {
            break;
        }
        
        if (input == 2) {
            if (machine.balance < 100) {
                machine.balance++;
            }
            continue;
        }
        
        if (!process_input(&machine, input)) {
            printf("Invalid transition from current state.\n");
        }
    }
    
    return 0;
}