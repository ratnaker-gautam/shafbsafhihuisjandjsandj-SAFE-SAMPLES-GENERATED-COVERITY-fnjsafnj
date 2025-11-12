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
    
    if (fclose(file) != 0) {
        fprintf(stderr, "Warning: Error closing file\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    if (strlen(argv[1]) == 0) {
        fprintf(stderr, "Error: Empty filename\n");
        return 1;
    }
    
    printf("1. Count all entries\n");
    printf("2. Show only errors\n");
    printf("3. Show only warnings\n");
    printf("Choose option (1-3): ");
    
    char input[16];
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int choice;
    if (sscanf(input, "%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    
    if (choice < 1 || choice > 3) {
        fprintf(stderr, "Choice must be between 1 and 3\n");
        return 1;
    }
    
    process_log_file(argv[1], processors[choice - 1]);
    
    return 0;
}