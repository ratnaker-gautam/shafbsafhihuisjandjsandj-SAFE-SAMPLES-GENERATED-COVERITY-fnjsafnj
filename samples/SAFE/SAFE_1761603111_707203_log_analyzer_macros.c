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

#define IS_VALID_LEVEL(level) \
    (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || \
     strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0)

#define PARSE_TIMESTAMP(str, tm) \
    (sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d", \
            &(tm)->tm_year, &(tm)->tm_mon, &(tm)->tm_mday, \
            &(tm)->tm_hour, &(tm)->tm_min, &(tm)->tm_sec) == 6)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[8];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 12];
} LogEntry;

int validate_timestamp(const char* timestamp) {
    struct tm tm = {0};
    if (!PARSE_TIMESTAMP(timestamp, &tm)) return 0;
    if (tm.tm_year < 2000 || tm.tm_year > 2100) return 0;
    if (tm.tm_mon < 1 || tm.tm_mon > 12) return 0;
    if (tm.tm_mday < 1 || tm.tm_mday > 31) return 0;
    if (tm.tm_hour < 0 || tm.tm_hour > 23) return 0;
    if (tm.tm_min < 0 || tm.tm_min > 59) return 0;
    if (tm.tm_sec < 0 || tm.tm_sec > 59) return 0;
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) continue;
        
        char timestamp[TIMESTAMP_LEN] = {0};
        char level[8] = {0};
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 12] = {0};
        
        int parsed = sscanf(line, "%19s %7s %[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!validate_timestamp(timestamp)) continue;
        if (!IS_VALID_LEVEL(level)) continue;
        if (strlen(message) == 0) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        strncpy(entries[count].level, level, 7);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level[7] = '\0';
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
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
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    if (count > 0) {
        printf("\nFirst entry: %s %s %s\n", 
               entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry: %s %s %s\n", 
               entries[count-1].timestamp, entries[count-1].level, entries[count-1].message);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}