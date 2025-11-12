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
    SODA,
    CHIPS,
    CANDY,
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
    vm->selected = SODA;
    vm->balance = 0;
    vm->inventory[SODA] = 10;
    vm->inventory[CHIPS] = 8;
    vm->inventory[CANDY] = 15;
    vm->prices[SODA] = 150;
    vm->prices[CHIPS] = 100;
    vm->prices[CANDY] = 75;
}

void display_state(struct vending_machine *vm) {
    switch (vm->state) {
        case IDLE:
            printf("State: IDLE | Balance: %u cents\n", vm->balance);
            break;
        case SELECTING:
            printf("State: SELECTING | Balance: %u cents\n", vm->balance);
            break;
        case PROCESSING:
            printf("State: PROCESSING | Product: %d\n", vm->selected);
            break;
        case DISPENSING:
            printf("State: DISPENSING | Product: %d\n", vm->selected);
            break;
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
}

void display_products(struct vending_machine *vm) {
    printf("Available products:\n");
    printf("0: Soda - %u cents (%u left)\n", vm->prices[SODA], vm->inventory[SODA]);
    printf("1: Chips - %u cents (%u left)\n", vm->prices[CHIPS], vm->inventory[CHIPS]);
    printf("2: Candy - %u cents (%u left)\n", vm->prices[CANDY], vm->inventory[CANDY]);
}

int handle_idle(struct vending_machine *vm, int choice) {
    if (choice == 1) {
        vm->state = SELECTING;
        return 1;
    } else if (choice == 2) {
        printf("Insert coins (in cents): ");
        int amount;
        if (scanf("%d", &amount) != 1) return 0;
        if (amount <= 0 || amount > 1000) {
            printf("Invalid amount\n");
            return 0;
        }
        if (vm->balance > UINT32_MAX - amount) {
            printf("Balance overflow\n");
            return 0;
        }
        vm->balance += amount;
        printf("New balance: %u cents\n", vm->balance);
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *vm, int choice) {
    if (choice >= 0 && choice < PRODUCT_COUNT) {
        vm->selected = (enum product)choice;
        vm->state = PROCESSING;
        return 1;
    } else if (choice == 9) {
        vm->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *vm) {
    if (vm->inventory[vm->selected] == 0) {
        printf("Product out of stock\n");
        vm->state = ERROR;
        return 0;
    }
    if (vm->balance < vm->prices[vm->selected]) {
        printf("Insufficient funds\n");
        vm->state = ERROR;
        return 0;
    }
    vm->balance -= vm->prices[vm->selected];
    vm->inventory[vm->selected]--;
    vm->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *vm) {
    printf("Dispensing product %d\n", vm->selected);
    if (vm->balance > 0) {
        printf("Returning change: %u cents\n", vm->balance);
        vm->balance = 0;
    }
    vm->state = IDLE;
    return 1;
}

int handle_error(struct vending_machine *vm) {
    vm->state = IDLE;
    return 1;
}

int main(void) {
    struct vending_machine vm;
    init_machine(&vm);
    
    printf("Vending Machine Simulator\n");
    printf("Commands: 1-Select 2-Insert coins 0-9-Product choice 9-Back 0-Exit\n");
    
    int running = 1;
    while (running) {
        display_state(&vm);
        
        switch (vm.state) {
            case IDLE:
                display_products(&vm);
                break;
            case SELECTING:
                display_products(&vm);
                break;
            default:
                break;
        }
        
        printf("Enter command: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 0) {
            running = 0;
            continue;
        }
        
        int result =