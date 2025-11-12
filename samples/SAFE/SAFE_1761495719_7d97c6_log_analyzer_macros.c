//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
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

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARN") == 0 || strcmp(l, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
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

#define COUNT_LEVEL(l, arr, cnt) do { \
    if (strcmp((arr)[i].level, l) == 0) { \
        (cnt)++; \
    } \
} while(0)

void analyze_logs(LogEntry logs[], int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        COUNT_LEVEL("INFO", logs, info_count);
        COUNT_LEVEL("WARN", logs, warn_count);
        COUNT_LEVEL("ERROR", logs, error_count);
    }
    
    printf("Log Analysis Summary:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Total entries: %d\n", count);
}

#define SAFE_FGETS(buf, len, stream) (fgets(buf, len, stream) != NULL)

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (!SAFE_FGETS(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            logs[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(logs, entry_count);
        
        printf("\nRecent ERROR entries:\n");
        int error_shown = 0;
        for (int i = entry_count - 1; i >= 0 && error_shown < 3; i--) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
                error_shown++;
            }
        }
        if (error_shown == 0) {
            printf("No ERROR entries found.\n");
        }
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}