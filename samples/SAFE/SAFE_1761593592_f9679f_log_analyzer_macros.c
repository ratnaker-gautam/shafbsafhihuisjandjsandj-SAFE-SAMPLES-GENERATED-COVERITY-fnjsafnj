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
    
    const char* msg_start = line + TIMESTAMP_LEN + 1;
    msg_start += strlen(entry->level) + 1;
    
    if (msg_start >= line + len) return 0;
    
    size_t msg_len = len - (msg_start - line);
    if (msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    char* newline = strchr(entry->message, '\n');
    if (newline) *newline = '\0';
    
    return 1;
}

#define COUNT_LEVEL(l, count_var) do { \
    if (strcmp(entry.level, l) == 0) { \
        if (count_var < MAX_ENTRIES) count_var++; \
    } \
} while(0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int info_count = 0, warn_count = 0, error_count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        
        if (strlen(line) == 0) break;
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            COUNT_LEVEL("INFO", info_count);
            COUNT_LEVEL("WARN", warn_count);
            COUNT_LEVEL("ERROR", error_count);
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (entry_count > 0) {
        printf("\nRecent entries:\n");
        int display_count = (entry_count < 5) ? entry_count : 5;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}