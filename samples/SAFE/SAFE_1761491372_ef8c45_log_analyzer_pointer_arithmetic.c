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
        char *start = ptr;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') {
            ptr++;
        }
        
        size_t len = ptr - start;
        if (len == 0) {
            ptr++;
            continue;
        }
        
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

void analyze_log_levels(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0;
    struct LogEntry *current = entries;
    
    for (int i = 0; i < count; i++) {
        char *level = current->level;
        
        if (strcmp(level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(level, "ERROR") == 0) {
            error_count++;
        }
        
        current++;
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO:  %d entries\n", info_count);
    printf("WARN:  %d entries\n", warn_count);
    printf("ERROR: %d entries\n", error_count);
}

void search_in_messages(struct LogEntry *entries, int count, const char *term) {
    if (entries == NULL || count <= 0 || term == NULL) return;
    
    printf("Search results for '%s':\n", term);
    struct LogEntry *current = entries;
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        char *msg = current->message;
        char *pos = msg;
        
        while (*pos) {
            if (strncmp(pos, term, strlen(term)) == 0) {
                printf("[%s] %s: %s\n", current->timestamp, current->level, current->message);
                found++;
                break;
            }
            pos++;
        }
        
        current++;
    }
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char buffer[MAX_LINE_LEN];
    
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
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    analyze_log_levels(entries, entry_count);
    
    printf("\nEnter search term (or 'QUIT' to exit): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(buffer, "QUIT") != 0 && strlen(buffer) > 0) {
            search_in_messages(entries, entry_count, buffer);
        }
    }
    
    return 0;
}