//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int handle_idle(struct vending_machine *vm, uint32_t coin) {
    if (coin > 500) {
        return 0;
    }
    
    if (vm->balance + coin < vm->balance) {
        return 0;
    }
    
    vm->balance += coin;
    vm->state = SELECTING;
    return 1;
}

int handle_selecting(struct vending_machine *vm, enum product choice) {
    if (choice >= PRODUCT_COUNT) {
        return 0;
    }
    
    if (vm->inventory[choice] == 0) {
        vm->state = ERROR;
        return 0;
    }
    
    if (vm->balance < vm->prices[choice]) {
        return 0;
    }
    
    vm->selected = choice;
    vm->state = PROCESSING;
    return 1;
}

int handle_processing(struct vending_machine *vm) {
    if (vm->selected >= PRODUCT_COUNT) {
        vm->state = ERROR;
        return 0;
    }
    
    if (vm->inventory[vm->selected] == 0) {
        vm->state = ERROR;
        return 0;
    }
    
    if (vm->balance < vm->prices[vm->selected]) {
        vm->state = ERROR;
        return 0;
    }
    
    vm->balance -= vm->prices[vm->selected];
    vm->inventory[vm->selected]--;
    vm->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *vm) {
    uint32_t change = vm->balance;
    vm->balance = 0;
    vm->state = IDLE;
    
    printf("Dispensed product. Change: %u\n", change);
    return 1;
}

int handle_error(struct vending_machine *vm) {
    uint32_t refund = vm->balance;
    vm->balance = 0;
    vm->state = IDLE;
    
    printf("Error occurred. Refund: %u\n", refund);
    return 0;
}

void print_status(struct vending_machine *vm) {
    printf("Current state: ");
    switch (vm->state) {
        case IDLE: printf("IDLE"); break;
        case SELECTING: printf("SELECTING"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case DISPENSING: printf("DISPENSING"); break;
        case ERROR: printf("ERROR"); break;
    }
    printf("\nBalance: %u\n", vm->balance);
    
    printf("Products:\n");
    printf("0: Cola - %u cents (%u left)\n", vm->prices[COLA], vm->inventory[COLA]);
    printf("1: Chips - %u cents (%u left)\n", vm->prices[CHIPS], vm->inventory[CHIPS]);
    printf("2: Candy - %u cents (%u left)\n", vm->prices[CANDY], vm->inventory[CANDY]);
    printf("3: Water - %u cents (%u left)\n", vm->prices[WATER], vm->inventory[WATER]);
}

int main(void) {
    struct vending_machine vm;
    init_machine(&vm);
    
    char input[32];
    int running = 1;
    
    printf("Vending Machine Simulator\n");
    printf("Commands: coin <amount>, select <0-3>, process, status, quit\n");
    
    while (running) {
        printf("\n> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
        } else if (strncmp(input, "coin ", 5) == 0) {
            char *endptr;
            long amount = strtol(input + 5, &endptr, 10);
            
            if (endptr == input + 5 || *endptr != '\0' || amount <= 0 || amount > 500) {
                printf("Invalid coin amount\n");
                continue;
            }
            
            switch (vm