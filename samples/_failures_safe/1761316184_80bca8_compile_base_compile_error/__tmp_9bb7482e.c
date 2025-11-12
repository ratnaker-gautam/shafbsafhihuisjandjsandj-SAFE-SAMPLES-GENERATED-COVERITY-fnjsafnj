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
    int *item_stock;
    int *item_prices;
};

void machine_init(struct vending_machine *vm, int max_items) {
    if (vm == NULL || max_items <= 0) return;
    
    vm->state = IDLE;
    vm->selection = -1;
    vm->balance = 0;
    vm->max_items = max_items;
    vm->item_count = 0;
    
    vm->item_stock = malloc(sizeof(int) * max_items);
    vm->item_prices = malloc(sizeof(int) * max_items);
    
    if (vm->item_stock == NULL || vm->item_prices == NULL) {
        vm->state = ERROR;
        return;
    }
    
    for (int i = 0; i < max_items; i++) {
        vm->item_stock[i] = 0;
        vm->item_prices[i] = 0;
    }
}

void machine_cleanup(struct vending_machine *vm) {
    if (vm == NULL) return;
    free(vm->item_stock);
    free(vm->item_prices);
}

int add_item(struct vending_machine *vm, int price, int stock) {
    if (vm == NULL || vm->state == ERROR) return -1;
    if (price < 0 || stock < 0 || vm->item_count >= vm->max_items) return -1;
    
    int idx = vm->item_count;
    vm->item_prices[idx] = price;
    vm->item_stock[idx] = stock;
    vm->item_count++;
    return idx;
}

void insert_coin(struct vending_machine *vm, int amount) {
    if (vm == NULL || vm->state == ERROR) return;
    if (amount <= 0) return;
    
    if (vm->balance > INT_MAX - amount) {
        vm->state = ERROR;
        return;
    }
    
    vm->balance += amount;
    
    switch (vm->state) {
        case IDLE:
            vm->state = SELECTING;
            break;
        case SELECTING:
        case PROCESSING:
            break;
        default:
            vm->state = ERROR;
            break;
    }
}

int make_selection(struct vending_machine *vm, int item_id) {
    if (vm == NULL || vm->state == ERROR) return 0;
    if (item_id < 0 || item_id >= vm->item_count) return 0;
    
    switch (vm->state) {
        case SELECTING:
            if (vm->item_stock[item_id] > 0 && vm->balance >= vm->item_prices[item_id]) {
                vm->selection = item_id;
                vm->state = PROCESSING;
                return 1;
            }
            break;
        default:
            break;
    }
    return 0;
}

void process_transaction(struct vending_machine *vm) {
    if (vm == NULL || vm->state == ERROR) return;
    
    switch (vm->state) {
        case PROCESSING:
            if (vm->selection >= 0 && vm->selection < vm->item_count) {
                if (vm->item_stock[vm->selection] > 0 && vm->balance >= vm->item_prices[vm->selection]) {
                    vm->balance -= vm->item_prices[vm->selection];
                    vm->item_stock[vm->selection]--;
                    vm->state = DISPENSING;
                } else {
                    vm->state = ERROR;
                }
            } else {
                vm->state = ERROR;
            }
            break;
        default:
            vm->state = ERROR;
            break;
    }
}

void dispense_item(struct vending_machine *vm) {
    if (vm == NULL || vm->state == ERROR) return;
    
    switch (vm->state) {
        case DISPENSING:
            printf("Dispensing item %d\n", vm->selection);
            vm->selection = -1;
            vm->state = IDLE;
            break;
        default:
            vm->state = ERROR;
            break;
    }
}

void cancel_transaction(struct vending_machine *vm) {
    if (vm == NULL || vm->state == ERROR) return;
    
    switch (vm->state) {
        case SELECTING:
        case PROCESSING:
            printf("Returning %d coins\n", vm->balance);
            vm->balance = 0;
            vm->selection = -1;
            vm->state = IDLE;
            break;
        default:
            vm->state = ERROR;
            break;
    }
}

int main() {
    struct vending_machine vm;
    machine_init(&vm, 5);
    
    if (vm.state == ERROR) {
        printf("Machine initialization failed\n");
        return 1;
    }
    
    add_item(&vm, 100, 3);
    add_item(&vm, 150, 2);
    add_item(&vm, 200, 1);
    
    printf("Vending Machine Simulation\n");
    
    insert_coin(&vm, 50);
    insert_coin(&vm, 50);
    
    if (make_selection(&vm, 0)) {
        process