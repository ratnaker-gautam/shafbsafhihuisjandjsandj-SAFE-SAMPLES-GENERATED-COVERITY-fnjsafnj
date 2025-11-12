//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    } else if (input == 2) {
        machine->balance += 1;
        printf("Balance: %d\n", machine->balance);
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
    } else if (input == 9) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    if (machine == NULL) return 0;
    
    if (machine->selection < 0 || machine->selection > 2) {
        machine->state = ERROR;
        return 0;
    }
    
    int prices[] = {2, 3, 1};
    int selected_price = prices[machine->selection];
    
    if (machine->balance >= selected_price && machine->item_count > 0) {
        machine->balance -= selected_price;
        machine->item_count--;
        machine->state = DISPENSING;
        return 1;
    } else {
        machine->state = ERROR;
        return 0;
    }
}

int handle_dispensing(struct vending_machine *machine) {
    if (machine == NULL) return 0;
    
    char *items[] = {"Soda", "Chips", "Candy"};
    printf("Dispensing: %s\n", items[machine->selection]);
    printf("Remaining balance: %d\n", machine->balance);
    machine->selection = -1;
    machine->state = IDLE;
    return 1;
}

int handle_error(struct vending_machine *machine) {
    if (machine == NULL) return 0;
    
    if (machine->item_count <= 0) {
        printf("Error: Out of stock\n");
    } else if (machine->balance < 0) {
        printf("Error: Insufficient balance\n");
    } else {
        printf("Error: Invalid selection\n");
    }
    
    machine->selection = -1;
    machine->state = IDLE;
    return 1;
}

int get_valid_input(void) {
    char buffer[16];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -2;
    }
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -2;
    }
    
    if (value < -1 || value > 9) {
        return -2;
    }
    
    return (int)value;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Commands: 1=Start, 2=Add coin, 0-2=Select item, 9=Cancel, -1=Exit\n");
    
    while (1) {
        switch (machine.state) {
            case IDLE:
                printf("State: IDLE - ");
                break;
            case SELECTING:
                printf("State: SELECTING - ");
                break;
            case PROCESSING:
                printf("State: PROCESSING - ");
                break;
            case DISPENSING:
                printf("State: DISPENSING - ");
                break;
            case ERROR:
                printf("State: ERROR - ");
                break;
        }
        
        printf("Enter command: ");
        fflush(stdout);
        
        int input = get_valid_input();
        if (input == -2) {
            printf("Invalid input. Please enter a number between -1 and 9.\n");
            continue;
        }
        
        if (input == -1) {
            printf("Exiting...\n");
            break;
        }
        
        int handled = 0;
        switch (machine.state) {
            case IDLE:
                handled = handle_idle(&machine, input);
                break;
            case SELECTING:
                handled = handle_selecting(&machine, input);
                break;
            case PROCESSING:
                handled = handle_processing(&machine);
                break;
            case DISPENSING:
                handled = handle_dispensing(&machine);
                break;
            case ERROR:
                handled = handle_error(&machine);
                break;
        }
        
        if (!handled) {
            printf("Invalid command for current state.\n");
        }
        
        if (machine.item_count <= 0) {
            printf("Machine empty. Shutting down.\