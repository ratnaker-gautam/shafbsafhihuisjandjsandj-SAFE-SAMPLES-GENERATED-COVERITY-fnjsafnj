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
        
        switch (field) {
            case 0:
                dest = entry->timestamp;
                len = 19;
                break;
            case 1:
                dest = entry->level;
                len = 9;
                break;
            case 2:
                dest = entry->message;
                len = MAX_LINE_LEN - 31;
                break;
        }
        
        size_t i = 0;
        while (*ptr && !isspace(*ptr) && i < len) {
            *dest++ = *ptr++;
            i++;
        }
        *dest = '\0';
        field++;
    }
    
    return field == 3;
}

void count_log_levels(struct LogEntry *entries, int count) {
    if (!entries || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, other_count = 0;
    
    for (int i = 0; i < count; i++) {
        struct LogEntry *entry = entries + i;
        
        if (strcmp(entry->level, "INFO") == 0) info_count++;
        else if (strcmp(entry->level, "WARN") == 0) warn_count++;
        else if (strcmp(entry->level, "ERROR") == 0) error_count++;
        else other_count++;
    }
    
    printf("Log Level Summary:\n");
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (int i = 0; i < entry_count; i++) {
        struct LogEntry *entry = entries + i;
        printf("%s [%s] %s\n", entry->timestamp, entry->level, entry->message);
    }
    
    printf("\n");
    count_log_levels(entries, entry_count);
    
    return 0;
}