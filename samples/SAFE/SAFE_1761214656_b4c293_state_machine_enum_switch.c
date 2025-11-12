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
    int *item_prices;
    int *item_stock;
};

int validate_selection(struct vending_machine *vm, int selection) {
    if (selection < 0 || selection >= vm->max_items) {
        return 0;
    }
    if (vm->item_stock[selection] <= 0) {
        return 0;
    }
    if (vm->balance < vm->item_prices[selection]) {
        return 0;
    }
    return 1;
}

void process_state(struct vending_machine *vm) {
    switch (vm->state) {
        case IDLE:
            printf("Machine ready. Current balance: %d\n", vm->balance);
            break;
            
        case SELECTING:
            if (validate_selection(vm, vm->selection)) {
                vm->state = PROCESSING;
            } else {
                printf("Invalid selection or insufficient funds\n");
                vm->state = ERROR;
            }
            break;
            
        case PROCESSING:
            vm->balance -= vm->item_prices[vm->selection];
            vm->item_stock[vm->selection]--;
            printf("Processing item %d...\n", vm->selection);
            vm->state = DISPENSING;
            break;
            
        case DISPENSING:
            printf("Dispensing item %d. Remaining balance: %d\n", 
                   vm->selection, vm->balance);
            vm->state = IDLE;
            break;
            
        case ERROR:
            printf("Error occurred. Returning to idle state.\n");
            vm->state = IDLE;
            break;
    }
}

int get_valid_input(int min, int max) {
    char input[32];
    int value;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &value) != 1) {
        return -1;
    }
    
    if (value < min || value > max) {
        return -1;
    }
    
    return value;
}

void initialize_machine(struct vending_machine *vm) {
    vm->max_items = 5;
    vm->item_prices = malloc(vm->max_items * sizeof(int));
    vm->item_stock = malloc(vm->max_items * sizeof(int));
    
    if (vm->item_prices == NULL || vm->item_stock == NULL) {
        exit(1);
    }
    
    int prices[] = {50, 75, 100, 125, 150};
    int stock[] = {5, 5, 5, 5, 5};
    
    for (int i = 0; i < vm->max_items; i++) {
        vm->item_prices[i] = prices[i];
        vm->item_stock[i] = stock[i];
    }
    
    vm->state = IDLE;
    vm->balance = 0;
    vm->selection = -1;
}

void cleanup_machine(struct vending_machine *vm) {
    free(vm->item_prices);
    free(vm->item_stock);
}

int main(void) {
    struct vending_machine vm;
    initialize_machine(&vm);
    
    printf("Vending Machine Simulator\n");
    printf("Items: 0($0.50) 1($0.75) 2($1.00) 3($1.25) 4($1.50)\n");
    
    while (1) {
        printf("\nOptions: 1=Add money 2=Select item 3=Exit\n");
        printf("Choice: ");
        
        int choice = get_valid_input(1, 3);
        if (choice == -1) {
            printf("Invalid choice\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Add amount (10-500): ");
                int amount = get_valid_input(10, 500);
                if (amount != -1) {
                    if (vm.balance > 1000 - amount) {
                        printf("Balance too high\n");
                    } else {
                        vm.balance += amount;
                        printf("New balance: %d\n", vm.balance);
                    }
                } else {
                    printf("Invalid amount\n");
                }
                break;
                
            case 2:
                printf("Select item (0-4): ");
                vm.selection = get_valid_input(0, 4);
                if (vm.selection != -1) {
                    vm.state = SELECTING;
                    while (vm.state != IDLE && vm.state != ERROR) {
                        process_state(&vm);
                    }
                } else {
                    printf("Invalid selection\n");
                }
                break;
                
            case 3:
                printf("Returning change: %d\n", vm.balance);
                cleanup_machine(&vm);
                return 0;
        }
    }
    
    cleanup_machine(&vm);
    return 0;
}