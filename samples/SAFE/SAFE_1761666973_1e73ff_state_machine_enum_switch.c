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

void initialize_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->item_count = 5;
    machine->max_items = 10;
}

int handle_idle(struct vending_machine *machine) {
    printf("Current state: IDLE\n");
    printf("Options: 1-Insert coin, 2-Select item, 3-Exit\n");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    
    switch (choice) {
        case 1:
            machine->balance += 25;
            printf("Inserted 25 cents. Balance: %d\n", machine->balance);
            break;
        case 2:
            machine->state = SELECTING;
            break;
        case 3:
            return 1;
        default:
            printf("Invalid choice\n");
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine) {
    printf("Current state: SELECTING\n");
    printf("Items available: 1-Soda (50), 2-Chips (75), 3-Candy (25)\n");
    printf("Your balance: %d\n", machine->balance);
    printf("Enter item number (1-3) or 0 to cancel: ");
    
    if (scanf("%d", &machine->selection) != 1) {
        while (getchar() != '\n');
        machine->selection = -1;
        return 0;
    }
    
    if (machine->selection == 0) {
        machine->state = IDLE;
        machine->selection = -1;
    } else if (machine->selection >= 1 && machine->selection <= 3) {
        machine->state = PROCESSING;
    } else {
        printf("Invalid selection\n");
        machine->selection = -1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    
    int price;
    switch (machine->selection) {
        case 1: price = 50; break;
        case 2: price = 75; break;
        case 3: price = 25; break;
        default:
            machine->state = ERROR;
            return 0;
    }
    
    if (machine->balance < price) {
        printf("Insufficient funds. Need %d, have %d\n", price, machine->balance);
        machine->state = SELECTING;
    } else if (machine->item_count <= 0) {
        printf("Out of stock\n");
        machine->state = ERROR;
    } else {
        machine->balance -= price;
        machine->item_count--;
        machine->state = DISPENSING;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    
    const char *item_name;
    switch (machine->selection) {
        case 1: item_name = "Soda"; break;
        case 2: item_name = "Chips"; break;
        case 3: item_name = "Candy"; break;
        default: item_name = "Unknown"; break;
    }
    
    printf("Dispensing: %s\n", item_name);
    printf("Remaining balance: %d\n", machine->balance);
    printf("Items remaining: %d\n", machine->item_count);
    
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("An error occurred. Resetting...\n");
    
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int main(void) {
    struct vending_machine machine;
    initialize_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("=========================\n");
    
    int exit_requested = 0;
    
    while (!exit_requested) {
        switch (machine.state) {
            case IDLE:
                exit_requested = handle_idle(&machine);
                break;
            case SELECTING:
                exit_requested = handle_selecting(&machine);
                break;
            case PROCESSING:
                exit_requested = handle_processing(&machine);
                break;
            case DISPENSING:
                exit_requested = handle_dispensing(&machine);
                break;
            case ERROR:
                exit_requested = handle_error(&machine);
                break;
            default:
                machine.state = ERROR;
                break;
        }
        
        printf("\n");
    }
    
    printf("Thank you for using the vending machine!\n");
    return 0;
}