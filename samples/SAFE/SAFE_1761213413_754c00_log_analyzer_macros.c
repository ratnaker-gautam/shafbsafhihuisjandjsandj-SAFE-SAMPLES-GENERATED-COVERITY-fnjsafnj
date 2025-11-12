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
    
    char level_buf[LEVEL_LEN];
    char timestamp_buf[TIMESTAMP_LEN];
    int scanned = sscanf(line, "%19s %9s", timestamp_buf, level_buf);
    if (scanned != 2) return 0;
    
    if (strlen(timestamp_buf) != 19 || timestamp_buf[10] != 'T') return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 10) continue;
        if (!isdigit(timestamp_buf[i]) && timestamp_buf[i] != '-' && timestamp_buf[i] != ':') return 0;
    }
    
    if (!IS_VALID_LEVEL(level_buf)) return 0;
    
    strncpy(entry->timestamp, timestamp_buf, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    strncpy(entry->level, level_buf, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';
    
    const char* message_start = strchr(line, ' ');
    if (message_start) {
        message_start = strchr(message_start + 1, ' ');
        if (message_start) {
            message_start++;
            strncpy(entry->message, message_start, sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';
            char* newline = strchr(entry->message, '\n');
            if (newline) *newline = '\0';
        } else {
            entry->message[0] = '\0';
        }
    } else {
        entry->message[0] = '\0';
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DDTHH:MM:SS LEVEL message):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count++] = entry;
        } else {
            printf("Invalid log entry format. Expected: YYYY-MM-DDTHH:MM:SS LEVEL message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (error_count > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            }
        }
    }
    
    return 0;
}