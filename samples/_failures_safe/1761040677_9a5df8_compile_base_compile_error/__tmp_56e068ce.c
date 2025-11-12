//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    if (price < 0 || stock < 0) return -1;
    if (vm->item_count >= vm->max_items) return -1;
    
    int idx = vm->item_count;
    vm->item_prices[idx] = price;
    vm->item_stock[idx] = stock;
    vm->item_count++;
    return idx;
}

void insert_coin(struct vending_machine *vm, int amount) {
    if (vm == NULL || amount <= 0) return;
    
    switch (vm->state) {
        case IDLE:
        case SELECTING:
            if (vm->balance > 1000 - amount) {
                vm->state = ERROR;
                return;
            }
            vm->balance += amount;
            vm->state = SELECTING;
            break;
        default:
            break;
    }
}

int select_item(struct vending_machine *vm, int item_id) {
    if (vm == NULL) return 0;
    
    switch (vm->state) {
        case SELECTING:
            if (item_id < 0 || item_id >= vm->item_count) {
                vm->state = ERROR;
                return 0;
            }
            if (vm->item_stock[item_id] <= 0) {
                return 0;
            }
            if (vm->balance < vm->item_prices[item_id]) {
                return 0;
            }
            vm->selection = item_id;
            vm->state = PROCESSING;
            return 1;
        default:
            return 0;
    }
}

int process_purchase(struct vending_machine *vm) {
    if (vm == NULL) return 0;
    
    switch (vm->state) {
        case PROCESSING:
            if (vm->selection < 0 || vm->selection >= vm->item_count) {
                vm->state = ERROR;
                return 0;
            }
            if (vm->item_stock[vm->selection] <= 0) {
                vm->state = ERROR;
                return 0;
            }
            if (vm->balance < vm->item_prices[vm->selection]) {
                vm->state = ERROR;
                return 0;
            }
            
            vm->balance -= vm->item_prices[vm->selection];
            vm->item_stock[vm->selection]--;
            vm->state = DISPENSING;
            return 1;
        default:
            return 0;
    }
}

void dispense_item(struct vending_machine *vm) {
    if (vm == NULL) return;
    
    switch (vm->state) {
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
    if (vm == NULL) return;
    
    switch (vm->state) {
        case IDLE:
        case SELECTING:
        case ERROR:
            printf("Returning %d coins\n", vm->balance);
            vm->balance = 0;
            vm->state = IDLE;
            break;
        default:
            break;
    }
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
    
    printf("Items available:\n");
    for (int i = 0; i < vm->item_count; i++) {
        printf("  %d: Price %d, Stock %d