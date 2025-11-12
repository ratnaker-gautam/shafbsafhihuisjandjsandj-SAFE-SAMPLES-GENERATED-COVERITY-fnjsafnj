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
    if (!file) {
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char* timestamp = strtok(line, " ");
        char* level = strtok(NULL, " ");
        char* message = strtok(NULL, "\n");
        
        if (timestamp && level && message) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            
            entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
            entries[count].level[sizeof(entries[count].level) - 1] = '\0';
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void count_log_levels(const struct LogEntry* entries, int count, int* error_count, int* warning_count, int* info_count) {
    *error_count = 0;
    *warning_count = 0;
    *info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            (*info_count)++;
        }
    }
}

void search_logs(const struct LogEntry* entries, int count, const char* keyword) {
    if (!keyword || strlen(keyword) == 0) return;
    
    printf("Search results for '%s':\n", keyword);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found containing '%s'\n", keyword);
    }
}

void display_summary(const struct LogEntry* entries, int count) {
    int error_count, warning_count, info_count;
    count_log_levels(entries, count, &error_count, &warning_count, &info_count);
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (count > 0) {
        printf("\nFirst 5 entries:\n");
        int display_count = count < 5 ? count : 5;
        for (int i = 0; i < display_count; i++) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    char search_term[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error: Could not open file '%s'\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No log entries found in file '%s'\n", filename);
        return 0;
    }
    
    printf("Successfully loaded %d log entries\n\n", entry_count);
    
    display_summary(entries, entry_count);
    
    printf("\nEnter search term (or press Enter to skip): ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        len = strlen(search_term);
        if (len > 0 && search_term[len - 1] == '\n') {
            search_term[len - 1] = '\