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
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN];
} LogEntry;

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || strcmp(level, "ERROR") == 0)

#define IS_VALID_TIMESTAMP(ts) (strlen(ts) == 19 && ts[4] == '-' && ts[7] == '-' && ts[10] == ' ' && ts[13] == ':' && ts[16] == ':')

int parse_log_entry(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strcpy(temp_line, line);
    
    if (sscanf(temp_line, "%19s %9s", entry->timestamp, entry->level) != 2) return 0;
    
    if (!IS_VALID_TIMESTAMP(entry->timestamp)) return 0;
    if (!VALID_LEVEL(entry->level)) return 0;
    
    const char* message_start = strchr(strchr(temp_line, ' ') + 1, ' ') + 1;
    if (!message_start || strlen(message_start) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->message, message_start);
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int info_count = 0, warn_count = 0, error_count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, "INFO") == 0) info_count++;
            else if (strcmp(entry.level, "WARN") == 0) warn_count++;
            else if (strcmp(entry.level, "ERROR") == 0) error_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (entry_count > 0) {
        printf("\nRecent entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}