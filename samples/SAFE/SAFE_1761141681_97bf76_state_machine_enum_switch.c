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
    } else if (input == 2) {
        machine->balance += 5;
        printf("Balance: %d\n", machine->balance);
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
    } else if (input == 9) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input == 1) {
        if (machine->balance >= 3) {
            if (machine->item_count > 0) {
                machine->balance -= 3;
                machine->item_count--;
                machine->state = DISPENSING;
            } else {
                machine->state = ERROR;
            }
        } else {
            machine->state = ERROR;
        }
        return 1;
    } else if (input == 2) {
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input == 1) {
        printf("Item dispensed: %d\n", machine->selection);
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

int handle_error(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input == 1) {
        if (machine->item_count <= 0) {
            printf("Out of stock\n");
        } else {
            printf("Insufficient funds\n");
        }
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
            printf("State: IDLE\n");
            printf("Options: 1-Select, 2-Add money\n");
            break;
        case SELECTING:
            printf("State: SELECTING\n");
            printf("Options: 0-9 Select item, 9-Cancel\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            printf("Selection: %d, Balance: %d\n", machine->selection, machine->balance);
            printf("Options: 1-Confirm, 2-Back\n");
            break;
        case DISPENSING:
            printf("State: DISPENSING\n");
            printf("Options: 1-Take item\n");
            break;
        case ERROR:
            printf("State: ERROR\n");
            printf("Options: 1-Return to idle\n");
            break;
    }
}

int main() {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    
    while (1) {
        print_state(&machine);
        printf("Enter choice: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (input == 99) {
            printf("Exiting\n");
            break;
        }
        
        if (!process_input(&machine, input)) {
            printf("Invalid choice for current state\n");
        }
        
        printf("\n");
    }
    
    return 0;
}