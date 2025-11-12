//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    if (filename == NULL || processor == NULL) {
        fprintf(stderr, "Invalid input parameters\n");
        return;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        memset(&entry, 0, sizeof(entry));
        
        int scanned = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (scanned == 3) {
            processor(&entry);
            entry_count++;
        }
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
    }
    
    fclose(file);
}

void print_menu() {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Count all log entries\n");
    printf("2. Show only ERROR entries\n");
    printf("3. Show only WARNING entries\n");
    printf("4. Exit\n");
    printf("Choose option: ");
}

int main() {
    char filename[256];
    printf("Enter log filename: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = 0;
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Filename cannot be empty\n");
        return 1;
    }
    
    int choice;
    char input[16];
    
    while (1) {
        print_menu();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            break;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                process_log_file(filename, count_entries);
                break;
            case 2:
                process_log_file(filename, show_errors);
                break;
            case 3:
                process_log_file(filename, show_warnings);
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid option. Please choose 1-4.\n");
                break;
        }
    }
    
    return 0;
}