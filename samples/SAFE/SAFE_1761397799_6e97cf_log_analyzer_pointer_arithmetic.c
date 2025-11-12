//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
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
        
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        field++;
    }
    
    return field == 3;
}

void count_log_levels(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, other_count = 0;
    struct LogEntry *current = entries;
    
    for (int i = 0; i < count; i++) {
        char *level = current->level;
        
        if (strcmp(level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(level, "ERROR") == 0) {
            error_count++;
        } else {
            other_count++;
        }
        
        current++;
    }
    
    printf("Log Level Summary:\n");
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

void search_logs(struct LogEntry *entries, int count, const char *search_term) {
    if (entries == NULL || count <= 0 || search_term == NULL) return;
    
    printf("Search results for '%s':\n", search_term);
    struct LogEntry *current = entries;
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        char *message = current->message;
        char *pos = message;
        
        while (*pos) {
            char *match_start = pos;
            const char *search_ptr = search_term;
            
            while (*pos && *search_ptr && *pos == *search_ptr) {
                pos++;
                search_ptr++;
            }
            
            if (*search_ptr == '\0') {
                printf("[%s] %s: %s\n", current->timestamp, current->level, current->message);
                found++;
                break;
            }
            
            if (*match_start) pos = match_start + 1;
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
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries...\n\n", entry_count);
    count_log_levels(entries, entry_count);
    
    printf("\nEnter search term (or 'QUIT' to exit): ");
    if (fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "QUIT") != 0) {
            search_logs(entries, entry_count, line);
        }
    }
    
    return 0;
}