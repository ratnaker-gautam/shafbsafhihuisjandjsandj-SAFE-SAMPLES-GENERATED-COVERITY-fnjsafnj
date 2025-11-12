//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
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
#define LEVEL_WARN "WARN"
#define LEVEL_INFO "INFO"
#define LEVEL_DEBUG "DEBUG"

#define IS_VALID_LEVEL(level) \
    (strcmp(level, LEVEL_ERROR) == 0 || \
     strcmp(level, LEVEL_WARN) == 0 || \
     strcmp(level, LEVEL_INFO) == 0 || \
     strcmp(level, LEVEL_DEBUG) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[MAX_LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - MAX_LEVEL_LEN - 3];
} LogEntry;

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
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (!parse_timestamp(temp, entry->timestamp)) return 0;
    
    char* rest = temp + TIMESTAMP_LEN - 1;
    if (*rest != ' ') return 0;
    rest++;
    
    char* level_start = rest;
    char* level_end = strchr(level_start, ' ');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= MAX_LEVEL_LEN) return 0;
    
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    char* message_start = level_end + 1;
    if (*message_start == '\0') return 0;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    int level_counts[4] = {0};
    const char* levels[] = {LEVEL_ERROR, LEVEL_WARN, LEVEL_INFO, LEVEL_DEBUG};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    if (count > 0) {
        printf("\nFirst entry: %s [%s] %s\n", 
               entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry: %s [%s] %s\n", 
               entries[count-1].timestamp, entries[count-1].level, entries[count-1].message);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (empty line to finish):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            printf("Added: %s [%