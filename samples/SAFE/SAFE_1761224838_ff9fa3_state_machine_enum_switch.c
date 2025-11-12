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
    int inventory[5];
};

void init_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    for (int i = 0; i < 5; i++) {
        machine->inventory[i] = 3;
    }
}

int handle_idle(struct vending_machine *machine) {
    printf("Current state: IDLE\n");
    printf("Enter selection (0-4) or -1 to exit: ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        return -1;
    }
    
    if (input == -1) {
        return 0;
    }
    
    if (input < 0 || input > 4) {
        printf("Invalid selection\n");
        return 1;
    }
    
    if (machine->inventory[input] <= 0) {
        printf("Item out of stock\n");
        return 1;
    }
    
    machine->selection = input;
    machine->state = SELECTING;
    return 1;
}

int handle_selecting(struct vending_machine *machine) {
    printf("Current state: SELECTING\n");
    printf("Selected item %d. Enter payment (1-100): ", machine->selection);
    
    int payment;
    if (scanf("%d", &payment) != 1) {
        machine->state = ERROR;
        return 1;
    }
    
    if (payment < 1 || payment > 100) {
        printf("Invalid payment amount\n");
        return 1;
    }
    
    machine->balance = payment;
    machine->state = PROCESSING;
    return 1;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    
    int price = (machine->selection + 1) * 2;
    
    if (machine->balance < price) {
        printf("Insufficient funds. Need %d, got %d\n", price, machine->balance);
        machine->state = ERROR;
        return 1;
    }
    
    int change = machine->balance - price;
    if (change > 0) {
        printf("Dispensing %d in change\n", change);
    }
    
    machine->inventory[machine->selection]--;
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    printf("Dispensing item %d\n", machine->selection);
    printf("Transaction complete\n\n");
    
    machine->selection = -1;
    machine->balance = 0;
    machine->state = IDLE;
    return 1;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("Transaction failed. Returning to idle state\n\n");
    
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
        int result = 1;
        
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
        
        if (result == 0) {
            running = 0;
        } else if (result == -1) {
            printf("Input error\n");
            running = 0;
        }
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    printf("Goodbye\n");
    return 0;
}