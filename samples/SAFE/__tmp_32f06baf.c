//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')
#define LEVEL_TO_INDEX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 3];
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
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entries(LogEntry* entries, int max_entries) {
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        LogEntry entry;
        if (!parse_timestamp(line, entry.timestamp)) {
            continue;
        }
        
        if (strlen(line) < TIMESTAMP_LEN + 3) continue;
        if (line[TIMESTAMP_LEN] != ' ' || line[TIMESTAMP_LEN + 1] != '[') continue;
        
        char level_char = line[TIMESTAMP_LEN + 2];
        if (!IS_VALID_LEVEL(level_char)) continue;
        entry.level = level_char;
        
        if (strlen(line) < TIMESTAMP_LEN + 5) continue;
        if (line[TIMESTAMP_LEN + 3] != ']' || line[TIMESTAMP_LEN + 4] != ' ') continue;
        
        const char* msg_start = line + TIMESTAMP_LEN + 5;
        size_t msg_len = strlen(msg_start);
        if (msg_len == 0) continue;
        
        if (msg_len >= sizeof(entry.message)) {
            msg_len = sizeof(entry.message) - 1;
        }
        strncpy(entry.message, msg_start, msg_len);
        entry.message[msg_len] = '\0';
        
        char* newline = strchr(entry.message, '\n');
        if (newline) *newline = '\0';
        
        entries[count] = entry;
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    char level_names[LOG_LEVELS][8] = {"ERROR", "WARN", "INFO", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        int level_idx = LEVEL_TO_INDEX(entries[i].level);
        if (level_idx >= 0 && level_idx < LOG_LEVELS) {
            level_counts[level_idx]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", count);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-6s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    if (count > 0) {
        printf("\nFirst 5 entries:\n");
        int display_count = count < 5 ? count : 5;
        for (int i = 0; i < display_count; i++) {
            printf("%s [%c] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS [L] message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}