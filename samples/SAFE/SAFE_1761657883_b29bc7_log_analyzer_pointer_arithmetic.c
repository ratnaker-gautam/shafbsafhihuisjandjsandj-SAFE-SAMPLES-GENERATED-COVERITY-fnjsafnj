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
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;
        
        size_t len = ptr - start;
        if (len == 0) break;
        
        switch (field) {
            case 0:
                if (len >= sizeof(entry->timestamp)) return 0;
                memcpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
                break;
            case 1:
                if (len >= sizeof(entry->level)) return 0;
                memcpy(entry->level, start, len);
                entry->level[len] = '\0';
                break;
            case 2:
                if (len >= sizeof(entry->message)) return 0;
                memcpy(entry->message, start, len);
                entry->message[len] = '\0';
                break;
        }
        
        field++;
        if (*ptr) ptr++;
    }
    
    return field == 3;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strcmp(buffer, "END") == 0) break;
        
        if (len >= sizeof(buffer) - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        struct LogEntry *current = entries + entry_count;
        if (parse_log_line(buffer, current)) {
            entry_count++;
        } else {
            printf("Invalid log format, skipping.\n");
        }
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    printf("====================\n");
    
    struct LogEntry *ptr = entries;
    struct LogEntry *end = entries + entry_count;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    while (ptr < end) {
        printf("Time: %s | Level: %s | Message: %s\n", 
               ptr->timestamp, ptr->level, ptr->message);
        
        if (strcmp(ptr->level, "ERROR") == 0) error_count++;
        else if (strcmp(ptr->level, "WARN") == 0) warning_count++;
        else if (strcmp(ptr->level, "INFO") == 0) info_count++;
        
        ptr++;
    }
    
    printf("\nSummary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warning_count);
    printf("INFO:  %d\n", info_count);
    
    return 0;
}