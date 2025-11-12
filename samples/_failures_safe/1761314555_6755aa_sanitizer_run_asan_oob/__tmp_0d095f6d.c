//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARNING") == 0 || strcmp(l, "ERROR") == 0)

int parse_timestamp(const char* ts) {
    struct tm tm = {0};
    return strptime(ts, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            continue;
        }
        
        char timestamp[TIMESTAMP_LEN];
        char level[LEVEL_LEN];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
        
        int parsed = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
        if (parsed == 3 && parse_timestamp(timestamp) && IS_VALID_LEVEL(level)) {
            strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
            entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
            strncpy(entries[count].level, level, LEVEL_LEN - 1);
            entries[count].level[LEVEL_LEN - 1] = '\0';
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            count++;
        }
    }
    
    return count;
}

#define COUNT_LEVEL(e, l, c, n) do { \
    if (strcmp((e).level, l) == 0) { \
        c++; \
    } \
    n++; \
} while(0)

void analyze_logs(LogEntry entries[], int count) {
    int info_count = 0, warning_count = 0, error_count = 0, total = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
        total++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", total);
    printf("INFO: %d (%.1f%%)\n", info_count, total > 0 ? (info_count * 100.0) / total : 0.0);
    printf("WARNING: %d (%.1f%%)\n", warning_count, total > 0 ? (warning_count * 100.0) / total : 0.0);
    printf("ERROR: %d (%.1f%%)\n", error_count, total > 0 ? (error_count * 100.0) / total : 0.0);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (int i = count - 1; i >= 0 && printed < 3; i--) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s: %s\n", entries[i].timestamp, entries[i].message);
                printed++;
            }
        }
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