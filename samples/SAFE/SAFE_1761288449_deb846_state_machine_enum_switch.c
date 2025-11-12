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

struct vending_machine {
    enum vending_state state;
    int selection;
    int payment;
    int stock[3];
    int prices[3];
};

void init_machine(struct vending_machine *vm) {
    vm->state = IDLE;
    vm->selection = -1;
    vm->payment = 0;
    vm->stock[0] = 5;
    vm->stock[1] = 3;
    vm->stock[2] = 7;
    vm->prices[0] = 150;
    vm->prices[1] = 200;
    vm->prices[2] = 100;
}

void display_menu(void) {
    printf("Available items:\n");
    printf("1. Soda (150 cents)\n");
    printf("2. Chips (200 cents)\n");
    printf("3. Candy (100 cents)\n");
    printf("Enter selection (1-3) or 0 to exit: ");
}

int get_valid_input(int min, int max) {
    int input;
    char buffer[32];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &input) != 1) {
        return -1;
    }
    
    if (input < min || input > max) {
        return -1;
    }
    
    return input;
}

void handle_idle(struct vending_machine *vm) {
    display_menu();
    int choice = get_valid_input(0, 3);
    
    if (choice == -1) {
        printf("Invalid input. Please try again.\n");
        return;
    }
    
    if (choice == 0) {
        printf("Goodbye!\n");
        exit(0);
    }
    
    vm->selection = choice - 1;
    
    if (vm->stock[vm->selection] <= 0) {
        printf("Item out of stock.\n");
        vm->selection = -1;
        return;
    }
    
    vm->state = SELECTING;
    printf("Selected item %d. Price: %d cents.\n", choice, vm->prices[vm->selection]);
}

void handle_selecting(struct vending_machine *vm) {
    printf("Enter payment in cents (0 to cancel): ");
    int payment = get_valid_input(0, 1000);
    
    if (payment == -1) {
        printf("Invalid payment amount.\n");
        return;
    }
    
    if (payment == 0) {
        printf("Transaction cancelled.\n");
        vm->state = IDLE;
        vm->selection = -1;
        return;
    }
    
    vm->payment = payment;
    vm->state = PAYING;
}

void handle_paying(struct vending_machine *vm) {
    int price = vm->prices[vm->selection];
    
    if (vm->payment < price) {
        printf("Insufficient payment. Need %d more cents.\n", price - vm->payment);
        vm->state = SELECTING;
        return;
    }
    
    if (vm->payment > price) {
        int change = vm->payment - price;
        printf("Dispensing %d cents change.\n", change);
    }
    
    vm->state = DISPENSING;
}

void handle_dispensing(struct vending_machine *vm) {
    printf("Dispensing item...\n");
    vm->stock[vm->selection]--;
    
    printf("Thank you for your purchase!\n");
    vm->state = IDLE;
    vm->selection = -1;
    vm->payment = 0;
}

void handle_error(struct vending_machine *vm) {
    printf("An error occurred. Resetting machine.\n");
    vm->state = IDLE;
    vm->selection = -1;
    vm->payment = 0;
}

int main(void) {
    struct vending_machine vm;
    init_machine(&vm);
    
    printf("Welcome to the Vending Machine!\n");
    
    while (1) {
        switch (vm.state) {
            case IDLE:
                handle_idle(&vm);
                break;
            case SELECTING:
                handle_selecting(&vm);
                break;
            case PAYING:
                handle_paying(&vm);
                break;
            case DISPENSING:
                handle_dispensing(&vm);
                break;
            case ERROR:
                handle_error(&vm);
                break;
            default:
                vm.state = ERROR;
                break;
        }
    }
    
    return 0;
}