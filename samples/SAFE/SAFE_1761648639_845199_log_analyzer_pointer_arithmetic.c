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
        while (*ptr && !isspace(*ptr)) ptr++;
        
        size_t len = ptr - start;
        if (len == 0) break;
        
        switch (field) {
            case 0:
                if (len >= sizeof(entry->timestamp)) return 0;
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
                break;
            case 1:
                if (len >= sizeof(entry->level)) return 0;
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
                break;
            case 2:
                if (len >= sizeof(entry->message)) return 0;
                strncpy(entry->message, start, len);
                entry->message[len] = '\0';
                break;
        }
        
        field++;
    }
    
    return field == 3;
}

void count_log_levels(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, other_count = 0;
    
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + count;
    
    while (current < end) {
        char *level = current->level;
        
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else other_count++;
        
        current++;
    }
    
    printf("Log Level Summary:\n");
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

void search_logs(struct LogEntry *entries, int count, const char *keyword) {
    if (entries == NULL || count <= 0 || keyword == NULL) return;
    
    printf("Search results for '%s':\n", keyword);
    
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + count;
    int found = 0;
    
    while (current < end) {
        char *msg = current->message;
        char *pos = msg;
        
        while (*pos) {
            if (strncasecmp(pos, keyword, strlen(keyword)) == 0) {
                printf("%s %s %s\n", current->timestamp, current->level, current->message);
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

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries...\n\n", entry_count);
    count_log_levels(entries, entry_count);
    
    printf("\nEnter search keyword (or 'QUIT' to exit): ");
    fflush(stdout);
    
    char keyword[50];
    while (fgets(keyword, sizeof(keyword), stdin) != NULL) {
        size_t len = strlen(keyword);
        if (len > 0 && keyword[len - 1] == '\n') {
            keyword[len - 1] = '\0';
        }
        
        if (strcmp(keyword, "QUIT") == 0) break;
        
        if (strlen(keyword) > 0) {
            search_logs(entries, entry_count, keyword);
        }
        
        printf("\nEnter search keyword (or 'QUIT' to exit): ");
        fflush(stdout);
    }
    
    return 0;
}