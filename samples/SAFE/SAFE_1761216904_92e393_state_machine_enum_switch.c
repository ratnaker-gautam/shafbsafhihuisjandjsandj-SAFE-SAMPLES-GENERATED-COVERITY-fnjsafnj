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
    printf("Current state: IDLE\n");
    printf("Enter selection (1-3) or 0 to exit: ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (input == 0) {
        return 0;
    }
    
    if (input < 1 || input > 3) {
        printf("Invalid selection\n");
        return 1;
    }
    
    machine->selection = input;
    machine->state = SELECTING;
    return 1;
}

int handle_selecting(struct vending_machine *machine) {
    printf("Current state: SELECTING\n");
    printf("Selected item: %d\n", machine->selection);
    printf("Enter payment (50 or 100): ");
    
    int payment;
    if (scanf("%d", &payment) != 1) {
        printf("Invalid input\n");
        machine->state = ERROR;
        return 1;
    }
    
    if (payment != 50 && payment != 100) {
        printf("Invalid payment amount\n");
        machine->state = ERROR;
        return 1;
    }
    
    machine->balance = payment;
    machine->state = PROCESSING;
    return 1;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    
    if (machine->item_count <= 0) {
        printf("Item out of stock\n");
        machine->state = ERROR;
        return 1;
    }
    
    int price = machine->selection * 25;
    
    if (machine->balance < price) {
        printf("Insufficient funds. Need %d, have %d\n", price, machine->balance);
        machine->state = ERROR;
        return 1;
    }
    
    int change = machine->balance - price;
    if (change > 0) {
        printf("Dispensing change: %d\n", change);
    }
    
    machine->item_count--;
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    printf("Dispensing item %d\n", machine->selection);
    printf("Thank you for your purchase!\n");
    
    machine->selection = -1;
    machine->balance = 0;
    machine->state = IDLE;
    return 1;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("Transaction failed. Returning to idle.\n");
    
    machine->selection = -1;
    machine->balance = 0;
    machine->state = IDLE;
    return 1;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    int running = 1;
    while (running) {
        switch (machine.state) {
            case IDLE:
                running = handle_idle(&machine);
                break;
            case SELECTING:
                running = handle_selecting(&machine);
                break;
            case PROCESSING:
                running = handle_processing(&machine);
                break;
            case DISPENSING:
                running = handle_dispensing(&machine);
                break;
            case ERROR:
                running = handle_error(&machine);
                break;
            default:
                printf("Invalid state\n");
                running = 0;
                break;
        }
        
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}