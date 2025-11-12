//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

void count_log_levels(struct LogEntry* entries, int count, int* info_count, 
                     int* warn_count, int* error_count, int* debug_count) {
    *info_count = *warn_count = *error_count = *debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) (*info_count)++;
        else if (strcmp(entries[i].level, "WARN") == 0) (*warn_count)++;
        else if (strcmp(entries[i].level, "ERROR") == 0) (*error_count)++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) (*debug_count)++;
    }
}

void display_statistics(struct LogEntry* entries, int count) {
    int info_count, warn_count, error_count, debug_count;
    count_log_levels(entries, count, &info_count, &warn_count, &error_count, &debug_count);
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int display_count = count < 5 ? count : 5;
        for (int i = 0; i < display_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry) && is_valid_level(entry.level)) {
            entries[entry_count] = entry;
            entry_count++;
        }
    }
    
    fclose(file);
    return entry_count;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file cannot be read\n");
        return 1;
    }
    
    display_statistics(entries, entry_count);
    
    return 0;
}