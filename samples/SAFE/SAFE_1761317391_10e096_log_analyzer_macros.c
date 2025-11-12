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
    
    if (sscanf(line, "%19s %9s", entry->timestamp, entry->level) != 2) return 0;
    
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    const char* msg_start = strchr(line, ' ');
    if (msg_start == NULL) return 0;
    msg_start = strchr(msg_start + 1, ' ');
    if (msg_start == NULL) return 0;
    msg_start++;
    
    size_t msg_len = strlen(msg_start);
    if (msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

#define COUNT_LEVEL(l, arr, cnt) do { \
    if (strcmp((arr)[i].level, l) == 0) (cnt)++; \
} while(0)

void analyze_logs(LogEntry logs[], size_t count) {
    size_t info_count = 0, warn_count = 0, error_count = 0;
    
    for (size_t i = 0; i < count; i++) {
        COUNT_LEVEL("INFO", logs, info_count);
        COUNT_LEVEL("WARN", logs, warn_count);
        COUNT_LEVEL("ERROR", logs, error_count);
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %zu\n", info_count);
    printf("WARN entries: %zu\n", warn_count);
    printf("ERROR entries: %zu\n", error_count);
    printf("Total entries: %zu\n", count);
    
    if (count > 0) {
        printf("Error rate: %.2f%%\n", (double)error_count / count * 100.0);
    }
}

#define CHECK_ALLOC(ptr) if ((ptr) == NULL) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }

int main(void) {
    LogEntry* logs = malloc(MAX_ENTRIES * sizeof(LogEntry));
    CHECK_ALLOC(logs);
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[MAX_LINE_LEN];
    size_t entry_count = 0;
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_entry(line, &logs[entry_count])) {
            entry_count++;
        } else {
            fprintf(stderr, "Invalid log entry format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(logs, entry_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    free(logs);
    return 0;
}