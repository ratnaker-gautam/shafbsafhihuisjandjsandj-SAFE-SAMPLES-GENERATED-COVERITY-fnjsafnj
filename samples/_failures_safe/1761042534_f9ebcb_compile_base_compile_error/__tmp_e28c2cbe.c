//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum vending_state {
    IDLE,
    SELECTING,
    PROCESSING,
    DISPENSING,
    ERROR
};

enum product {
    COLA,
    CHIPS,
    CANDY,
    WATER,
    PRODUCT_COUNT
};

struct vending_machine {
    enum vending_state state;
    enum product selected_product;
    uint32_t balance;
    uint32_t product_prices[PRODUCT_COUNT];
    uint32_t product_stock[PRODUCT_COUNT];
};

void initialize_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->balance = 0;
    machine->product_prices[COLA] = 150;
    machine->product_prices[CHIPS] = 100;
    machine->product_prices[CANDY] = 75;
    machine->product_prices[WATER] = 50;
    
    for (int i = 0; i < PRODUCT_COUNT; i++) {
        machine->product_stock[i] = 5;
    }
}

void display_state(struct vending_machine *machine) {
    printf("Current state: ");
    switch (machine->state) {
        case IDLE:
            printf("IDLE");
            break;
        case SELECTING:
            printf("SELECTING");
            break;
        case PROCESSING:
            printf("PROCESSING");
            break;
        case DISPENSING:
            printf("DISPENSING");
            break;
        case ERROR:
            printf("ERROR");
            break;
    }
    printf(" | Balance: %u cents\n", machine->balance);
}

void display_products(struct vending_machine *machine) {
    printf("Available products:\n");
    printf("0: Cola - %u cents (Stock: %u)\n", machine->product_prices[COLA], machine->product_stock[COLA]);
    printf("1: Chips - %u cents (Stock: %u)\n", machine->product_prices[CHIPS], machine->product_stock[CHIPS]);
    printf("2: Candy - %u cents (Stock: %u)\n", machine->product_prices[CANDY], machine->product_stock[CANDY]);
    printf("3: Water - %u cents (Stock: %u)\n", machine->product_prices[WATER], machine->product_stock[WATER]);
}

int handle_idle_state(struct vending_machine *machine, int choice) {
    if (choice == 1) {
        machine->state = SELECTING;
        return 1;
    } else if (choice == 2) {
        printf("Inserting 25 cents\n");
        if (machine->balance > UINT32_MAX - 25) {
            printf("Balance overflow!\n");
            return 0;
        }
        machine->balance += 25;
        return 1;
    } else if (choice == 3) {
        printf("Inserting 50 cents\n");
        if (machine->balance > UINT32_MAX - 50) {
            printf("Balance overflow!\n");
            return 0;
        }
        machine->balance += 50;
        return 1;
    } else if (choice == 4) {
        printf("Returning %u cents\n", machine->balance);
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int handle_selecting_state(struct vending_machine *machine, int choice) {
    if (choice >= 0 && choice < PRODUCT_COUNT) {
        if (machine->product_stock[choice] == 0) {
            printf("Product out of stock!\n");
            return 0;
        }
        machine->selected_product = choice;
        machine->state = PROCESSING;
        return 1;
    } else if (choice == 9) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing_state(struct vending_machine *machine) {
    uint32_t price = machine->product_prices[machine->selected_product];
    
    if (machine->balance < price) {
        printf("Insufficient funds! Need %u cents, have %u cents\n", price, machine->balance);
        machine->state = SELECTING;
        return 0;
    }
    
    machine->balance -= price;
    machine->product_stock[machine->selected_product]--;
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing_state(struct vending_machine *machine) {
    printf("Dispensing ");
    switch (machine->selected_product) {
        case COLA:
            printf("Cola");
            break;
        case CHIPS:
            printf("Chips");
            break;
        case CANDY:
            printf("Candy");
            break;
        case WATER:
            printf("Water");
            break;
        default:
            break;
    }
    printf("\n");
    machine->state = IDLE;
    return 1;
}

int main() {
    struct vending_machine machine;
    initialize_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Commands: 1=Select, 2=25c, 3=50c, 4=Return, 0-3=Product, 9=Back\n");
    
    while (1) {
        display_state(&machine);
        
        switch (machine.state) {
            case IDLE:
                display_products(&machine);