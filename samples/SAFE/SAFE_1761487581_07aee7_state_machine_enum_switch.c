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
    } else if (input == 0) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        if (machine->item_count > 0) {
            machine->balance += 50;
            machine->state = DISPENSING;
            return 1;
        } else {
            machine->state = ERROR;
            return 1;
        }
    } else if (input == 0) {
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        machine->item_count--;
        machine->balance = 0;
        machine->selection = -1;
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_error(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        machine->state = IDLE;
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
            printf("Options: 1-Start selection\n");
            break;
        case SELECTING:
            printf("State: SELECTING\n");
            printf("Items: 1-Soda(50), 2-Chips(50), 3-Candy(50)\n");
            printf("Options: 1-3 Select item, 0-Back\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            printf("Selected: %d\n", machine->selection);
            printf("Options: 1-Insert coin, 0-Back\n");
            break;
        case DISPENSING:
            printf("State: DISPENSING\n");
            printf("Balance: %d\n", machine->balance);
            printf("Options: 1-Take item\n");
            break;
        case ERROR:
            printf("State: ERROR\n");
            printf("Out of stock!\n");
            printf("Options: 1-Continue\n");
            break;
    }
    printf("Items remaining: %d\n\n", machine->item_count);
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int input;
    
    printf("Vending Machine Simulator\n");
    printf("Enter -1 to exit\n\n");
    
    while (1) {
        print_state(&machine);
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == -1) {
            break;
        }
        
        if (!process_input(&machine, input)) {
            printf("Invalid action for current state.\n");
        }
    }
    
    return 0;
}