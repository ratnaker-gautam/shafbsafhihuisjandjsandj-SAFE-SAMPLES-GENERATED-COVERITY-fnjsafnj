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
    printf("Enter command (i: insert coin, s: select item, q: quit): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    char cmd = input[0];
    
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
            return 0;
        default:
            printf("Invalid command\n");
    }
    
    return 1;
}

int handle_selecting(struct vending_machine *machine) {
    printf("Available items:\n");
    printf("0: Soda (25)\n");
    printf("1: Chips (50)\n");
    printf("2: Candy (35)\n");
    printf("3: Water (20)\n");
    printf("4: Gum (15)\n");
    printf("Enter item number (0-4) or 'c' to cancel: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 1 && input[0] == 'c') {
        machine->state = IDLE;
        return 1;
    }
    
    char *endptr;
    long selection = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        printf("Invalid selection\n");
        return 1;
    }
    
    if (selection < 0 || selection > 4) {
        printf("Invalid item number\n");
        return 1;
    }
    
    machine->selection = (int)selection;
    machine->state = PROCESSING;
    return 1;
}

int handle_processing(struct vending_machine *machine) {
    int prices[] = {25, 50, 35, 20, 15};
    int selection = machine->selection;
    
    if (selection < 0 || selection > 4) {
        machine->state = ERROR;
        return 1;
    }
    
    if (machine->inventory[selection] <= 0) {
        printf("Item out of stock\n");
        machine->state = IDLE;
        return 1;
    }
    
    if (machine->balance < prices[selection]) {
        printf("Insufficient balance. Need %d, have %d\n", prices[selection], machine->balance);
        machine->state = IDLE;
        return 1;
    }
    
    machine->balance -= prices[selection];
    machine->inventory[selection]--;
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    char *items[] = {"Soda", "Chips", "Candy", "Water", "Gum"};
    int selection = machine->selection;
    
    if (selection >= 0 && selection <= 4) {
        printf("Dispensing %s\n", items[selection]);
        printf("Remaining balance: %d\n", machine->balance);
    }
    
    machine->selection = -1;
    machine->state = IDLE;
    return 1;
}

int handle_error(struct vending_machine *machine) {
    printf("System error occurred\n");
    machine->state = IDLE;
    machine->selection = -1;
    return 1;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    
    printf("Vending Machine Simulator\n");
    
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
        }
    }
    
    printf("Goodbye\n");
    return 0;
}