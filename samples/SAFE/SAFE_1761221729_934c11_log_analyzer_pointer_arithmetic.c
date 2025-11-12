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
        while (*ptr && !isspace(*ptr) && *ptr != ']') ptr++;
        
        size_t len = ptr - start;
        if (len == 0) break;
        
        switch (field) {
            case 0:
                if (len >= sizeof(entry->timestamp)) len = sizeof(entry->timestamp) - 1;
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
                break;
            case 1:
                if (*start == '[') start++;
                if (len >= sizeof(entry->level)) len = sizeof(entry->level) - 1;
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
                break;
            case 2:
                while (*ptr && isspace(*ptr)) ptr++;
                start = ptr;
                while (*ptr && *ptr != '\n') ptr++;
                len = ptr - start;
                if (len >= sizeof(entry->message)) len = sizeof(entry->message) - 1;
                strncpy(entry->message, start, len);
                entry->message[len] = '\0';
                break;
        }
        field++;
    }
    
    return field >= 2;
}

void analyze_logs(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + count;
    
    while (current < end) {
        if (strcmp(current->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(current->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(current->level, "INFO") == 0) {
            info_count++;
        }
        current++;
    }
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries processed: %d\n", count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP [LEVEL] MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
        
        printf("\nLast 5 entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        struct LogEntry *current = entries + start;
        struct LogEntry *end = entries + entry_count;
        
        while (current < end) {
            printf("%s [%s] %s\n", current->timestamp, current->level, current->message);
            current++;
        }
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}