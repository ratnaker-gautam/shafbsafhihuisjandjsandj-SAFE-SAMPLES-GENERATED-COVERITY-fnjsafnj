//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
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

enum product {
    COLA,
    CHIPS,
    CANDY,
    PRODUCT_COUNT
};

struct vending_machine {
    enum vending_state state;
    enum product selected_product;
    uint32_t balance;
    uint32_t product_prices[PRODUCT_COUNT];
    uint32_t product_stock[PRODUCT_COUNT];
};

void init_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selected_product = COLA;
    machine->balance = 0;
    machine->product_prices[COLA] = 150;
    machine->product_prices[CHIPS] = 100;
    machine->product_prices[CANDY] = 75;
    machine->product_stock[COLA] = 5;
    machine->product_stock[CHIPS] = 3;
    machine->product_stock[CANDY] = 7;
}

void print_menu(struct vending_machine *machine) {
    printf("Available products:\n");
    printf("0: Cola - %u cents (stock: %u)\n", machine->product_prices[COLA], machine->product_stock[COLA]);
    printf("1: Chips - %u cents (stock: %u)\n", machine->product_prices[CHIPS], machine->product_stock[CHIPS]);
    printf("2: Candy - %u cents (stock: %u)\n", machine->product_prices[CANDY], machine->product_stock[CANDY]);
    printf("Current balance: %u cents\n", machine->balance);
}

void handle_idle(struct vending_machine *machine) {
    printf("Machine is idle. Press 's' to select product, 'q' to quit: ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        machine->state = ERROR;
        return;
    }
    
    if (input[0] == 's' && input[1] == '\n') {
        machine->state = SELECTING;
    } else if (input[0] == 'q' && input[1] == '\n') {
        exit(0);
    } else {
        printf("Invalid input.\n");
    }
}

void handle_selecting(struct vending_machine *machine) {
    print_menu(machine);
    printf("Select product (0-2) or 'b' to go back: ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        machine->state = ERROR;
        return;
    }
    
    if (input[0] == 'b' && input[1] == '\n') {
        machine->state = IDLE;
        return;
    }
    
    if (strlen(input) != 2 || input[1] != '\n') {
        printf("Invalid input.\n");
        return;
    }
    
    int product_num = input[0] - '0';
    if (product_num < 0 || product_num >= PRODUCT_COUNT) {
        printf("Invalid product selection.\n");
        return;
    }
    
    enum product selected = (enum product)product_num;
    if (machine->product_stock[selected] == 0) {
        printf("Product out of stock.\n");
        return;
    }
    
    machine->selected_product = selected;
    machine->state = PAYING;
    printf("Selected product %d. Price: %u cents.\n", product_num, machine->product_prices[selected]);
}

void handle_paying(struct vending_machine *machine) {
    uint32_t price = machine->product_prices[machine->selected_product];
    printf("Current balance: %u cents. Need: %u cents.\n", machine->balance, price);
    printf("Insert coins (1=10, 2=25, 5=50, 10=100) or 'c' to cancel: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        machine->state = ERROR;
        return;
    }
    
    if (input[0] == 'c' && input[1] == '\n') {
        machine->state = SELECTING;
        return;
    }
    
    if (strlen(input) != 2 || input[1] != '\n') {
        printf("Invalid input.\n");
        return;
    }
    
    int coin = input[0] - '0';
    int valid_coins[] = {1, 2, 5, 10};
    int coin_values[] = {10, 25, 50, 100};
    int valid = 0;
    int coin_value = 0;
    
    for (int i = 0; i < 4; i++) {
        if (coin == valid_coins[i]) {
            valid = 1;
            coin_value = coin_values[i];
            break;
        }
    }
    
    if (!valid) {
        printf("Invalid coin.\n");
        return;
    }
    
    if (machine->balance > UINT32_MAX - coin_value) {
        printf("Balance overflow.\n");
        machine->state = ERROR;
        return;
    }
    
    machine->balance += coin_value;
    
    if (machine->balance >= price) {
        machine->state = DISPENSING