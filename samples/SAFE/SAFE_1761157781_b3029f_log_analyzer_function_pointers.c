//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

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

void find_errors(LogEntry* entry) {
    if (strstr(entry->level, "ERROR") != NULL) {
        printf("ERROR found: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strstr(entry->level, "WARN") != NULL) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19) return 0;
    if (strlen(level) >= 9) return 0;
    if (strlen(message) >= MAX_LINE_LENGTH - 1) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void process_log_file(const char* filename, LogProcessor processor) {
    if (filename == NULL || processor == NULL) return;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        line[MAX_LINE_LENGTH - 1] = '\0';
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            processor(&entry);
            entry_count++;
        }
    }
    
    if (ferror(file)) {
        printf("Error reading file\n");
    }
    
    fclose(file);
}

int main() {
    printf("Log Analyzer\n");
    printf("Available operations:\n");
    printf("1. Count log entries\n");
    printf("2. Find ERROR entries\n");
    printf("3. Show WARNING entries\n");
    
    int choice;
    printf("Enter choice (1-3): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    char filename[256];
    printf("Enter log filename: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid filename\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_entries, find_errors, show_warnings};
    process_log_file(filename, processors[choice - 1]);
    
    return 0;
}