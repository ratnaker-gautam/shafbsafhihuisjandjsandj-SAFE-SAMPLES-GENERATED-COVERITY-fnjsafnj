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
    printf("Available items (0-4): ");
    for (int i = 0; i < 5; i++) {
        printf("%d:%s ", i, machine->inventory[i] > 0 ? "available" : "sold out");
    }
    printf("\nSelect item (0-4) or b: back: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        printf("Invalid selection\n");
        return 0;
    }
    
    if (input[0] == 'b') {
        machine->state = IDLE;
        return 0;
    }
    
    if (!isdigit((unsigned char)input[0])) {
        printf("Invalid selection\n");
        return 0;
    }
    
    int selection = input[0] - '0';
    if (selection < 0 || selection > 4) {
        printf("Invalid selection\n");
        return 0;
    }
    
    if (machine->inventory[selection] <= 0) {
        printf("Item sold out\n");
        return 0;
    }
    
    machine->selection = selection;
    machine->state = PROCESSING;
    
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    printf("Selected item: %d\n", machine->selection);
    printf("Enter payment (1-5 coins) or c: cancel: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        printf("Invalid payment\n");
        return 0;
    }
    
    if (input[0] == 'c') {
        machine->state = IDLE;
        machine->selection = -1;
        return 0;
    }
    
    if (!isdigit((unsigned char)input[0])) {
        printf("Invalid payment\n");
        return 0;
    }
    
    int payment = input[0] - '0';
    if (payment < 1 || payment > 5) {
        printf("Invalid payment amount\n");
        return 0;
    }
    
    machine->balance = payment;
    
    if (payment >= 2) {
        machine->state = DISPENSING;
    } else {
        printf("Insufficient payment\n");
        machine->state = ERROR;
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
    int change = machine->balance - 2;
    
    printf("Dispensing item %d\n", machine->selection);
    if (change > 0) {
        printf("Returning %d coins\n", change);
    }
    
    machine->selection = -1;
    machine->balance = 0;
    machine->state = IDLE;
    
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("Transaction error occurred\n");
    printf("Press r to reset: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")