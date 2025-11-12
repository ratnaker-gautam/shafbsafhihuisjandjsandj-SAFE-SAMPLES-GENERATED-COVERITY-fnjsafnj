//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    int *item_prices;
    int *item_stock;
};

void machine_init(struct vending_machine *vm, int max_items) {
    if (vm == NULL || max_items <= 0) return;
    
    vm->state = IDLE;
    vm->selection = -1;
    vm->balance = 0;
    vm->max_items = max_items;
    vm->item_count = 0;
    
    vm->item_prices = malloc(max_items * sizeof(int));
    vm->item_stock = malloc(max_items * sizeof(int));
    
    if (vm->item_prices == NULL || vm->item_stock == NULL) {
        vm->state = ERROR;
        return;
    }
    
    for (int i = 0; i < max_items; i++) {
        vm->item_prices[i] = 0;
        vm->item_stock[i] = 0;
    }
}

void machine_cleanup(struct vending_machine *vm) {
    if (vm == NULL) return;
    free(vm->item_prices);
    free(vm->item_stock);
}

int add_item(struct vending_machine *vm, int price, int stock) {
    if (vm == NULL || price < 0 || stock < 0 || vm->state == ERROR) return -1;
    if (vm->item_count >= vm->max_items) return -1;
    
    int idx = vm->item_count;
    vm->item_prices[idx] = price;
    vm->item_stock[idx] = stock;
    vm->item_count++;
    return idx;
}

void process_state(struct vending_machine *vm, int input) {
    if (vm == NULL || vm->state == ERROR) return;
    
    switch (vm->state) {
        case IDLE:
            if (input >= 0 && input < vm->item_count) {
                vm->selection = input;
                vm->state = SELECTING;
                printf("Selected item %d. Price: %d\n", input, vm->item_prices[input]);
            } else if (input == -1) {
                printf("Returning to idle.\n");
            } else {
                printf("Invalid selection.\n");
            }
            break;
            
        case SELECTING:
            if (input > 0) {
                if (vm->balance > INT_MAX - input) {
                    printf("Balance overflow.\n");
                    vm->state = ERROR;
                    break;
                }
                vm->balance += input;
                printf("Added %d. Balance: %d\n", input, vm->balance);
                
                if (vm->balance >= vm->item_prices[vm->selection]) {
                    vm->state = PROCESSING;
                }
            } else if (input == 0) {
                vm->state = PROCESSING;
            } else if (input == -1) {
                printf("Cancelled. Returning change: %d\n", vm->balance);
                vm->balance = 0;
                vm->selection = -1;
                vm->state = IDLE;
            } else {
                printf("Invalid input.\n");
            }
            break;
            
        case PROCESSING:
            if (vm->selection < 0 || vm->selection >= vm->item_count) {
                printf("Invalid selection.\n");
                vm->state = ERROR;
            } else if (vm->item_stock[vm->selection] <= 0) {
                printf("Item out of stock.\n");
                vm->state = ERROR;
            } else if (vm->balance < vm->item_prices[vm->selection]) {
                printf("Insufficient funds. Need %d more.\n", 
                       vm->item_prices[vm->selection] - vm->balance);
                vm->state = SELECTING;
            } else {
                vm->balance -= vm->item_prices[vm->selection];
                vm->item_stock[vm->selection]--;
                printf("Dispensing item %d. Change: %d\n", vm->selection, vm->balance);
                vm->state = DISPENSING;
            }
            break;
            
        case DISPENSING:
            if (input == -1) {
                vm->balance = 0;
                vm->selection = -1;
                vm->state = IDLE;
                printf("Transaction complete.\n");
            } else {
                printf("Take your item and press -1 to continue.\n");
            }
            break;
            
        case ERROR:
            if (input == -1) {
                vm->balance = 0;
                vm->selection = -1;
                vm->state = IDLE;
                printf("Reset to idle.\n");
            } else {
                printf("Machine error. Press -1 to reset.\n");
            }
            break;
    }
}

int main() {
    struct vending_machine vm;
    machine_init(&vm, 5);
    
    if (vm.state == ERROR) {
        printf("Failed to initialize machine.\n");
        return 1;
    }
    
    add_item(&vm, 100, 3);
    add_item(&vm, 150, 2);
    add_item(&vm, 75, 5);
    
    printf("Vending Machine