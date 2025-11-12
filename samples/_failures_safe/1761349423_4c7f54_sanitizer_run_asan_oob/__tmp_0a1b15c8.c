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
#define LEVEL_NAME(level) (level_names[(level)])

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
int level_counts[LOG_LEVELS] = {0};
LogEntry entries[MAX_ENTRIES];
int entry_count = 0;

int parse_log_level(const char *str) {
    if (!str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) return i;
    }
    return -1;
}

int validate_timestamp(const char *timestamp) {
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

int parse_log_line(const char *line, LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level_str[16];
    int scanned = sscanf(line, "%19s %15s %1023[^\n]", 
                        entry->timestamp, level_str, entry->message);
    
    if (scanned != 3) return 0;
    if (!validate_timestamp(entry->timestamp)) return 0;
    
    entry->level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    return 1;
}

void analyze_logs() {
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", LEVEL_NAME(i), level_counts[i]);
    }
    
    if (entry_count > 0) {
        printf("\nLast 5 entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, 
                   LEVEL_NAME(entries[i].level), entries[i].message);
        }
    }
}

int main() {
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    printf("Enter log entries (timestamp LEVEL message). Empty line to finish.\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message text\n");
    printf("Valid levels: DEBUG, INFO, WARNING, ERROR\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_count++;
        if (line[0] == '\n') break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        if (strlen(line) == 0) continue;
        
        if (entry_count >= MAX_ENTRIES) {
            printf("Maximum entries reached (%d)\n", MAX_ENTRIES);
            break;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            level_counts[entry.level]++;
            entry_count++;
        } else {
            printf("Invalid log format on line %d: %s\n", line_count, line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs();
    return 0;
}