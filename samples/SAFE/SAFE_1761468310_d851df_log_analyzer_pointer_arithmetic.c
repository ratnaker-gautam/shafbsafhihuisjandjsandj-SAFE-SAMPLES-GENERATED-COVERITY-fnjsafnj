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
        
        if (ptr > start) {
            size_t len = ptr - start;
            if (field == 0) {
                if (len >= sizeof(entry->timestamp)) return 0;
                memcpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
            } else if (field == 1) {
                if (len >= sizeof(entry->level)) return 0;
                memcpy(entry->level, start, len);
                entry->level[len] = '\0';
            } else if (field == 2) {
                if (len >= sizeof(entry->message)) return 0;
                memcpy(entry->message, start, len);
                entry->message[len] = '\0';
            }
        }
        field++;
        if (*ptr) ptr++;
    }
    
    return field >= 3;
}

void analyze_log_levels(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, other_count = 0;
    
    for (int i = 0; i < count; i++) {
        char *level = entries[i].level;
        if (strcmp(level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(level, "ERROR") == 0) {
            error_count++;
        } else {
            other_count++;
        }
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
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (len >= sizeof(buffer) - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        struct LogEntry *current = entries + entry_count;
        if (parse_log_line(buffer, current)) {
            entry_count++;
        } else {
            printf("Invalid log format, skipping: %s\n", buffer);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (int i = 0; i < entry_count; i++) {
        struct LogEntry *entry = entries + i;
        printf("%d: [%s] %s - %s\n", i + 1, entry->timestamp, entry->level, entry->message);
    }
    
    printf("\n");
    analyze_log_levels(entries, entry_count);
    
    return 0;
}