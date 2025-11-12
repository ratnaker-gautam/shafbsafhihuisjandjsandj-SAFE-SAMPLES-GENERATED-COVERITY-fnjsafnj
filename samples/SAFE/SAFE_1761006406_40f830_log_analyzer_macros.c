//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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
#define IS_EMPTY_LINE(s) ((s)[0] == '\0' || ((s)[0] == '\n' && (s)[1] == '\0'))

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_log_level(const char *str) {
    if (str == NULL) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcasecmp(str, level_names[i]) == 0) return i;
    }
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        char c = str[i];
        if (i == 4 || i == 7) {
            if (c != '-') return 0;
        } else if (i == 10) {
            if (c != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (c != ':') return 0;
        } else {
            if (!isdigit(c)) return 0;
        }
        timestamp[i] = c;
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (IS_EMPTY_LINE(line)) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        if (parse_timestamp(line, timestamp)) {
            const char *rest = line + TIMESTAMP_LEN;
            if (sscanf(rest, " %15s", level_str) == 1) {
                int level = parse_log_level(level_str);
                if (IS_VALID_LEVEL(level)) {
                    const char *msg_start = rest + strlen(level_str);
                    while (*msg_start == ' ') msg_start++;
                    
                    strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
                    entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                    entries[count].level = level;
                    strncpy(entries[count].message, msg_start, sizeof(entries[count].message) - 1);
                    entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                    
                    char *newline = strchr(entries[count].message, '\n');
                    if (newline) *newline = '\0';
                    
                    count++;
                }
            }
        }
    }
    
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
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
            printf("%s %s %s\n", entries[i].timestamp, LEVEL_NAME(entries[i].level), 
                   entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}