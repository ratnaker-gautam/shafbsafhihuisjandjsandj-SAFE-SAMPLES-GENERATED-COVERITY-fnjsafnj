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
    int *stock;
};

int initialize_machine(struct vending_machine *machine, int max_items) {
    if (machine == NULL || max_items <= 0) return 0;
    
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->max_items = max_items;
    machine->item_count = max_items;
    
    machine->stock = malloc(sizeof(int) * max_items);
    if (machine->stock == NULL) return 0;
    
    for (int i = 0; i < max_items; i++) {
        machine->stock[i] = 5;
    }
    
    return 1;
}

void cleanup_machine(struct vending_machine *machine) {
    if (machine != NULL && machine->stock != NULL) {
        free(machine->stock);
    }
}

int validate_selection(struct vending_machine *machine, int selection) {
    if (machine == NULL) return 0;
    if (selection < 0 || selection >= machine->max_items) return 0;
    if (machine->stock[selection] <= 0) return 0;
    return 1;
}

int process_payment(struct vending_machine *machine) {
    if (machine == NULL) return 0;
    
    int required_amount = (machine->selection + 1) * 10;
    if (machine->balance >= required_amount) {
        machine->balance -= required_amount;
        return 1;
    }
    return 0;
}

void handle_state_transition(struct vending_machine *machine, int input) {
    if (machine == NULL) return;
    
    switch (machine->state) {
        case IDLE:
            if (input >= 0 && input < machine->max_items) {
                if (validate_selection(machine, input)) {
                    machine->selection = input;
                    machine->state = SELECTING;
                    printf("Selected item %d. Please insert %d cents.\n", 
                           input, (input + 1) * 10);
                } else {
                    printf("Invalid selection or out of stock.\n");
                }
            } else if (input == -1) {
                printf("Adding 25 cents to balance.\n");
                machine->balance += 25;
                printf("Current balance: %d cents.\n", machine->balance);
            } else {
                printf("Invalid input.\n");
            }
            break;
            
        case SELECTING:
            if (input == -1) {
                printf("Adding 25 cents to balance.\n");
                machine->balance += 25;
                printf("Current balance: %d cents.\n", machine->balance);
                
                if (process_payment(machine)) {
                    machine->state = PROCESSING;
                }
            } else if (input == -2) {
                printf("Cancelling transaction. Returning to idle.\n");
                machine->selection = -1;
                machine->state = IDLE;
            } else {
                printf("Invalid input during selection.\n");
            }
            break;
            
        case PROCESSING:
            if (process_payment(machine)) {
                machine->stock[machine->selection]--;
                machine->state = DISPENSING;
            } else {
                machine->state = ERROR;
            }
            break;
            
        case DISPENSING:
            printf("Dispensing item %d. Enjoy!\n", machine->selection);
            printf("Returning %d cents change.\n", machine->balance);
            machine->balance = 0;
            machine->selection = -1;
            machine->state = IDLE;
            break;
            
        case ERROR:
            printf("Error processing transaction. Returning to idle.\n");
            machine->selection = -1;
            machine->state = IDLE;
            break;
    }
}

int main() {
    struct vending_machine machine;
    const int max_items = 5;
    
    if (!initialize_machine(&machine, max_items)) {
        fprintf(stderr, "Failed to initialize vending machine.\n");
        return 1;
    }
    
    printf("Vending Machine Simulator\n");
    printf("Select item (0-%d) or -1 to add money, -2 to cancel.\n", max_items - 1);
    
    int input;
    char buffer[32];
    
    while (1) {
        printf("\nCurrent state: ");
        switch (machine.state) {
            case IDLE: printf("IDLE"); break;
            case SELECTING: printf("SELECTING"); break;
            case PROCESSING: printf("PROCESSING"); break;
            case DISPENSING: printf("DISPENSING"); break;
            case ERROR: printf("ERROR"); break;
        }
        printf(" | Balance: %d cents\n", machine.balance);
        printf("Enter command: ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        if (strlen(buffer) == 0) continue;
        
        char *endptr;
        input = strtol(buffer, &endptr, 10);
        if (endptr == buffer) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == -3)