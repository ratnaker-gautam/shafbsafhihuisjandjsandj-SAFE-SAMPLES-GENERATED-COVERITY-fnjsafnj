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
    vm->item_prices = NULL;
    vm->item_stock = NULL;
}

int add_item(struct vending_machine *vm, int price, int stock) {
    if (vm == NULL || vm->state == ERROR) return 0;
    if (price < 0 || stock < 0) return 0;
    if (vm->item_count >= vm->max_items) return 0;
    
    int idx = vm->item_count;
    vm->item_prices[idx] = price;
    vm->item_stock[idx] = stock;
    vm->item_count++;
    
    return 1;
}

int insert_coin(struct vending_machine *vm, int amount) {
    if (vm == NULL || vm->state == ERROR) return 0;
    if (amount <= 0) return 0;
    
    if (vm->balance > INT_MAX - amount) return 0;
    vm->balance += amount;
    
    if (vm->state == IDLE) {
        vm->state = SELECTING;
    }
    
    return 1;
}

int select_item(struct vending_machine *vm, int item_id) {
    if (vm == NULL || vm->state == ERROR) return 0;
    if (item_id < 0 || item_id >= vm->item_count) return 0;
    
    vm->selection = item_id;
    
    if (vm->state == SELECTING || vm->state == IDLE) {
        vm->state = PROCESSING;
    }
    
    return 1;
}

int process_transaction(struct vending_machine *vm) {
    if (vm == NULL || vm->state == ERROR) return 0;
    if (vm->selection < 0 || vm->selection >= vm->item_count) return 0;
    
    int price = vm->item_prices[vm->selection];
    int stock = vm->item_stock[vm->selection];
    
    if (stock <= 0) {
        vm->state = ERROR;
        return 0;
    }
    
    if (vm->balance < price) {
        vm->state = ERROR;
        return 0;
    }
    
    vm->balance -= price;
    vm->item_stock[vm->selection]--;
    vm->state = DISPENSING;
    
    return 1;
}

void dispense_item(struct vending_machine *vm) {
    if (vm == NULL || vm->state == ERROR) return;
    
    if (vm->state == DISPENSING) {
        printf("Dispensing item %d\n", vm->selection);
        vm->state = IDLE;
        vm->selection = -1;
    }
}

void return_change(struct vending_machine *vm) {
    if (vm == NULL || vm->state == ERROR) return;
    
    if (vm->balance > 0) {
        printf("Returning change: %d\n", vm->balance);
        vm->balance = 0;
    }
}

void reset_machine(struct vending_machine *vm) {
    if (vm == NULL) return;
    
    return_change(vm);
    vm->state = IDLE;
    vm->selection = -1;
}

int main(void) {
    struct vending_machine vm;
    machine_init(&vm, 5);
    
    if (vm.state == ERROR) {
        printf("Machine initialization failed\n");
        return 1;
    }
    
    add_item(&vm, 100, 3);
    add_item(&vm, 150, 2);
    add_item(&vm, 75, 5);
    
    printf("Vending Machine Simulation\n");
    
    insert_coin(&vm, 200);
    select_item(&vm, 1);
    
    if (process_transaction(&vm)) {
        dispense_item(&vm);
    } else {
        printf("Transaction failed\n");
        reset_machine(&vm);
    }
    
    return_change(&vm);
    
    insert_coin(&vm, 50);
    select_item(&vm, 2);
    
    if (process_transaction(&vm)) {
        dispense_item