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
        char *pos = msg;
        
        while (*pos) {
            char *match = pos;
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
            
            pos++;
        }
    }
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        if (buffer[0] == '\n') break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: timestamp level message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries.\n\n", entry_count);
    
    count_log_levels(entries, entry_count);
    printf("\n");
    
    printf("Enter search keyword: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if (buffer[0] != '\0') {
            search_logs(entries, entry_count, buffer);
        }
    }
    
    return 0;
}