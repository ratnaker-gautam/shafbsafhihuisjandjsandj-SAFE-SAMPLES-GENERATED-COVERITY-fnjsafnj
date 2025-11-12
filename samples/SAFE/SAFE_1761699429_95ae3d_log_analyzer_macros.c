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

#define IS_VALID_LEVEL(level) ((level) >= 0 && (level) < LOG_LEVELS)
#define IS_VALID_TIMESTAMP(ts) (strlen(ts) == TIMESTAMP_LEN - 1)
#define SAFE_STRCPY(dest, src, max) do { \
    strncpy((dest), (src), (max) - 1); \
    (dest)[(max) - 1] = '\0'; \
} while(0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN];
} LogEntry;

const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char* level_str) {
    if (!level_str) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(level_str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (!timestamp || !IS_VALID_TIMESTAMP(timestamp)) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19s %15s %1023[^\n]", timestamp, level_str, message) != 3) {
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }
        
        SAFE_STRCPY(entries[count].timestamp, timestamp, TIMESTAMP_LEN);
        entries[count].level = level;
        SAFE_STRCPY(entries[count].message, message, MAX_LINE_LEN);
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    SAFE_STRCPY(earliest, entries[0].timestamp, TIMESTAMP_LEN);
    SAFE_STRCPY(latest, entries[0].timestamp, TIMESTAMP_LEN);
    
    for (int i = 0; i < count; i++) {
        level_counts[entries[i].level]++;
        
        if (strcmp(entries[i].timestamp, earliest) < 0) {
            SAFE_STRCPY(earliest, entries[i].timestamp, TIMESTAMP_LEN);
        }
        if (strcmp(entries[i].timestamp, latest) > 0) {
            SAFE_STRCPY(latest, entries[i].timestamp, TIMESTAMP_LEN);
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Breakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("  %s: %d (%.1f%%)\n", level_names[i], level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Press Ctrl+D to finish input.\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count > 0) {
        analyze_logs(entries, count);
    } else {
        printf("No valid log entries processed.\n");
    }
    
    return 0;
}