//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    if (strcmp(str, "ERROR") == 0) return LOG_LEVEL_ERROR;
    if (strcmp(str, "WARN") == 0) return LOG_LEVEL_WARN;
    if (strcmp(str, "INFO") == 0) return LOG_LEVEL_INFO;
    if (strcmp(str, "DEBUG") == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int validate_timestamp(const char* ts) {
    if (strlen(ts) != 19) return 0;
    if (ts[4] != '-' || ts[7] != '-' || ts[10] != ' ' || ts[13] != ':' || ts[16] != ':') return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(ts[i])) return 0;
    }
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        if (sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message) != 3) {
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "9999-99-99 99:99:99";
    char latest[TIMESTAMP_LEN] = "0000-00-00 00:00:00";
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
        
        if (strcmp(entries[i].timestamp, earliest) < 0) {
            strncpy(earliest, entries[i].timestamp, TIMESTAMP_LEN - 1);
            earliest[TIMESTAMP_LEN - 1] = '\0';
        }
        
        if (strcmp(entries[i].timestamp, latest) > 0) {
            strncpy(latest, entries[i].timestamp, TIMESTAMP_LEN - 1);
            latest[TIMESTAMP_LEN - 1] = '\0';
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Level distribution:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("  %s: %d\n", level_names[i], level_counts[i]);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}