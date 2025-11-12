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
            } else {
                if (len >= sizeof(entry->message)) return 0;
                strncpy(entry->message, start, len);
                entry->message[len] = '\0';
            }
        }
        field++;
        if (*ptr) ptr++;
    }
    
    return field == 3;
}

int count_level_occurrences(struct LogEntry *entries, int count, const char *level) {
    if (entries == NULL || level == NULL || count <= 0) return 0;
    
    int occurrences = 0;
    struct LogEntry *end = entries + count;
    
    for (struct LogEntry *current = entries; current < end; current++) {
        if (strcmp(current->level, level) == 0) {
            occurrences++;
        }
    }
    
    return occurrences;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "END") == 0) break;
        
        if (strlen(buffer) == 0) continue;
        
        struct LogEntry *current = entries + entry_count;
        if (parse_log_line(buffer, current)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries processed.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_level_occurrences(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    printf("\nRecent entries:\n");
    int display_count = (entry_count > 5) ? 5 : entry_count;
    struct LogEntry *start = entries + (entry_count - display_count);
    
    for (struct LogEntry *current = start; current < entries + entry_count; current++) {
        printf("%s [%s] %s\n", current->timestamp, current->level, current->message);
    }
    
    return 0;
}