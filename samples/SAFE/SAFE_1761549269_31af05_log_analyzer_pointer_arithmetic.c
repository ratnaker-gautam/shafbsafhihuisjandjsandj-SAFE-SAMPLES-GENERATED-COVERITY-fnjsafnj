//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

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
        while (*ptr && !isspace(*ptr)) ptr++;
        
        size_t len = ptr - start;
        if (len == 0) break;
        
        switch (field) {
            case 0:
                if (len >= sizeof(entry->timestamp)) return 0;
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
                break;
            case 1:
                if (len >= sizeof(entry->level)) return 0;
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
                break;
            case 2:
                if (len >= sizeof(entry->message)) return 0;
                strncpy(entry->message, start, len);
                entry->message[len] = '\0';
                break;
        }
        field++;
    }
    
    return field == 3;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (len == 0) continue;
        
        struct LogEntry *current = entries + entry_count;
        if (parse_log_line(line, current)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    printf("TIMESTAMP           LEVEL   MESSAGE\n");
    printf("-----------------------------------\n");
    
    struct LogEntry *ptr = entries;
    int remaining = entry_count;
    while (remaining > 0) {
        printf("%-19s %-7s %s\n", ptr->timestamp, ptr->level, ptr->message);
        ptr++;
        remaining--;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    ptr = entries;
    remaining = entry_count;
    while (remaining > 0) {
        if (strcmp(ptr->level, "ERROR") == 0) error_count++;
        else if (strcmp(ptr->level, "WARN") == 0) warning_count++;
        else if (strcmp(ptr->level, "INFO") == 0) info_count++;
        ptr++;
        remaining--;
    }
    
    printf("\nSummary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warning_count);
    printf("INFO:  %d\n", info_count);
    
    return 0;
}