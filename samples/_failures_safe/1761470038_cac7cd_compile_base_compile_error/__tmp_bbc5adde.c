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
    int *item_stock;
};

int validate_selection(struct vending_machine *vm, int selection) {
    if (selection < 0 || selection >= vm->max_items) {
        return 0;
    }
    if (vm->item_stock[selection] <= 0) {
        return 0;
    }
    return 1;
}

int process_payment(struct vending_machine *vm, int selection) {
    int price = (selection + 1) * 10;
    if (vm->balance < price) {
        return 0;
    }
    vm->balance -= price;
    return 1;
}

void initialize_machine(struct vending_machine *vm, int max_items) {
    if (max_items <= 0) {
        max_items = 5;
    }
    vm->max_items = max_items;
    vm->state = IDLE;
    vm->selection = -1;
    vm->balance = 100;
    vm->item_stock = malloc(sizeof(int) * max_items);
    if (vm->item_stock == NULL) {
        vm->state = ERROR;
        return;
    }
    vm->item_count = max_items;
    for (int i = 0; i < max_items; i++) {
        vm->item_stock[i] = 3;
    }
}

void cleanup_machine(struct vending_machine *vm) {
    if (vm->item_stock != NULL) {
        free(vm->item_stock);
        vm->item_stock = NULL;
    }
}

void handle_state_transition(struct vending_machine *vm, int input) {
    switch (vm->state) {
        case IDLE:
            if (input >= 0 && input < vm->max_items) {
                vm->selection = input;
                vm->state = SELECTING;
            } else {
                vm->state = ERROR;
            }
            break;
            
        case SELECTING:
            if (input == 1) {
                if (validate_selection(vm, vm->selection)) {
                    vm->state = PROCESSING;
                } else {
                    vm->state = ERROR;
                }
            } else if (input == 0) {
                vm->selection = -1;
                vm->state = IDLE;
            } else {
                vm->state = ERROR;
            }
            break;
            
        case PROCESSING:
            if (input == 1) {
                if (process_payment(vm, vm->selection)) {
                    vm->state = DISPENSING;
                } else {
                    vm->state = ERROR;
                }
            } else if (input == 0) {
                vm->selection = -1;
                vm->state = IDLE;
            } else {
                vm->state = ERROR;
            }
            break;
            
        case DISPENSING:
            if (vm->selection >= 0 && vm->selection < vm->max_items) {
                vm->item_stock[vm->selection]--;
                printf("Item %d dispensed. Remaining stock: %d\n", 
                       vm->selection, vm->item_stock[vm->selection]);
            }
            vm->selection = -1;
            vm->state = IDLE;
            break;
            
        case ERROR:
            printf("Error occurred. Resetting machine.\n");
            vm->selection = -1;
            vm->state = IDLE;
            break;
    }
}

void display_state(struct vending_machine *vm) {
    switch (vm->state) {
        case IDLE:
            printf("State: IDLE. Available items (0-%d). Balance: %d\n", 
                   vm->max_items - 1, vm->balance);
            for (int i = 0; i < vm->max_items; i++) {
                printf("Item %d: $%d, Stock: %d\n", i, (i+1)*10, vm->item_stock[i]);
            }
            printf("Enter item number to select: ");
            break;
            
        case SELECTING:
            printf("State: SELECTING. Selected item %d. Price: $%d\n", 
                   vm->selection, (vm->selection+1)*10);
            printf("Enter 1 to confirm, 0 to cancel: ");
            break;
            
        case PROCESSING:
            printf("State: PROCESSING. Confirm purchase of item %d for $%d\n", 
                   vm->selection, (vm->selection+1)*10);
            printf("Enter 1 to pay, 0 to cancel: ");
            break;
            
        case DISPENSING:
            printf("State: DISPENSING\n");
            break;
            
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
}

int main() {
    struct vending_machine vm;
    initialize_machine(&vm, 5);
    
    if (vm.state == ERROR) {
        printf("Failed to initialize vending machine\n");
        return 1;
    }
    
    int input;
    int result;
    
    while (1) {
        display_state(&vm);
        
        if (vm.state == DISPENSING || vm.state == ERROR) {
            handle_state_transition(&vm, 0);
            continue;
        }