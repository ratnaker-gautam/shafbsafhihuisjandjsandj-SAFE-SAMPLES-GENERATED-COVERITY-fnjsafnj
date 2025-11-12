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

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    const char *ptr = line;
    int field = 0;
    char *dest = NULL;
    size_t len = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        const char *start = ptr;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;
        
        size_t field_len = ptr - start;
        if (field_len == 0) break;
        
        switch (field) {
            case 0:
                dest = entry->timestamp;
                len = sizeof(entry->timestamp) - 1;
                break;
            case 1:
                dest = entry->level;
                len = sizeof(entry->level) - 1;
                break;
            case 2:
                dest = entry->message;
                len = sizeof(entry->message) - 1;
                break;
        }
        
        if (field_len > len) field_len = len;
        memcpy(dest, start, field_len);
        dest[field_len] = '\0';
        
        field++;
    }
    
    return field == 3;
}

void analyze_log_levels(struct LogEntry *entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        char *level = entries[i].level;
        if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "INFO") == 0) info_count++;
    }
    
    printf("Log Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        
        char *newline = buffer;
        while (*newline && *newline != '\n') newline++;
        if (newline == buffer) break;
        *newline = '\0';
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: timestamp level message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries...\n\n", entry_count);
    analyze_log_levels(entries, entry_count);
    
    printf("\nFirst 5 entries:\n");
    for (int i = 0; i < 5 && i < entry_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}