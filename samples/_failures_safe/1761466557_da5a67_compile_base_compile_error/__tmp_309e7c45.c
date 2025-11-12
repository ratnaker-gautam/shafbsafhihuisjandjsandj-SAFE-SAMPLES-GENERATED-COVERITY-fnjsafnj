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
    if (vm == NULL || vm->state == ERROR) return -1;
    if (price < 0 || stock < 0) return -1;
    if (vm->item_count >= vm->max_items) return -1;
    
    int idx = vm->item_count;
    vm->item_prices[idx] = price;
    vm->item_stock[idx] = stock;
    vm->item_count++;
    return idx;
}

void insert_coin(struct vending_machine *vm, int amount) {
    if (vm == NULL || vm->state == ERROR) return;
    if (amount <= 0) return;
    
    if (vm->state == IDLE || vm->state == SELECTING) {
        if (vm->balance > INT_MAX - amount) {
            vm->state = ERROR;
            return;
        }
        vm->balance += amount;
        vm->state = SELECTING;
    }
}

int make_selection(struct vending_machine *vm, int selection) {
    if (vm == NULL || vm->state == ERROR) return 0;
    if (selection < 0 || selection >= vm->item_count) return 0;
    
    if (vm->state == SELECTING) {
        if (vm->item_stock[selection] <= 0) return 0;
        if (vm->balance < vm->item_prices[selection]) return 0;
        
        vm->selection = selection;
        vm->state = PROCESSING;
        return 1;
    }
    return 0;
}

void process_transaction(struct vending_machine *vm) {
    if (vm == NULL || vm->state == ERROR) return;
    
    switch (vm->state) {
        case PROCESSING:
            if (vm->selection < 0 || vm->selection >= vm->item_count) {
                vm->state = ERROR;
                return;
            }
            if (vm->item_stock[vm->selection] <= 0) {
                vm->state = ERROR;
                return;
            }
            if (vm->balance < vm->item_prices[vm->selection]) {
                vm->state = ERROR;
                return;
            }
            
            vm->balance -= vm->item_prices[vm->selection];
            vm->item_stock[vm->selection]--;
            vm->state = DISPENSING;
            break;
            
        case DISPENSING:
            printf("Dispensing item %d\n", vm->selection);
            vm->selection = -1;
            vm->state = IDLE;
            break;
            
        default:
            break;
    }
}

void return_coins(struct vending_machine *vm) {
    if (vm == NULL || vm->state == ERROR) return;
    
    if (vm->balance > 0) {
        printf("Returning %d coins\n", vm->balance);
        vm->balance = 0;
    }
    vm->selection = -1;
    vm->state = IDLE;
}

void print_status(struct vending_machine *vm) {
    if (vm == NULL) return;
    
    printf("State: ");
    switch (vm->state) {
        case IDLE: printf("IDLE"); break;
        case SELECTING: printf("SELECTING"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case DISPENSING: printf("DISPENSING"); break;
        case ERROR: printf("ERROR"); break;
    }
    printf(", Balance: %d\n", vm->balance);
    
    printf("Available items:\n");
    for (int i = 0; i < vm->item_count; i++) {
        printf("  %d: Price %d, Stock %d\n", i, vm->item_prices[i], vm->item_stock[i]);
    }
}

int main(void) {
    struct vending_machine vm;
    machine_init(&vm, 10);
    
    if (vm.state == ERROR) {
        printf("Machine initialization failed\n");
        return 1;
    }