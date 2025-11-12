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
    int *inventory;
};

int validate_input(const char *input, int min, int max) {
    if (input == NULL || strlen(input) == 0) return -1;
    
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) return -1;
    }
    
    long val = strtol(input, NULL, 10);
    if (val < min || val > max) return -1;
    
    return (int)val;
}

void initialize_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->max_items = 5;
    machine->item_count = machine->max_items;
    machine->inventory = malloc(sizeof(int) * machine->max_items);
    if (machine->inventory == NULL) exit(1);
    
    for (int i = 0; i < machine->max_items; i++) {
        machine->inventory[i] = 10;
    }
}

void cleanup_machine(struct vending_machine *machine) {
    free(machine->inventory);
}

void handle_idle_state(struct vending_machine *machine) {
    char input[32];
    printf("Vending Machine - Current balance: %d\n", machine->balance);
    printf("Options: 1) Insert coin 2) Select item 3) Exit\n");
    printf("Enter choice: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return;
    input[strcspn(input, "\n")] = 0;
    
    int choice = validate_input(input, 1, 3);
    if (choice == -1) {
        printf("Invalid input\n");
        return;
    }
    
    switch (choice) {
        case 1:
            machine->balance += 10;
            printf("Coin inserted. New balance: %d\n", machine->balance);
            break;
        case 2:
            machine->state = SELECTING;
            break;
        case 3:
            printf("Goodbye!\n");
            exit(0);
    }
}

void handle_selecting_state(struct vending_machine *machine) {
    char input[32];
    printf("Available items (price: 20 each):\n");
    for (int i = 0; i < machine->item_count; i++) {
        printf("%d) Item %d (stock: %d)\n", i + 1, i + 1, machine->inventory[i]);
    }
    printf("Select item (1-%d) or 0 to cancel: ", machine->item_count);
    
    if (fgets(input, sizeof(input), stdin) == NULL) return;
    input[strcspn(input, "\n")] = 0;
    
    int selection = validate_input(input, 0, machine->item_count);
    if (selection == -1) {
        printf("Invalid selection\n");
        return;
    }
    
    if (selection == 0) {
        machine->state = IDLE;
        return;
    }
    
    machine->selection = selection - 1;
    machine->state = PROCESSING;
}

void handle_processing_state(struct vending_machine *machine) {
    if (machine->selection < 0 || machine->selection >= machine->item_count) {
        machine->state = ERROR;
        return;
    }
    
    if (machine->inventory[machine->selection] <= 0) {
        printf("Item out of stock\n");
        machine->state = IDLE;
        return;
    }
    
    if (machine->balance < 20) {
        printf("Insufficient balance. Need 20, have %d\n", machine->balance);
        machine->state = IDLE;
        return;
    }
    
    machine->balance -= 20;
    machine->inventory[machine->selection]--;
    machine->state = DISPENSING;
}

void handle_dispensing_state(struct vending_machine *machine) {
    printf("Dispensing item %d...\n", machine->selection + 1);
    printf("Remaining balance: %d\n", machine->balance);
    machine->selection = -1;
    machine->state = IDLE;
}

void handle_error_state(struct vending_machine *machine) {
    printf("System error occurred\n");
    machine->state = IDLE;
    machine->selection = -1;
}

int main(void) {
    struct vending_machine machine;
    initialize_machine(&machine);
    
    printf("Vending Machine Started\n");
    
    while (1) {
        switch (machine.state) {
            case IDLE:
                handle_idle_state(&machine);
                break;
            case SELECTING:
                handle_selecting_state(&machine);
                break;
            case PROCESSING:
                handle_processing_state(&machine);
                break;
            case DISPENSING:
                handle_dispensing_state(&machine);
                break;
            case ERROR:
                handle_error_state(&machine);
                break;
        }
    }
    
    cleanup_machine(&machine);
    return 0;
}