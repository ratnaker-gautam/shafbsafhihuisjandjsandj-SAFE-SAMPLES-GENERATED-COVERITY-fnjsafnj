//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LOG_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_errors(LogEntry* entries, int count) {
    int error_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    printf("Total ERROR entries: %d\n", error_count);
}

void list_warnings(LogEntry* entries, int count) {
    printf("WARNING entries:\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "WARNING") == 0) {
            printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

void show_statistics(LogEntry* entries, int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("Log Statistics:\n");
    printf("INFO: %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR: %d entries\n", error_count);
    printf("Total: %d entries\n", count);
}

int parse_log_line(const char* line, LogEntry* entry) {
    char temp_line[MAX_LINE_LENGTH];
    
    if (strlen(line) >= MAX_LINE_LENGTH - 1) {
        return 0;
    }
    
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) {
        return 0;
    }
    
    if (strlen(timestamp) >= 31 || strlen(level) >= 15 || strlen(message) >= 255) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    return 1;
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) && entry_count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[entry_count])) {
                entry_count++;
            }
        }
    }
    
    fclose(file);
    return entry_count;
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (processor && entries && count > 0) {
        processor(entries, count);
    }
}

int main() {
    LogEntry entries[MAX_LOG_ENTRIES];
    char filename[256];
    LogProcessor processors[] = {count_errors, list_warnings, show_statistics};
    const char* processor_names[] = {"Count Errors", "List Warnings", "Show Statistics"};
    int processor_count = 3;
    
    printf("Enter log filename: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading filename\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_LOG_ENTRIES);
    
    if (entry_count < 0) {
        printf("Error opening file: %s\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    printf("\nLoaded %d log entries\n\n", entry_count);
    printf("Available processors:\n");
    for (int i = 0; i < processor_count; i++) {
        printf("%d. %s\n", i + 1, processor_names[i]);
    }
    
    printf("\nSelect processor (1-%d): ", processor_count);
    
    char choice