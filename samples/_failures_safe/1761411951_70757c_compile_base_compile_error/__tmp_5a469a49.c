//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(level) \
    (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || \
     strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0)

#define LEVEL_COLOR(level) \
    (strcmp(level, "INFO") == 0 ? "\033[32m" : \
     strcmp(level, "WARN") == 0 ? "\033[33m" : \
     strcmp(level, "ERROR") == 0 ? "\033[31m" : \
     strcmp(level, "DEBUG") == 0 ? "\033[36m" : "\033[0m")

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[8];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (sscanf(temp, "%19s %7s %[^\n]", 
               entry->timestamp, entry->level, entry->message) != 3) {
        return 0;
    }
    
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (entry->timestamp[i] == '\0') break;
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && 
            entry->timestamp[i] != ':' && entry->timestamp[i] != '.') {
            return 0;
        }
    }
    
    return 1;
}

void print_statistics(LogEntry entries[], int count, const char* filter_level) {
    int level_counts[LOG_LEVELS] = {0};
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < LOG_LEVELS; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("\nLog Statistics:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", levels[i], level_counts[i],
               count > 0 ? (level_counts[i] * 100.0) / count : 0.0);
    }
    
    if (filter_level != NULL && IS_VALID_LEVEL(filter_level)) {
        printf("\nFiltered entries (%s):\n", filter_level);
        for (int i = 0; i < count; i++) {
            if (strcmp(entries[i].level, filter_level) == 0) {
                printf("%s %s%s\033[0m %s\n", 
                       entries[i].timestamp, 
                       LEVEL_COLOR(entries[i].level),
                       entries[i].level, 
                       entries[i].message);
            }
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    char filter[8] = {0};
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    printf("Enter 'FILTER LEVEL' to show only specific level: ");
    
    if (fgets(filter, sizeof(filter), stdin) != NULL) {
        filter[strcspn(filter, "\n")] = '\0';
        if (!IS_VALID_LEVEL(filter)) {
            filter[0] = '\0';
        }
    }
    
    printf("\nEnter log data:\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (int i = 0; i < entry_count; i++) {