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
    printf("Enter command (s: select, q: quit): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) != 1) {
        printf("Invalid input\n");
        return 0;
    }
    
    switch (input[0]) {
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
    printf("Available items: 1-5 (Cost: 2 coins each)\n");
    printf("Enter item number (1-5) or b: back: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Invalid input\n");
        return 0;
    }
    
    if (input[0] == 'b') {
        machine->state = IDLE;
        return 0;
    }
    
    if (isdigit(input[0]) && strlen(input) == 1) {
        int item = input[0] - '0';
        if (item >= 1 && item <= 5) {
            machine->selection = item;
            machine->state = PROCESSING;
            printf("Selected item %d\n", item);
        } else {
            printf("Invalid item number\n");
        }
    } else {
        printf("Invalid input\n");
    }
    
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    printf("Current state: PROCESSING\n");
    printf("Item cost: 2 coins. Current balance: %d\n", machine->balance);
    printf("Enter coins (1-9) or c: cancel: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Invalid input\n");
        return 0;
    }
    
    if (input[0] == 'c') {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 0;
    }
    
    if (isdigit(input[0]) && strlen(input) == 1) {
        int coin = input[0] - '0';
        if (coin >= 1 && coin <= 9) {
            if (machine->balance + coin > 100) {
                printf("Balance too high\n");
                return 0;
            }
            machine->balance += coin;
            printf("Added %d coin(s). New balance: %d\n", coin, machine->balance);
            
            if (machine->balance >= 2) {
                machine->state = DISPENSING;
            }
        } else {
            printf("Invalid coin amount\n");
        }
    } else {
        printf("Invalid input\n");
    }
    
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Current state: DISPENSING\n");
    
    if (machine->item_count <= 0) {
        printf("Item out of stock\n");
        machine->state = ERROR;
        return 0;
    }
    
    printf("Dispensing item %d\n", machine->selection);
    machine->item_count--;
    machine->balance -= 2;
    
    if (machine->balance > 0) {
        printf("Returning change: %d coin(s)\n", machine->balance);
        machine->balance = 0;
    }
    
    printf("Thank you for your purchase!\n");
    machine->selection = -1;
    machine->state = IDLE;
    
    return 0;
}

int handle_error(struct vending_machine *machine) {
    printf("Current state: ERROR\n");
    printf("Machine error occurred. Resetting...\n");
    
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    
    return 0;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Commands: s-select, q-quit, b-back, c-cancel, 1-9 coins\n");