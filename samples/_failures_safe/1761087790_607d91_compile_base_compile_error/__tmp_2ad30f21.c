//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LEN 10
#define TIMESTAMP_LEN 20

#define LEVEL_ERROR "ERROR"
#define LEVEL_WARN  "WARN"
#define LEVEL_INFO  "INFO"

#define IS_VALID_LEVEL(l) (strcmp(l, LEVEL_ERROR) == 0 || strcmp(l, LEVEL_WARN) == 0 || strcmp(l, LEVEL_INFO) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[MAX_LEVEL_LEN];
    char message[MAX_LINE_LEN];
} LogEntry;

int parse_timestamp(const char* str, char* timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
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
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp_part = strtok(temp, " ");
    if (timestamp_part == NULL) return 0;
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;
    
    char* level_part = strtok(NULL, " ");
    if (level_part == NULL) return 0;
    if (strlen(level_part) >= MAX_LEVEL_LEN) return 0;
    strncpy(entry->level, level_part, MAX_LEVEL_LEN - 1);
    entry->level[MAX_LEVEL_LEN - 1] = '\0';
    
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    char* message_part = strtok(NULL, "\n");
    if (message_part == NULL) return 0;
    if (strlen(message_part) >= MAX_LINE_LEN) return 0;
    strncpy(entry->message, message_part, MAX_LINE_LEN - 1);
    entry->message[MAX_LINE_LEN - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, LEVEL_ERROR) == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, LEVEL_WARN) == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, LEVEL_INFO) == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int start = (count > 5) ? count - 5 : 0;
        for (int i = start; i < count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log lines (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: ERROR, WARN, INFO\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
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
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            printf("Added: %s [%s] %s\n", entry.t