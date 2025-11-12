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
                if (len >= sizeof(entry->timestamp)) len = sizeof(entry->timestamp) - 1;
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
            } else if (field == 1) {
                if (len >= sizeof(entry->level)) len = sizeof(entry->level) - 1;
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
            } else {
                if (len >= sizeof(entry->message)) len = sizeof(entry->message) - 1;
                strncpy(entry->message, start, len);
                entry->message[len] = '\0';
            }
        }
        field++;
        if (*ptr) ptr++;
    }
    
    return field >= 2;
}

void count_log_levels(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, other_count = 0;
    
    for (int i = 0; i < count; i++) {
        char *level = entries[i].level;
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else other_count++;
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
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        char *msg = entries[i].message;
        char *ptr = msg;
        
        while (*ptr) {
            char *match = ptr;
            const char *k = keyword;
            
            while (*match && *k && *match == *k) {
                match++;
                k++;
            }
            
            if (*k == '\0') {
                printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                found++;
                break;
            }
            
            ptr++;
        }
    }
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    
    char buffer[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        char *ptr = buffer;
        while (*ptr && isspace(*ptr)) ptr++;
        if (*ptr == '\0' || *ptr == '\n') break;
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries.\n\n", entry_count);
    
    count_log_levels(entries, entry_count);
    printf("\n");
    
    char search_term[50];
    printf("Enter search term: ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        char *ptr = search_term;
        while (*ptr && *ptr != '\n') ptr++;
        if (ptr > search_term) *ptr = '\0';
        
        if (strlen(search_term) > 0) {
            search_logs(entries, entry_count, search_term);
        }
    }
    
    return 0;
}