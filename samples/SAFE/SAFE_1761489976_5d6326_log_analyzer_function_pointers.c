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
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[count])) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0) return;
    processor(entries, count);
}

int main() {
    LogEntry entries[MAX_LOG_ENTRIES];
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = 0;
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_LOG_ENTRIES);
    if (entry_count == 0) {
        printf("No log entries found or error reading file\n");
        return 1;
    }
    
    printf("\nLog Analysis Options:\n");
    printf("1. Count errors\n");
    printf("2. List warnings\n");
    printf("3. Show statistics\n");
    printf("Enter choice (1-3): ");
    
    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Error reading choice\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    LogProcessor processors[] = {count_errors, list_warnings, show_statistics};
    
    if (choice >= 1 && choice <= 3) {
        process_logs(entries, entry_count, processors[choice - 1]);
    } else {
        printf("Invalid choice\n");
        return 1;
    }
    
    return 0;
}