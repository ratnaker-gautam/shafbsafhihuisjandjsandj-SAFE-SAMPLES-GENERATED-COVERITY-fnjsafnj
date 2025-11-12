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
            printf("Invalid command\n");
            break;
    }
    
    return 0;
}

int handle_selecting(struct vending_machine *machine) {
    printf("Current state: SELECTING\n");
    printf("Available items (0-4): ");
    for (int i = 0; i < 5; i++) {
        if (machine->inventory[i] > 0) {
            printf("%d ", i);
        }
    }
    printf("\nEnter item number (0-4) or 'c' to cancel: ");
    
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 1 && input[0] == 'c') {
        machine->state = IDLE;
        return 0;
    }
    
    if (strlen(input) != 1 || !isdigit(input[0])) {
        printf("Invalid input\n");
        return 0;
    }
    
    int selection = input[0] - '0';
    
    if (selection < 0 || selection > 4) {
        printf("Invalid selection\n");
        return 0;
    }
    
    if (machine->inventory[selection] <= 0) {
        printf("Item out of stock\n");
        return 0;
    }
    
    machine->selection = selection;
    machine->state = PROCESSING;
    
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    printf("Selected item: %d\n", machine->selection);
    printf("Price: 2 coins\n");
    printf("Current balance: %d\n", machine->balance);
    printf("Enter command (i: insert coin, d: dispense, c: cancel): ");
    
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
        case 'i':
            if (machine->balance < 10) {
                machine->balance++;
                printf("Coin inserted. Balance: %d\n", machine->balance);
            } else {
                printf("Maximum balance reached\n");
            }
            break;
        case 'd':
            if (machine->balance >= 2) {
                machine->state = DISPENSING;
            } else {
                printf("Insufficient balance\n");
            }
            break;
        case 'c':
            machine->state = IDLE;
            machine->selection = -1;
            machine->balance = 0;
            break;
        default:
            printf("Invalid command\n");
            break;
    }
    
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    
    if (machine->selection < 0 || machine->selection > 4) {
        machine->state = ERROR;
        return 0;
    }
    
    if (machine->inventory[machine->selection] <= 0) {
        machine->state = ERROR;
        return 0;
    }
    
    machine->inventory[machine->selection]--;
    machine->balance -= 2;
    
    printf("Dispensing item %d\n", machine->selection);
    printf("Change returned: %d\n", machine->balance);
    
    machine->selection = -1;
    machine->balance = 0;
    machine->state = IDLE;
    
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("An error occurred. Resetting machine.\n");
    
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    
    return 0;
}

int main(void) {
    struct v