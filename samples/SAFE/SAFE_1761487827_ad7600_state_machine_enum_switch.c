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
    printf("Current balance: %d\n", machine->balance);
    printf("Enter command (i=insert coin, s=select item, q=quit): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) != 1) {
        printf("Invalid input\n");
        return 0;
    }
    
    char cmd = input[0];
    
    switch (cmd) {
        case 'i':
            if (machine->balance < 100) {
                machine->balance += 25;
                printf("Coin inserted. Balance: %d\n", machine->balance);
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
    }
    
    return 0;
}

int handle_selecting(struct vending_machine *machine) {
    printf("Available items:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d: Item %d (stock: %d, price: 50)\n", i, i, machine->inventory[i]);
    }
    printf("Select item (0-4) or 'c' to cancel: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 1 && input[0] == 'c') {
        machine->state = IDLE;
        return 0;
    }
    
    char *endptr;
    long selection = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        printf("Invalid selection\n");
        return 0;
    }
    
    if (selection < 0 || selection > 4) {
        printf("Selection out of range\n");
        return 0;
    }
    
    machine->selection = (int)selection;
    machine->state = PROCESSING;
    
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    if (machine->selection < 0 || machine->selection > 4) {
        machine->state = ERROR;
        return 0;
    }
    
    if (machine->inventory[machine->selection] <= 0) {
        printf("Item out of stock\n");
        machine->state = IDLE;
        return 0;
    }
    
    if (machine->balance < 50) {
        printf("Insufficient balance. Need 50, have %d\n", machine->balance);
        machine->state = IDLE;
        return 0;
    }
    
    machine->balance -= 50;
    machine->inventory[machine->selection]--;
    machine->state = DISPENSING;
    
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Dispensing item %d\n", machine->selection);
    printf("Remaining balance: %d\n", machine->balance);
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Error occurred. Resetting machine.\n");
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    
    printf("Vending Machine Simulator\n");
    
    while (running) {
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
        }
        
        if (result == -1) {
            running = 0;
        }
        
        printf("---\n");
    }
    
    printf("Goodbye\n");
    return 0;
}