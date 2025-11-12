//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char level_buf[16];
    char message_buf[MAX_LINE_LENGTH];
    int timestamp;
    
    int parsed = sscanf(line, "%d %15s %1023[^\n]", 
                       &timestamp, level_buf, message_buf);
    
    if (parsed != 3) return 0;
    
    if (timestamp < 0) return 0;
    
    if (strlen(level_buf) >= 16 || strlen(message_buf) >= MAX_LINE_LENGTH) return 0;
    
    strncpy(entry->level, level_buf, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message_buf, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    entry->timestamp = timestamp;
    
    return 1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int depth) {
    if (depth >= MAX_LEVELS) return;
    if (start > end) return;
    if (!entries) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = start; i <= end; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Depth %d: Range %d-%d: ERRORS=%d WARNINGS=%d INFO=%d\n",
           depth, start, end, error_count, warning_count, info_count);
    
    if (end - start > 1) {
        int mid = start + (end - start) / 2;
        analyze_log_recursive(entries, start, mid, depth + 1);
        analyze_log_recursive(entries, mid + 1, end, depth + 1);
    }
}

int read_log_entries(struct LogEntry** entries_ptr) {
    if (!entries_ptr) return 0;
    
    struct LogEntry* entries = malloc(100 * sizeof(struct LogEntry));
    if (!entries) return 0;
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp LEVEL message):\n");
    printf("Type 'END' on a separate line to finish.\n");
    
    while (count < 100 && fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[count] = entry;
            count++;
        } else {
            printf("Invalid format, skipping: %s\n", line);
        }
    }
    
    *entries_ptr = entries;
    return count;
}

int main(void) {
    struct LogEntry* entries = NULL;
    int count = read_log_entries(&entries);
    
    if (count <= 0 || !entries) {
        printf("No valid log entries to analyze.\n");
        if (entries) free(entries);
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    analyze_log_recursive(entries, 0, count - 1, 0);
    
    free(entries);
    return 0;
}