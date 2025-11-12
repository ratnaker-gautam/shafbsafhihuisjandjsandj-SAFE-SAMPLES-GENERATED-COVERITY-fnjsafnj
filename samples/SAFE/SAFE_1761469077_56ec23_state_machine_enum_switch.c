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
    if (input >= 0 && input <= 2) {
        machine->selection = input;
        machine->state = PROCESSING;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    int price = 0;
    if (machine->selection == 0) price = 50;
    else if (machine->selection == 1) price = 75;
    else if (machine->selection == 2) price = 100;
    
    if (input == 1) {
        machine->balance += 25;
        if (machine->balance >= price) {
            machine->state = DISPENSING;
        }
        return 1;
    } else if (input == 2) {
        machine->balance = 0;
        machine->selection = -1;
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (machine->item_count > 0) {
        machine->item_count--;
        int price = 0;
        if (machine->selection == 0) price = 50;
        else if (machine->selection == 1) price = 75;
        else if (machine->selection == 2) price = 100;
        
        if (machine->balance > price) {
            printf("Change: %d\n", machine->balance - price);
        }
        machine->balance = 0;
        machine->selection = -1;
        machine->state = IDLE;
        return 1;
    } else {
        machine->state = ERROR;
        return 0;
    }
}

int handle_error(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 3) {
        machine->item_count = machine->max_items;
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
            printf("Options: 0-Soda(50), 1-Chips(75), 2-Candy(100)\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            printf("Balance: %d\n", machine->balance);
            printf("Options: 1-Insert 25, 2-Cancel\n");
            break;
        case DISPENSING:
            printf("State: DISPENSING\n");
            printf("Dispensing item...\n");
            break;
        case ERROR:
            printf("State: ERROR\n");
            printf("Out of stock! Options: 3-Restock\n");
            break;
    }
}

int main(void) {
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
        
        if (input == 9) {
            printf("Exiting...\n");
            break;
        }
        
        if (!process_input(&machine, input)) {
            printf("Invalid choice for current state\n");
        }
        
        printf("\n");
    }
    
    return 0;
}