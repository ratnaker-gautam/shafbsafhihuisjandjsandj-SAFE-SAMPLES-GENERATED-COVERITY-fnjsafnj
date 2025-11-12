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
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed == 3) {
            for (int i = 0; i < strlen(entry.level); i++) {
                entry.level[i] = toupper(entry.level[i]);
            }
            processor(&entry);
            line_count++;
        }
    }
    
    if (fclose(file) != 0) {
        fprintf(stderr, "Warning: Error closing file\n");
    }
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
    
    printf("\n1. Count all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Choose option: ");
    
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
            break;
        case 2:
            processor = show_errors;
            break;
        case 3:
            processor = show_warnings;
            break;
        default:
            fprintf(stderr, "Error: Invalid choice\n");
            return 1;
    }
    
    if (processor) {
        process_log_file(filename, processor);
    }
    
    return 0;
}