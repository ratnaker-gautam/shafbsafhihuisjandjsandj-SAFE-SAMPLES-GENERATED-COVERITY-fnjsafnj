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
    int stock[5];
};

void init_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    for (int i = 0; i < 5; i++) {
        machine->stock[i] = 3;
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
    
    char cmd = tolower(input[0]);
    
    switch (cmd) {
        case 'i':
            if (machine->balance < 100) {
                machine->balance += 25;
                printf("Inserted coin. New balance: %d\n", machine->balance);
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
    printf("Available items:\n");
    printf("0: Soda (25 cents, stock: %d)\n", machine->stock[0]);
    printf("1: Chips (50 cents, stock: %d)\n", machine->stock[1]);
    printf("2: Candy (35 cents, stock: %d)\n", machine->stock[2]);
    printf("3: Water (20 cents, stock: %d)\n", machine->stock[3]);
    printf("4: Gum (15 cents, stock: %d)\n", machine->stock[4]);
    printf("Enter item number (0-4) or 'c' to cancel: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
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
    
    if (selection < 0 || selection > 4) {
        printf("Invalid item number\n");
        return 0;
    }
    
    machine->selection = (int)selection;
    machine->state = PROCESSING;
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    int prices[] = {25, 50, 35, 20, 15};
    int selection = machine->selection;
    
    if (selection < 0 || selection > 4) {
        machine->state = ERROR;
        return 0;
    }
    
    if (machine->stock[selection] <= 0) {
        printf("Item out of stock\n");
        machine->state = IDLE;
        return 0;
    }
    
    if (machine->balance < prices[selection]) {
        printf("Insufficient funds. Need %d, have %d\n", prices[selection], machine->balance);
        machine->state = IDLE;
        return 0;
    }
    
    machine->balance -= prices[selection];
    machine->stock[selection]--;
    machine->state = DISPENSING;
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    char *items[] = {"Soda", "Chips", "Candy", "Water", "Gum"};
    if (machine->selection >= 0 && machine->selection <= 4) {
        printf("Dispensing: %s\n", items[machine->selection]);
    }
    printf("Remaining balance: %d\n", machine->balance);
    machine->selection = -1;
    machine->state = IDLE;
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("System error occurred\n");
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