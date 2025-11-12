//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
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
        
        if (ptr > start) {
            size_t len = ptr - start;
            if (field == 0 && len < sizeof(entry->timestamp)) {
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
            } else if (field == 1 && len < sizeof(entry->level)) {
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
            } else if (field == 2) {
                char *msg_start = start;
                while (*ptr && *ptr != '\n') ptr++;
                size_t msg_len = ptr - msg_start;
                if (msg_len < sizeof(entry->message)) {
                    strncpy(entry->message, msg_start, msg_len);
                    entry->message[msg_len] = '\0';
                    break;
                }
            }
        }
        field++;
        if (*ptr) ptr++;
    }
    
    return (field >= 2);
}

void analyze_log_levels(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, other_count = 0;
    
    for (int i = 0; i < count; i++) {
        char *level = entries[i].level;
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else other_count++;
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        char *newline = buffer;
        while (*newline && *newline != '\n') newline++;
        if (*newline == '\n') *newline = '\0';
        
        if (strcmp(buffer, "END") == 0) break;
        
        if (strlen(buffer) == 0) continue;
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (int i = 0; i < entry_count; i++) {
        printf("%d: [%s] %s - %s\n", i + 1, 
               entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    printf("\n");
    analyze_log_levels(entries, entry_count);
    
    return 0;
}