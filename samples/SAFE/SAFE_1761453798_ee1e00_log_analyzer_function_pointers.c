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
    
    while (fgets(line, sizeof(line), file) && line_count < MAX_ENTRIES) {
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        memset(&entry, 0, sizeof(entry));
        
        char* parts[3];
        char* token = strtok(line, "|");
        int part_count = 0;
        
        while (token && part_count < 3) {
            parts[part_count++] = token;
            token = strtok(NULL, "|");
        }
        
        if (part_count == 3) {
            if (strlen(parts[0]) < sizeof(entry.timestamp)) {
                strncpy(entry.timestamp, parts[0], sizeof(entry.timestamp)-1);
            }
            if (strlen(parts[1]) < sizeof(entry.level)) {
                strncpy(entry.level, parts[1], sizeof(entry.level)-1);
            }
            if (strlen(parts[2]) < sizeof(entry.message)) {
                strncpy(entry.message, parts[2], sizeof(entry.message)-1);
            }
            
            processor(&entry);
        }
        line_count++;
    }
    
    if (fclose(file) != 0) {
        fprintf(stderr, "Error closing file\n");
    }
}

int main() {
    printf("Log Analyzer\n");
    printf("1. Count all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Enter choice (1-3): ");
    
    char input[16];
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int choice;
    if (sscanf(input, "%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    
    if (choice < 1 || choice > 3) {
        fprintf(stderr, "Choice must be between 1 and 3\n");
        return 1;
    }
    
    printf("Enter log filename: ");
    char filename[256];
    if (!fgets(filename, sizeof(filename), stdin)) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Filename cannot be empty\n");
        return 1;
    }
    
    process_log_file(filename, processors[choice-1]);
    
    return 0;
}