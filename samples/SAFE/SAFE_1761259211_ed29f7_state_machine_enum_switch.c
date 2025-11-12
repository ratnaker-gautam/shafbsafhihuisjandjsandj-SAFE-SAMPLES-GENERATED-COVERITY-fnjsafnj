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

void init_machine(struct vending_machine *vm) {
    vm->state = IDLE;
    vm->selection = -1;
    vm->balance = 0;
    for (int i = 0; i < 5; i++) {
        vm->inventory[i] = 3;
    }
}

int handle_idle(struct vending_machine *vm) {
    printf("Current balance: %d\n", vm->balance);
    printf("Enter command (s: select, i: insert coin, q: quit): ");
    
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
            vm->state = SELECTING;
            break;
        case 'i':
            if (vm->balance < 100) {
                vm->balance += 10;
                printf("Coin inserted. New balance: %d\n", vm->balance);
            } else {
                printf("Maximum balance reached\n");
            }
            break;
        case 'q':
            return -1;
        default:
            printf("Unknown command\n");
            break;
    }
    
    return 0;
}

int handle_selecting(struct vending_machine *vm) {
    printf("Available items:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d: Item %d (stock: %d)\n", i, i, vm->inventory[i]);
    }
    printf("Select item (0-4) or 'c' to cancel: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 1 && input[0] == 'c') {
        vm->state = IDLE;
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
    
    vm->selection = (int)selection;
    
    if (vm->inventory[vm->selection] <= 0) {
        printf("Item out of stock\n");
        vm->state = IDLE;
        return 0;
    }
    
    vm->state = PROCESSING;
    return 0;
}

int handle_processing(struct vending_machine *vm) {
    int price = 20;
    
    if (vm->balance < price) {
        printf("Insufficient balance. Need %d, have %d\n", price, vm->balance);
        vm->state = IDLE;
        return 0;
    }
    
    vm->balance -= price;
    vm->inventory[vm->selection]--;
    vm->state = DISPENSING;
    return 0;
}

int handle_dispensing(struct vending_machine *vm) {
    printf("Dispensing item %d\n", vm->selection);
    printf("Remaining balance: %d\n", vm->balance);
    vm->selection = -1;
    vm->state = IDLE;
    return 0;
}

int main(void) {
    struct vending_machine vm;
    init_machine(&vm);
    
    printf("Vending Machine Simulator\n");
    
    while (1) {
        int result = 0;
        
        switch (vm.state) {
            case IDLE:
                result = handle_idle(&vm);
                break;
            case SELECTING:
                result = handle_selecting(&vm);
                break;
            case PROCESSING:
                result = handle_processing(&vm);
                break;
            case DISPENSING:
                result = handle_dispensing(&vm);
                break;
            case ERROR:
                printf("Error state reached\n");
                vm.state = IDLE;
                break;
        }
        
        if (result == -1) {
            break;
        }
        
        printf("---\n");
    }
    
    printf("Goodbye\n");
    return 0;
}