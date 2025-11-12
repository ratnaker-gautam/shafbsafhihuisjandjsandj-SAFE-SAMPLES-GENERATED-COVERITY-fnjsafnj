//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            fprintf(stderr, "Warning: Too many entries, processing first %d\n", MAX_ENTRIES);
            break;
        }
        
        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Warning: Line %d too long, skipping\n", line_count);
            continue;
        }
        
        LogEntry entry;
        memset(&entry, 0, sizeof(entry));
        
        int fields = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (fields >= 2) {
            for (int i = 0; entry.level[i]; i++) {
                entry.level[i] = toupper(entry.level[i]);
            }
            processor(&entry);
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
    printf("Choose option (1-4): ");
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Error: Empty filename\n");
        return 1;
    }
    
    int choice;
    while (1) {
        print_menu();
        
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != '\n');
        
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
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    
    return 0;
}