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
    char message[MAX_LINE_LENGTH];
};

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len-1] == '\n') {
            line[line_len-1] = '\0';
            line_len--;
        }
        
        if (line_len == 0) {
            continue;
        }
        
        char* timestamp = strtok(line, " ");
        char* level = strtok(NULL, " ");
        char* message = strtok(NULL, "");
        
        if (timestamp != NULL && level != NULL && message != NULL) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp)-1);
            entries[count].timestamp[sizeof(entries[count].timestamp)-1] = '\0';
            
            strncpy(entries[count].level, level, sizeof(entries[count].level)-1);
            entries[count].level[sizeof(entries[count].level)-1] = '\0';
            
            strncpy(entries[count].message, message, sizeof(entries[count].message)-1);
            entries[count].message[sizeof(entries[count].message)-1] = '\0';
            
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("  ERROR: %d entries\n", error_count);
    printf("  WARNING: %d entries\n", warning_count);
    printf("  INFO: %d entries\n", info_count);
    printf("  TOTAL: %d entries\n", count);
}

void search_log_messages(struct LogEntry* entries, int count, const char* search_term) {
    if (search_term == NULL || strlen(search_term) == 0) {
        return;
    }
    
    printf("Search results for '%s':\n", search_term);
    int found_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, search_term) != NULL) {
            printf("  [%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found_count++;
        }
    }
    
    printf("Found %d matching entries\n", found_count);
}

void display_recent_entries(struct LogEntry* entries, int count, int num_entries) {
    if (num_entries <= 0 || num_entries > count) {
        num_entries = count;
    }
    
    printf("Most recent %d entries:\n", num_entries);
    
    int start = count - num_entries;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("  [%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    char search_term[256];
    char input[256];
    int choice;
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error opening or reading file: %s\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No log entries found in file\n");
        return 0;
    }
    
    printf("Successfully loaded %d log entries\n\n", entry_count);
    
    while (1) {
        printf("Log Analyzer Menu:\n");
        printf("1. Analyze log levels\n");
        printf("2. Search log messages\n");
        printf("3. Display recent entries