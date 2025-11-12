//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH - 48];
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
        while (*ptr && !isspace(*ptr)) ptr++;
        size_t field_len = ptr - start;
        
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
        
        if (field_len >= len) field_len = len - 1;
        memcpy(dest, start, field_len);
        *(dest + field_len) = '\0';
        
        field++;
    }
    
    return field == 3;
}

void analyze_logs(struct LogEntry *entries, int count) {
    if (!entries || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + count;
    
    while (current < end) {
        char *level = current->level;
        
        if (strcmp(level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(level, "INFO") == 0) {
            info_count++;
        }
        
        current++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (count > 0) {
        printf("\nFirst entry timestamp: %s\n", entries->timestamp);
        printf("Last entry timestamp: %s\n", (entries + count - 1)->timestamp);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry *entry = entries + entry_count;
        if (parse_log_line(line, entry)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}