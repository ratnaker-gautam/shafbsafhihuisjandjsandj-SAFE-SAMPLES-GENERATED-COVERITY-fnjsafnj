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
    if (line == NULL || entry == NULL) return 0;
    
    const char *ptr = line;
    int field = 0;
    char *dest = NULL;
    size_t len = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        switch (field) {
            case 0: dest = entry->timestamp; len = sizeof(entry->timestamp) - 1; break;
            case 1: dest = entry->level; len = sizeof(entry->level) - 1; break;
            case 2: dest = entry->message; len = sizeof(entry->message) - 1; break;
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

void analyze_log_levels(struct LogEntry *entries, int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO: %d entries\n", info_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("ERROR: %d entries\n", error_count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Type 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES && fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'E' && buffer[1] == 'N' && buffer[2] == 'D' && 
            (buffer[3] == '\n' || buffer[3] == '\0')) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len > 1) {
            if (parse_log_line(buffer, entries + entry_count)) {
                entry_count++;
            } else {
                printf("Invalid log format. Expected: TIMESTAMP LEVEL MESSAGE\n");
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries...\n\n", entry_count);
    analyze_log_levels(entries, entry_count);
    
    printf("\nRecent log entries:\n");
    int display_count = (entry_count > 5) ? 5 : entry_count;
    for (int i = entry_count - display_count; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}