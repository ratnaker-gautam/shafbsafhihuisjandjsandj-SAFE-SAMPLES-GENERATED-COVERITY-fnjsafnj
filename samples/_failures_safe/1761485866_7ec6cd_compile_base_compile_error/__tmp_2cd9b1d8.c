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
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_by_level(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("INFO: %d\nWARN: %d\nERROR: %d\n", info_count, warn_count, error_count);
}

void find_errors(LogEntry* entries, int count) {
    printf("Error messages:\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

void show_all(LogEntry* entries, int count) {
    printf("All log entries:\n");
    for (int i = 0; i < count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len >= MAX_LINE_LENGTH || len < 10) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= sizeof(temp)) return 0;
    strncpy(temp, line, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0) return;
    processor(entries, count);
}

int main(void) {
    LogEntry entries[MAX_LOG_ENTRIES];
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_LOG_ENTRIES);
    if (entry_count <= 0) {
        fprintf(stderr, "No valid log entries found or file error\n");
        return 1;
    }
    
    printf("Found %d log entries\n", entry_count);
    
    LogProcessor processors[] = {count_by_level, find_errors, show_all};
    const char* names[] = {"Count by level", "Find errors", "Show all"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    while