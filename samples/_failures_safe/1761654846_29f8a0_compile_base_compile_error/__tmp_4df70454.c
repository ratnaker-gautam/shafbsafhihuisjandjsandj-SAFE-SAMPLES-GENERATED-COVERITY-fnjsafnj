//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum vending_state {
    IDLE,
    SELECTING,
    PROCESSING,
    DISPENSING,
    ERROR
};

enum product {
    COLA,
    CHIPS,
    CANDY,
    WATER,
    PRODUCT_COUNT
};

struct vending_machine {
    enum vending_state state;
    enum product selected;
    uint32_t balance;
    uint32_t inventory[PRODUCT_COUNT];
    uint32_t prices[PRODUCT_COUNT];
};

void init_machine(struct vending_machine *vm) {
    vm->state = IDLE;
    vm->selected = COLA;
    vm->balance = 0;
    
    vm->inventory[COLA] = 10;
    vm->inventory[CHIPS] = 8;
    vm->inventory[CANDY] = 15;
    vm->inventory[WATER] = 12;
    
    vm->prices[COLA] = 150;
    vm->prices[CHIPS] = 100;
    vm->prices[CANDY] = 75;
    vm->prices[WATER] = 80;
}

int handle_idle(struct vending_machine *vm, int input) {
    if (input >= 0 && input < PRODUCT_COUNT) {
        vm->selected = input;
        vm->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *vm, int input) {
    if (input == 1) {
        if (vm->inventory[vm->selected] > 0) {
            vm->state = PROCESSING;
            return 1;
        } else {
            vm->state = ERROR;
            return 0;
        }
    } else if (input == 0) {
        vm->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *vm, int input) {
    if (input >= 10 && input <= 500) {
        if (vm->balance > UINT32_MAX - input) {
            vm->state = ERROR;
            return 0;
        }
        vm->balance += input;
        
        if (vm->balance >= vm->prices[vm->selected]) {
            vm->state = DISPENSING;
            return 1;
        }
        return 1;
    } else if (input == 0) {
        vm->state = IDLE;
        vm->balance = 0;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *vm) {
    if (vm->inventory[vm->selected] == 0) {
        vm->state = ERROR;
        return 0;
    }
    
    vm->inventory[vm->selected]--;
    vm->balance -= vm->prices[vm->selected];
    
    vm->state = IDLE;
    vm->balance = 0;
    return 1;
}

int handle_error(struct vending_machine *vm, int input) {
    if (input == 0) {
        vm->state = IDLE;
        vm->balance = 0;
        return 1;
    }
    return 0;
}

void print_state(struct vending_machine *vm) {
    const char *product_names[] = {"Cola", "Chips", "Candy", "Water"};
    
    switch (vm->state) {
        case IDLE:
            printf("State: IDLE\n");
            printf("Products: 0-Cola($1.50), 1-Chips($1.00), 2-Candy($0.75), 3-Water($0.80)\n");
            break;
        case SELECTING:
            printf("State: SELECTING\n");
            printf("Selected: %s ($%d.%02d)\n", product_names[vm->selected], 
                   vm->prices[vm->selected] / 100, vm->prices[vm->selected] % 100);
            printf("Options: 1-Confirm, 0-Cancel\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            printf("Selected: %s\n", product_names[vm->selected]);
            printf("Price: $%d.%02d, Balance: $%d.%02d\n", 
                   vm->prices[vm->selected] / 100, vm->prices[vm->selected] % 100,
                   vm->balance / 100, vm->balance % 100);
            printf("Insert coins (10-500 cents) or 0 to cancel: ");
            break;
        case DISPENSING:
            printf("State: DISPENSING\n");
            printf("Dispensing %s\n", product_names[vm->selected]);
            printf("Change: $%d.%02d\n", vm->balance / 100, vm->balance % 100);
            break;
        case ERROR:
            printf("State: ERROR\n");
            printf("Error occurred. Press 0 to reset: ");
            break;
    }
}

int main(void) {
    struct vending_machine vm;
    init_machine(&vm);
    int input;
    
    printf("Vending Machine Simulator\n");
    printf("Enter -1 to exit\n\n");
    
    while (1) {
        print_state(&vm);
        
        if (vm.state == DISPENSING) {
            handle_dispensing(&vm);
            continue;
        }