//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 16

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define IS_VALID_LEVEL(lvl) (strcmp(lvl, "INFO") == 0 || strcmp(lvl, "WARNING") == 0 || strcmp(lvl, "ERROR") == 0)

int parse_timestamp(const char* str, char* timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    strncpy(timestamp, str, TIMESTAMP_LEN - 1);
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (!parse_timestamp(temp, entry->timestamp)) return 0;
    
    char* rest = temp + TIMESTAMP_LEN - 1;
    if (*rest != ' ') return 0;
    rest++;
    
    char* level_start = rest;
    char* bracket = strchr(level_start, ']');
    if (bracket == NULL) return 0;
    
    size_t level_len = bracket - level_start;
    if (level_len >= LEVEL_LEN || level_len < 2) return 0;
    
    strncpy(entry->level, level_start + 1, level_len - 1);
    entry->level[level_len - 1] = '\0';
    
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    rest = bracket + 1;
    if (*rest != ' ') return 0;
    rest++;
    
    size_t msg_len = strlen(rest);
    if (msg_len == 0) return 0;
    if (msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, rest, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(entries, count, target_level) \
    do { \
        for (size_t i = 0; i < count; i++) { \
            if (strcmp(entries[i].level, target_level) == 0) { \
                printf("  %s: %s\n", entries[i].timestamp, entries[i].message); \
            } \
        } \
    } while(0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    size_t entry_count = 0;
    
    printf("Log Analyzer - Enter log lines (format: YYYY-MM-DD HH:MM:SS [LEVEL] message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
            printf("Log entry added successfully.\n");
        } else {
            printf("Invalid log format. Please use: YYYY-MM-DD HH:MM:SS [LEVEL] message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %zu\n", entry_count);
    
    int info_count = 0, warning_count = 0, error_count = 0;
    for (size_t i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") ==