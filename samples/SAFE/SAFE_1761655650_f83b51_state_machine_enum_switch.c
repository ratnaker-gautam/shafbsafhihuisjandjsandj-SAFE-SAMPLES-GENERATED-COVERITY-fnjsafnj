//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum vending_state {
    IDLE,
    SELECTING,
    PAYING,
    DISPENSING,
    ERROR
};

enum product {
    SODA,
    CHIPS,
    CANDY,
    PRODUCT_COUNT
};

struct vending_machine {
    enum vending_state state;
    enum product selected;
    int balance;
    int inventory[PRODUCT_COUNT];
    int prices[PRODUCT_COUNT];
};

void init_machine(struct vending_machine *vm) {
    vm->state = IDLE;
    vm->selected = SODA;
    vm->balance = 0;
    vm->inventory[SODA] = 5;
    vm->inventory[CHIPS] = 3;
    vm->inventory[CANDY] = 7;
    vm->prices[SODA] = 150;
    vm->prices[CHIPS] = 100;
    vm->prices[CANDY] = 75;
}

void display_state(struct vending_machine *vm) {
    printf("Current state: ");
    switch(vm->state) {
        case IDLE: printf("IDLE\n"); break;
        case SELECTING: printf("SELECTING\n"); break;
        case PAYING: printf("PAYING\n"); break;
        case DISPENSING: printf("DISPENSING\n"); break;
        case ERROR: printf("ERROR\n"); break;
    }
    printf("Balance: %d cents\n", vm->balance);
    printf("Inventory - Soda: %d, Chips: %d, Candy: %d\n",
           vm->inventory[SODA], vm->inventory[CHIPS], vm->inventory[CANDY]);
}

int handle_idle(struct vending_machine *vm, int input) {
    if (input == 1) {
        vm->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *vm, int input) {
    if (input >= 0 && input < PRODUCT_COUNT) {
        if (vm->inventory[input] > 0) {
            vm->selected = (enum product)input;
            vm->state = PAYING;
            return 1;
        }
    }
    vm->state = ERROR;
    return 0;
}

int handle_paying(struct vending_machine *vm, int input) {
    if (input > 0 && input <= 1000) {
        if (vm->balance + input >= 0 && vm->balance + input <= 10000) {
            vm->balance += input;
            if (vm->balance >= vm->prices[vm->selected]) {
                vm->state = DISPENSING;
                return 1;
            }
            return 1;
        }
    }
    vm->state = ERROR;
    return 0;
}

int handle_dispensing(struct vending_machine *vm) {
    if (vm->inventory[vm->selected] > 0) {
        vm->inventory[vm->selected]--;
        vm->balance -= vm->prices[vm->selected];
        if (vm->balance < 0) vm->balance = 0;
        printf("Dispensed product: ");
        switch(vm->selected) {
            case SODA: printf("Soda\n"); break;
            case CHIPS: printf("Chips\n"); break;
            case CANDY: printf("Candy\n"); break;
            default: break;
        }
        vm->state = IDLE;
        return 1;
    }
    vm->state = ERROR;
    return 0;
}

int handle_error(struct vending_machine *vm) {
    printf("Error occurred. Resetting machine.\n");
    vm->balance = 0;
    vm->state = IDLE;
    return 1;
}

int main(void) {
    struct vending_machine vm;
    init_machine(&vm);
    
    printf("Vending Machine Simulator\n");
    printf("Commands: 1=Start, 0-2=Select, 3+=Insert coins, -1=Exit\n");
    
    int running = 1;
    while (running) {
        display_state(&vm);
        printf("Enter command: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            vm.state = ERROR;
        }
        
        if (input == -1) {
            running = 0;
            continue;
        }
        
        int handled = 0;
        switch(vm.state) {
            case IDLE:
                handled = handle_idle(&vm, input);
                break;
            case SELECTING:
                handled = handle_selecting(&vm, input);
                break;
            case PAYING:
                handled = handle_paying(&vm, input);
                break;
            case DISPENSING:
                handled = handle_dispensing(&vm);
                break;
            case ERROR:
                handled = handle_error(&vm);
                break;
        }
        
        if (!handled) {
            vm.state = ERROR;
        }
        
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}