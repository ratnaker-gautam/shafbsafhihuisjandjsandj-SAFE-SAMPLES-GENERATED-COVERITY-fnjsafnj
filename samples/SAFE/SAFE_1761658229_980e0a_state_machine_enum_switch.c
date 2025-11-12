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
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->item_count = 5;
    machine->max_items = 10;
}

int handle_idle(struct vending_machine *machine) {
    printf("Current balance: %d\n", machine->balance);
    printf("Enter command (i: insert coin, s: select item, q: quit): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) != 1) {
        printf("Invalid input\n");
        return 0;
    }
    
    char cmd = tolower(input[0]);
    
    switch (cmd) {
        case 'i':
            if (machine->balance < 100) {
                machine->balance += 25;
                printf("Coin inserted. New balance: %d\n", machine->balance);
            } else {
                printf("Maximum balance reached\n");
            }
            break;
        case 's':
            machine->state = SELECTING;
            break;
        case 'q':
            return -1;
        default:
            printf("Unknown command\n");
            break;
    }
    
    return 0;
}

int handle_selecting(struct vending_machine *machine) {
    printf("Available items (cost: 50 each):\n");
    printf("0: Soda\n1: Chips\n2: Candy\n");
    printf("Enter item number (0-2) or 'c' to cancel: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 1 && tolower(input[0]) == 'c') {
        machine->state = IDLE;
        return 0;
    }
    
    char *endptr;
    long selection = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        printf("Invalid selection\n");
        return 0;
    }
    
    if (selection < 0 || selection > 2) {
        printf("Invalid item number\n");
        return 0;
    }
    
    machine->selection = (int)selection;
    machine->state = PROCESSING;
    
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    if (machine->balance < 50) {
        printf("Insufficient funds. Need 50, have %d\n", machine->balance);
        machine->state = IDLE;
        return 0;
    }
    
    if (machine->item_count <= 0) {
        printf("Out of stock\n");
        machine->state = IDLE;
        return 0;
    }
    
    machine->balance -= 50;
    machine->item_count--;
    machine->state = DISPENSING;
    
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    const char *items[] = {"Soda", "Chips", "Candy"};
    
    if (machine->selection >= 0 && machine->selection <= 2) {
        printf("Dispensing: %s\n", items[machine->selection]);
        printf("Remaining balance: %d\n", machine->balance);
    }
    
    machine->selection = -1;
    machine->state = IDLE;
    
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Error state reached. Resetting...\n");
    machine->state = IDLE;
    machine->selection = -1;
    return 0;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    
    while (1) {
        int result = 0;
        
        switch (machine.state) {
            case IDLE:
                result = handle_idle(&machine);
                break;
            case SELECTING:
                result = handle_selecting(&machine);
                break;
            case PROCESSING:
                result = handle_processing(&machine);
                break;
            case DISPENSING:
                result = handle_dispensing(&machine);
                break;
            case ERROR:
                result = handle_error(&machine);
                break;
            default:
                machine.state = ERROR;
                break;
        }
        
        if (result == -1) {
            break;
        }
        
        printf("---\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}