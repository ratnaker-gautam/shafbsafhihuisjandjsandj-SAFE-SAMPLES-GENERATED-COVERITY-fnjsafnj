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
    printf("Enter command (s: select item, q: quit): ");
    
    char input[10];
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
    printf("Current state: SELECTING\n");
    printf("Items: 0($1), 1($2), 2($3), 3($4), 4($5)\n");
    printf("Enter item number (0-4) or 'b' to go back: ");
    
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 1 && input[0] == 'b') {
        machine->state = IDLE;
        return 0;
    }
    
    if (strlen(input) != 1 || !isdigit(input[0])) {
        printf("Invalid input\n");
        return 0;
    }
    
    int item = input[0] - '0';
    
    if (item < 0 || item > 4) {
        printf("Invalid item\n");
        return 0;
    }
    
    if (machine->inventory[item] <= 0) {
        printf("Item out of stock\n");
        machine->state = IDLE;
        return 0;
    }
    
    machine->selection = item;
    machine->state = PROCESSING;
    
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    int price = machine->selection + 1;
    printf("Selected item %d, price: $%d\n", machine->selection, price);
    printf("Current balance: $%d\n", machine->balance);
    printf("Enter money amount (1-5) or 'c' to cancel: ");
    
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 1 && input[0] == 'c') {
        machine->state = IDLE;
        machine->selection = -1;
        return 0;
    }
    
    if (strlen(input) != 1 || !isdigit(input[0])) {
        printf("Invalid input\n");
        return 0;
    }
    
    int amount = input[0] - '0';
    
    if (amount < 1 || amount > 5) {
        printf("Invalid amount\n");
        return 0;
    }
    
    if (machine->balance + amount > 100) {
        printf("Balance limit exceeded\n");
        return 0;
    }
    
    machine->balance += amount;
    
    if (machine->balance >= price) {
        machine->state = DISPENSING;
    }
    
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    int price = machine->selection + 1;
    
    machine->inventory[machine->selection]--;
    machine->balance -= price;
    
    printf("Dispensed item %d\n", machine->selection);
    
    if (machine->balance > 0) {
        printf("Returning change: $%d\n", machine->balance);
        machine->balance = 0;
    }
    
    machine->selection = -1;
    machine->state = IDLE;
    
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("System error occurred\n");
    machine->state = IDLE;
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
                result = handle_selecting(&machine