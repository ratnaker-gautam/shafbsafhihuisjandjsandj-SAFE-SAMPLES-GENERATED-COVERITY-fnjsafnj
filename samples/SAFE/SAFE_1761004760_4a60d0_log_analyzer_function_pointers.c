//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
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
    printf("Processed %d entries so far...\n", count);
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
            fprintf(stderr, "Warning: Maximum entries reached\n");
            break;
        }
        
        if (strlen(line) >= sizeof(line) - 1) {
            fprintf(stderr, "Warning: Line %d too long, skipping\n", line_count);
            continue;
        }
        
        LogEntry entry;
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed == 3) {
            processor(&entry);
        } else {
            fprintf(stderr, "Warning: Malformed log entry on line %d\n", line_count);
        }
    }
    
    if (fclose(file) != 0) {
        fprintf(stderr, "Warning: Error closing file\n");
    }
}

int main() {
    printf("Log Analyzer\n");
    printf("============\n\n");
    
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Error: Empty filename\n");
        return 1;
    }
    
    printf("\n1. Count all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Choose option (1-3): ");
    
    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    LogProcessor processor = NULL;
    
    switch (choice) {
        case 1:
            processor = count_entries;
            printf("\nCounting all entries:\n");
            break;
        case 2:
            processor = show_errors;
            printf("\nShowing errors only:\n");
            break;
        case 3:
            processor = show_warnings;
            printf("\nShowing warnings only:\n");
            break;
        default:
            fprintf(stderr, "Error: Invalid choice\n");
            return 1;
    }
    
    if (processor) {
        process_log_file(filename, processor);
    }
    
    printf("\nAnalysis complete.\n");
    return 0;
}