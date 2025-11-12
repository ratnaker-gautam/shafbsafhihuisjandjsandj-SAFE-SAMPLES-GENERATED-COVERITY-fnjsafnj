//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
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
            fprintf(stderr, "Error: Line %d too long\n", line_count);
            continue;
        }

        LogEntry entry;
        int parsed = sscanf(line, "%19s %9s %[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed == 3) {
            processor(&entry);
        } else {
            fprintf(stderr, "Warning: Malformed log entry at line %d\n", line_count);
        }
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Warning: Error closing file\n");
    }
}

int main() {
    printf("Log Analyzer\n");
    printf("Available processors:\n");
    printf("1. Count entries\n");
    printf("2. Show errors\n");
    printf("3. Show warnings\n");

    int choice;
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }

    if (choice < 1 || choice > 3) {
        fprintf(stderr, "Error: Choice must be between 1 and 3\n");
        return 1;
    }

    char filename[256];
    printf("Enter log filename: ");
    
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error: Invalid filename\n");
        return 1;
    }

    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    LogProcessor selected = processors[choice - 1];

    process_log_file(filename, selected);

    return 0;
}