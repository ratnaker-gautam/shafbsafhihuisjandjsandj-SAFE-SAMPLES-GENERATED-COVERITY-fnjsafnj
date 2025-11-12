//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    int field = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        
        char *start = ptr;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;
        
        if (ptr > start) {
            size_t len = ptr - start;
            if (field == 0 && len < sizeof(entry->timestamp)) {
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
            } else if (field == 1 && len < sizeof(entry->level)) {
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
            } else if (field == 2 && len < sizeof(entry->message)) {
                strncpy(entry->message, start, len);
                entry->message[len] = '\0';
            }
        }
        
        field++;
        if (*ptr) ptr++;
    }
    
    return field >= 3;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish.\n\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        struct LogEntry *current = entries + entry_count;
        
        if (parse_log_line(line, current)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n\n", entry_count);
    
    if (entry_count > 0) {
        int error_count = 0;
        int warning_count = 0;
        int info_count = 0;
        
        struct LogEntry *ptr = entries;
        struct LogEntry *end = entries + entry_count;
        
        while (ptr < end) {
            if (strcmp(ptr->level, "ERROR") == 0) {
                error_count++;
            } else if (strcmp(ptr->level, "WARN") == 0) {
                warning_count++;
            } else if (strcmp(ptr->level, "INFO") == 0) {
                info_count++;
            }
            ptr++;
        }
        
        printf("ERROR entries: %d\n", error_count);
        printf("WARN entries: %d\n", warning_count);
        printf("INFO entries: %d\n", info_count);
        
        if (error_count > 0) {
            printf("\nERROR entries:\n");
            ptr = entries;
            while (ptr < end) {
                if (strcmp(ptr->level, "ERROR") == 0) {
                    printf("- %s: %s\n", ptr->timestamp, ptr->message);
                }
                ptr++;
            }
        }
    }
    
    return 0;
}