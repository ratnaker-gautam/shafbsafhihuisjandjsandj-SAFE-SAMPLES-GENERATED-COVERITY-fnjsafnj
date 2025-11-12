//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum vending_state {
    IDLE,
    SELECTING,
    PAYMENT,
    DISPENSING,
    ERROR
};

struct vending_machine {
    enum vending_state state;
    int selection;
    int payment;
    int balance;
    int item_count;
    int max_items;
};

void machine_init(struct vending_machine *vm) {
    if (vm == NULL) return;
    vm->state = IDLE;
    vm->selection = -1;
    vm->payment = 0;
    vm->balance = 0;
    vm->item_count = 5;
    vm->max_items = 10;
}

int handle_idle(struct vending_machine *vm, int input) {
    if (vm == NULL) return 0;
    if (input < 0 || input > 2) return 0;
    
    if (input == 0) {
        printf("Please select item (1 or 2): ");
        vm->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *vm, int input) {
    if (vm == NULL) return 0;
    if (input < 1 || input > 2) return 0;
    
    vm->selection = input;
    printf("Selected item %d. Price: %d coins. Insert payment: ", input, input * 10);
    vm->state = PAYMENT;
    return 1;
}

int handle_payment(struct vending_machine *vm, int input) {
    if (vm == NULL) return 0;
    if (input < 1 || input > 20) return 0;
    
    vm->payment += input;
    int price = vm->selection * 10;
    
    if (vm->payment >= price) {
        vm->balance = vm->payment - price;
        printf("Payment accepted. Dispensing item %d.\n", vm->selection);
        if (vm->balance > 0) {
            printf("Returning %d coins as change.\n", vm->balance);
        }
        vm->state = DISPENSING;
        return 1;
    } else {
        printf("Inserted %d coins. Need %d more coins: ", vm->payment, price - vm->payment);
        return 1;
    }
}

int handle_dispensing(struct vending_machine *vm, int input) {
    if (vm == NULL) return 0;
    if (input != 0) return 0;
    
    if (vm->item_count > 0) {
        vm->item_count--;
        printf("Item dispensed successfully. %d items remaining.\n", vm->item_count);
    } else {
        printf("Out of items!\n");
    }
    
    vm->selection = -1;
    vm->payment = 0;
    vm->balance = 0;
    vm->state = IDLE;
    printf("Returning to idle state.\n");
    return 1;
}

int handle_error(struct vending_machine *vm, int input) {
    if (vm == NULL) return 0;
    if (input != 0) return 0;
    
    printf("Resetting machine.\n");
    machine_init(vm);
    return 1;
}

int process_input(struct vending_machine *vm, int input) {
    if (vm == NULL) return 0;
    
    switch (vm->state) {
        case IDLE:
            return handle_idle(vm, input);
        case SELECTING:
            return handle_selecting(vm, input);
        case PAYMENT:
            return handle_payment(vm, input);
        case DISPENSING:
            return handle_dispensing(vm, input);
        case ERROR:
            return handle_error(vm, input);
        default:
            vm->state = ERROR;
            return 0;
    }
}

void print_state(struct vending_machine *vm) {
    if (vm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "SELECTING", "PAYMENT", "DISPENSING", "ERROR"
    };
    
    printf("Current state: %s\n", state_names[vm->state]);
    printf("Items available: %d\n", vm->item_count);
}

int main(void) {
    struct vending_machine vm;
    machine_init(&vm);
    
    printf("Vending Machine Simulator\n");
    printf("Commands: 0=Start selection, 1=Item1, 2=Item2, 3-20=Coins, 99=Exit\n");
    
    int running = 1;
    while (running) {
        print_state(&vm);
        printf("Enter command: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input == 99) {
            running = 0;
            continue;
        }
        
        if (!process_input(&vm, input)) {
            printf("Invalid command for current state.\n");
        }
        
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}