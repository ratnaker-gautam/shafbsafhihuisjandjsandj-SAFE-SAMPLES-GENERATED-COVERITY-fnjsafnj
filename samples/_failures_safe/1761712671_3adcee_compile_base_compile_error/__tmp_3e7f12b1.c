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
    const char *items[5];
    int prices[5];
};

void machine_init(struct vending_machine *vm) {
    vm->state = IDLE;
    vm->selection = -1;
    vm->balance = 0;
    for (int i = 0; i < 5; i++) {
        vm->stock[i] = 3;
    }
    vm->items[0] = "Water";
    vm->items[1] = "Soda";
    vm->items[2] = "Chips";
    vm->items[3] = "Candy";
    vm->items[4] = "Gum";
    vm->prices[0] = 100;
    vm->prices[1] = 150;
    vm->prices[2] = 200;
    vm->prices[3] = 125;
    vm->prices[4] = 75;
}

void display_menu(struct vending_machine *vm) {
    printf("Vending Machine Menu:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d. %s - $%d.%02d (%d left)\n", 
               i + 1, vm->items[i], 
               vm->prices[i] / 100, vm->prices[i] % 100,
               vm->stock[i]);
    }
    printf("Current balance: $%d.%02d\n", vm->balance / 100, vm->balance % 100);
}

int handle_idle(struct vending_machine *vm) {
    printf("Enter command (m: menu, s: select, i: insert coin, q: quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) != 1) {
        printf("Invalid input\n");
        return 0;
    }
    
    switch (input[0]) {
        case 'm':
            display_menu(vm);
            break;
        case 's':
            vm->state = SELECTING;
            break;
        case 'i':
            vm->state = PROCESSING;
            break;
        case 'q':
            return -1;
        default:
            printf("Invalid command\n");
    }
    return 0;
}

int handle_selecting(struct vending_machine *vm) {
    printf("Enter item number (1-5) or c to cancel: ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        vm->state = ERROR;
        return 0;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 1 && input[0] == 'c') {
        vm->state = IDLE;
        return 0;
    }
    
    if (strlen(input) != 1 || !isdigit(input[0])) {
        printf("Invalid selection\n");
        return 0;
    }
    
    int selection = input[0] - '0';
    if (selection < 1 || selection > 5) {
        printf("Invalid selection\n");
        return 0;
    }
    
    vm->selection = selection - 1;
    
    if (vm->stock[vm->selection] <= 0) {
        printf("Item out of stock\n");
        vm->state = IDLE;
        return 0;
    }
    
    if (vm->balance >= vm->prices[vm->selection]) {
        vm->state = DISPENSING;
    } else {
        printf("Insufficient funds. Need $%d.%02d more\n", 
               (vm->prices[vm->selection] - vm->balance) / 100,
               (vm->prices[vm->selection] - vm->balance) % 100);
        vm->state = IDLE;
    }
    
    return 0;
}

int handle_processing(struct vending_machine *vm) {
    printf("Enter coin amount (25, 50, 100, 200) or c to cancel: ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        vm->state = ERROR;
        return 0;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 1 && input[0] == 'c') {
        vm->state = IDLE;
        return 0;
    }
    
    int coin;
    if (sscanf(input, "%d", &coin) != 1) {
        printf("Invalid coin amount\n");
        return 0;
    }
    
    int valid_coins[] = {25, 50, 100, 200};
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (coin == valid_coins[i]) {
            valid = 1;
            break;
        }
    }
    
    if (!valid) {
        printf("Invalid coin amount\n");