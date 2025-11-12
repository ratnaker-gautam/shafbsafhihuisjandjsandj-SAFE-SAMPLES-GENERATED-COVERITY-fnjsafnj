//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
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
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Level Analysis:\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (int i = 0; i < entry_count; i++) {
        printf("%d: [%s] %s: %s\n", i + 1, 
               entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    printf("\n");
    analyze_log_levels(entries, entry_count);
    
    return 0;
}