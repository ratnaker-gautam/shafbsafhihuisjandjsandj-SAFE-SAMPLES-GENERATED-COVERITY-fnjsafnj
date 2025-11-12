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

int initialize_machine(struct vending_machine *machine) {
    if (machine == NULL) return 0;
    
    machine->max_items = 5;
    machine->item_stock = malloc(machine->max_items * sizeof(int));
    machine->item_prices = malloc(machine->max_items * sizeof(int));
    
    if (machine->item_stock == NULL || machine->item_prices == NULL) {
        return 0;
    }
    
    for (int i = 0; i < machine->max_items; i++) {
        machine->item_stock[i] = 3;
        machine->item_prices[i] = (i + 1) * 10;
    }
    
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->item_count = machine->max_items;
    
    return 1;
}

void cleanup_machine(struct vending_machine *machine) {
    if (machine == NULL) return;
    free(machine->item_stock);
    free(machine->item_prices);
}

int handle_idle_state(struct vending_machine *machine, int input) {
    if (input >= 0 && input < machine->item_count) {
        machine->selection = input;
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting_state(struct vending_machine *machine, int input) {
    if (input > 0 && input <= 100) {
        if (machine->balance > INT_MAX - input) return 0;
        machine->balance += input;
        
        if (machine->balance >= machine->item_prices[machine->selection]) {
            machine->state = PROCESSING;
        }
        return 1;
    }
    return 0;
}

int handle_processing_state(struct vending_machine *machine) {
    if (machine->selection < 0 || machine->selection >= machine->item_count) {
        machine->state = ERROR;
        return 0;
    }
    
    if (machine->item_stock[machine->selection] <= 0) {
        machine->state = ERROR;
        return 0;
    }
    
    if (machine->balance < machine->item_prices[machine->selection]) {
        machine->state = ERROR;
        return 0;
    }
    
    machine->balance -= machine->item_prices[machine->selection];
    machine->item_stock[machine->selection]--;
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing_state(struct vending_machine *machine) {
    int change = machine->balance;
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
    return change;
}

int handle_error_state(struct vending_machine *machine) {
    int refund = machine->balance;
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
    return refund;
}

void print_state(struct vending_machine *machine) {
    printf("Current state: ");
    switch (machine->state) {
        case IDLE: printf("IDLE"); break;
        case SELECTING: printf("SELECTING"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case DISPENSING: printf("DISPENSING"); break;
        case ERROR: printf("ERROR"); break;
    }
    printf("\n");
}

void print_menu(struct vending_machine *machine) {
    printf("Available items:\n");
    for (int i = 0; i < machine->item_count; i++) {
        printf("%d: Price %d, Stock %d\n", i, machine->item_prices[i], machine->item_stock[i]);
    }
    printf("Current balance: %d\n", machine->balance);
}

int main(void) {
    struct vending_machine machine;
    if (!initialize_machine(&machine)) {
        fprintf(stderr, "Failed to initialize vending machine\n");
        return 1;
    }
    
    printf("Vending Machine Simulator\n");
    
    while (1) {
        print_state(&machine);
        print_menu(&machine);
        
        printf("Enter command (-1 to quit, 0-%d to select item, positive amount to insert money): ", machine.item_count - 1);
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (input == -1) {
            break;
        }
        
        int result = 0;
        switch (machine.state) {
            case IDLE:
                result = handle_idle_state(&machine, input);
                if (!result) printf("Invalid selection\n");
                break;
                
            case SELECTING:
                result = handle_selecting_state(&machine, input);
                if (!result) printf("Invalid money amount\n");
                break;
                
            case PROCESSING:
                result = handle_processing_state(&machine);
                if (!result) printf("Processing