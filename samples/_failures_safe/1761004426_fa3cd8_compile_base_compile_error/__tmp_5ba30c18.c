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
    printf("Enter command (s: select, q: quit): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    char cmd = input[0];
    
    switch (cmd) {
        case 's':
            machine->state = SELECTING;
            break;
        case 'q':
            return 0;
        default:
            printf("Invalid command\n");
            break;
    }
    
    return 1;
}

int handle_selecting(struct vending_machine *machine) {
    printf("Current state: SELECTING\n");
    printf("Items: 0($1) 1($2) 2($3) 3($4) 4($5)\n");
    printf("Enter item number (0-4) or b: back: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (input[0] == 'b') {
        machine->state = IDLE;
        return 1;
    }
    
    if (!isdigit(input[0]) || strlen(input) > 1) {
        printf("Invalid selection\n");
        return 1;
    }
    
    int selection = input[0] - '0';
    
    if (selection < 0 || selection > 4) {
        printf("Invalid selection\n");
        return 1;
    }
    
    if (machine->inventory[selection] <= 0) {
        printf("Item out of stock\n");
        machine->state = ERROR;
        return 1;
    }
    
    machine->selection = selection;
    machine->state = PROCESSING;
    
    return 1;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    int price = machine->selection + 1;
    printf("Item %d costs $%d. Current balance: $%d\n", machine->selection, price, machine->balance);
    printf("Enter coins (1-5) or c: cancel: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (input[0] == 'c') {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    
    if (!isdigit(input[0]) || strlen(input) > 1) {
        printf("Invalid coin\n");
        return 1;
    }
    
    int coin = input[0] - '0';
    
    if (coin < 1 || coin > 5) {
        printf("Invalid coin\n");
        return 1;
    }
    
    if (machine->balance + coin > 100) {
        printf("Balance too high\n");
        machine->state = ERROR;
        return 1;
    }
    
    machine->balance += coin;
    
    if (machine->balance >= price) {
        machine->state = DISPENSING;
    }
    
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    int price = machine->selection + 1;
    
    machine->inventory[machine->selection]--;
    int change = machine->balance - price;
    
    printf("Dispensing item %d\n", machine->selection);
    if (change > 0) {
        printf("Returning change: $%d\n", change);
    }
    
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
    
    return 1;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("Error occurred. Resetting...\n");
    
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
    
    return 1;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    
    while (running) {