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
    printf("Processed %d entries so far\n", count);
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

void parse_log_line(const char* line, LogEntry* entry) {
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) {
        return;
    }
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp && level && message) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
    }
}

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file: %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0) {
            line[strcspn(line, "\n")] = '\0';
            parse_log_line(line, &entries[count]);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (!processor || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count;
    
    printf("Enter log filename: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count == 0) {
        printf("No log entries found or file error\n");
        return 1;
    }
    
    printf("\nChoose processing mode:\n");
    printf("1. Count entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Enter choice (1-3): ");
    
    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Input error\n");
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
            printf("Invalid choice\n");
            return 1;
    }
    
    printf("\nProcessing logs...\n");
    process_logs(entries, entry_count, processor);
    
    return 0;
}