//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARNING") == 0 || strcmp(l, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= TIMESTAMP_LEN - 1) return 0;
    if (strlen(level) >= LEVEL_LEN - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    if (!IS_VALID_LEVEL(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    strncpy(entry->level, level, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(entry_ptr, l, info_cnt, warn_cnt, err_cnt) do { \
    if (strcmp((entry_ptr)->level, "INFO") == 0) { \
        (info_cnt)++; \
    } else if (strcmp((entry_ptr)->level, "WARNING") == 0) { \
        (warn_cnt)++; \
    } else if (strcmp((entry_ptr)->level, "ERROR") == 0) { \
        (err_cnt)++; \
    } \
} while(0)

void analyze_logs(LogEntry entries[], int count) {
    int levelINFO_count = 0;
    int levelWARNING_count = 0;
    int levelERROR_count = 0;
    
    for (int i = 0; i < count; i++) {
        COUNT_LEVEL(&entries[i], entries[i].level, levelINFO_count, levelWARNING_count, levelERROR_count);
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", levelINFO_count);
    printf("WARNING: %d\n", levelWARNING_count);
    printf("ERROR: %d\n", levelERROR_count);
    
    printf("\nRecent entries:\n");
    int start = count > 5 ? count - 5 : 0;
    for (int i = start; i < count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

#define READ_LINE(f, b) (fgets(b, MAX_LINE_LEN, f) != NULL)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    
    char buffer[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES && READ_LINE(stdin, buffer)) {
        if (buffer[0] == '\n' || buffer[0] == '\0') break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) continue;
        
        LogEntry entry;
        if (parse_log_entry(buffer, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", buffer);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}