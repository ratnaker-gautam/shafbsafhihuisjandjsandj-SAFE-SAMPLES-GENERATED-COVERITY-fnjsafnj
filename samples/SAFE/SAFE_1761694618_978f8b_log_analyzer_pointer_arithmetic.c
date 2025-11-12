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
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
            } else if (field == 1) {
                if (len >= sizeof(entry->level)) return 0;
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
            } else if (field == 2) {
                if (len >= sizeof(entry->message)) return 0;
                strncpy(entry->message, start, len);
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
    
    int info_count = 0, warn_count = 0, error_count = 0;
    struct LogEntry *current = entries;
    
    for (int i = 0; i < count; i++) {
        char *level_ptr = current->level;
        
        if (strcmp(level_ptr, "INFO") == 0) {
            info_count++;
        } else if (strcmp(level_ptr, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(level_ptr, "ERROR") == 0) {
            error_count++;
        }
        current++;
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries...\n\n", entry_count);
    analyze_log_levels(entries, entry_count);
    
    printf("\nRecent log entries:\n");
    struct LogEntry *current = entries;
    int display_count = (entry_count > 5) ? 5 : entry_count;
    
    for (int i = 0; i < display_count; i++) {
        printf("%s [%s] %s\n", current->timestamp, current->level, current->message);
        current++;
    }
    
    return 0;
}