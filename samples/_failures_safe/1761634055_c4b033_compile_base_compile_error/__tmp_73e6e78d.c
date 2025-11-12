//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

int validate_log_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

int read_log_file(struct LogEntry entries[], int* count) {
    printf("Enter log data (one entry per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    *count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (*count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (validate_log_level(entry.level)) {
                entries[*count] = entry;
                (*count)++;
            } else {
                printf("Invalid log level in: %s\n", line);
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    return 1;
}

void analyze_logs(const struct LogEntry entries[], int count) {
    if (count == 0) {
        printf("No valid log entries to analyze.\n");
        return;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (error_count > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
}

void search_logs(const struct LogEntry entries[], int count) {
    if (count == 0) {
        printf("No logs available for search.\n");
        return;
    }
    
    char search_term[128];
    printf("Enter search term: ");
    if (fgets(search_term, sizeof(search_term), stdin) == NULL) return;
    
    search_term[strcspn(search_term, "\n")] = 0;
    
    if (strlen(search_term) == 0) {
        printf("Empty search term.\n");
        return;
    }
    
    printf("Search results for '%s':\n", search_term);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, search_term) != NULL) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("=== Log Analyzer ===\n");