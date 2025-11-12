//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 16

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN];
} LogEntry;

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARNING") == 0 || strcmp(l, "ERROR") == 0)

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char ts_buf[TIMESTAMP_LEN];
    char lvl_buf[LEVEL_LEN];
    int scanned = sscanf(line, "%19s %15s", ts_buf, lvl_buf);
    if (scanned != 2) return 0;
    
    if (!IS_VALID_LEVEL(lvl_buf)) return 0;
    
    const char* msg_start = line + strlen(ts_buf) + strlen(lvl_buf) + 2;
    if (msg_start >= line + strlen(line)) return 0;
    
    strncpy(entry->timestamp, ts_buf, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    strncpy(entry->level, lvl_buf, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(l) do { \
    if (strcmp(entries[i].level, #l) == 0) { \
        level##l##_count++; \
    } \
} while(0)

void analyze_logs(LogEntry entries[], int count) {
    int levelINFO_count = 0;
    int levelWARNING_count = 0;
    int levelERROR_count = 0;
    
    for (int i = 0; i < count; i++) {
        COUNT_LEVEL(INFO);
        COUNT_LEVEL(WARNING);
        COUNT_LEVEL(ERROR);
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", levelINFO_count);
    printf("WARNING entries: %d\n", levelWARNING_count);
    printf("ERROR entries: %d\n", levelERROR_count);
    printf("Total entries: %d\n", count);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
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
            printf("Added: %s %s %s\n", entry.timestamp, entry.level, entry.message);
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}