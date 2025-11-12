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

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARN") == 0 || strcmp(l, "ERROR") == 0)

int parse_log_entry(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len >= MAX_LINE_LEN || len < TIMESTAMP_LEN + LEVEL_LEN + 3) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= sizeof(temp)) return 0;
    strncpy(temp, line, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    if (temp[TIMESTAMP_LEN - 1] != ' ' || temp[TIMESTAMP_LEN + LEVEL_LEN] != ' ') return 0;
    
    strncpy(entry->timestamp, temp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    strncpy(entry->level, temp + TIMESTAMP_LEN, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';
    
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    const char* msg_start = temp + TIMESTAMP_LEN + LEVEL_LEN + 1;
    if (strlen(msg_start) >= sizeof(entry->message)) return 0;
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(l, arr, cnt) do { \
    if (strcmp((arr)[i].level, l) == 0) (cnt)++; \
} while(0)

void analyze_logs(LogEntry logs[], int count) {
    if (logs == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        COUNT_LEVEL("INFO", logs, info_count);
        COUNT_LEVEL("WARN", logs, warn_count);
        COUNT_LEVEL("ERROR", logs, error_count);
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int start = count > 5 ? count - 5 : 0;
        for (int i = start; i < count; i++) {
            printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
        }
    }
}

#define SAFE_STRCPY(dest, src, max) do { \
    strncpy((dest), (src), (max) - 1); \
    (dest)[(max) - 1] = '\0'; \
} while(0)

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            SAFE_STRCPY(logs[entry_count].timestamp, entry.timestamp, TIMESTAMP_LEN);
            SAFE_STRCPY(logs[entry_count].level, entry.level, LEVEL_LEN);
            SAFE_STRCPY(logs[entry_count].message, entry.message, sizeof(entry.message));
            entry_count++;
        } else {
            printf("Invalid log entry format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(logs, entry_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}