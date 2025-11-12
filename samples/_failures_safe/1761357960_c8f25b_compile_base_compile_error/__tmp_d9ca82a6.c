//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ')

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_log_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    int count = 0;
    for (int i = 0; str[i] && count < TIMESTAMP_LEN; i++) {
        if (IS_TIMESTAMP_CHAR(str[i])) {
            timestamp[count++] = str[i];
        } else {
            break;
        }
    }
    if (count > 0) {
        timestamp[count] = '\0';
        return 1;
    }
    return 0;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN + 1] = {0};
        int level = -1;
        char message[MAX_LINE_LEN] = {0};
        
        if (!parse_timestamp(line, timestamp)) continue;
        
        const char *level_start = line + strlen(timestamp);
        while (*level_start && isspace(*level_start)) level_start++;
        
        const char *level_end = level_start;
        while (*level_end && !isspace(*level_end)) level_end++;
        
        if (level_end > level_start) {
            char level_str[16] = {0};
            size_t len = level_end - level_start;
            if (len >= sizeof(level_str)) len = sizeof(level_str) - 1;
            strncpy(level_str, level_start, len);
            level_str[len] = '\0';
            level = parse_log_level(level_str);
        }
        
        if (!IS_VALID_LEVEL(level)) continue;
        
        const char *msg_start = level_end;
        while (*msg_start && isspace(*msg_start)) msg_start++;
        
        if (*msg_start) {
            size_t msg_len = strlen(msg_start);
            if (msg_len > 0 && msg_start[msg_len - 1] == '\n') {
                msg_len--;
            }
            if (msg_len >= sizeof(entries[count].message)) {
                msg_len = sizeof(entries[count].message) - 1;
            }
            strncpy(entries[count].message, msg_start, msg_len);
            entries[count].message[msg_len] = '\0';
        }
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN);
        entries[count].timestamp[TIMESTAMP_LEN] = '\0';
        entries[count].level = level;
        count++;
    }
    
    return count;
}

void analyze_logs(const LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("[%s] %s: %s\n", entries[i].timestamp, 
                   LEVEL_NAME(entries[i].level), entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    analyze