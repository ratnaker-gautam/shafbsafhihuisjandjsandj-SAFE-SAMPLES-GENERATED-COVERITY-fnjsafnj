//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
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
    
    int mid = start + (end - start) / 2;
    if (mid < start || mid > end) return;
    
    printf("%*s[%d] %s: %s\n", depth * 2, "", 
           entries[mid].timestamp, entries[mid].level, entries[mid].message);
    
    if (start <= mid - 1) {
        analyze_log_recursive(entries, start, mid - 1, depth + 1);
    }
    
    if (mid + 1 <= end) {
        analyze_log_recursive(entries, mid + 1, end, depth + 1);
    }
}

int count_logs_by_level(struct LogEntry* entries, int count, const char* level, int index) {
    if (!entries || !level || index >= count || index < 0) return 0;
    
    int current = 0;
    if (strcmp(entries[index].level, level) == 0) {
        current = 1;
    }
    
    if (index + 1 >= count) return current;
    
    return current + count_logs_by_level(entries, count, level, index + 1);
}

int main(void) {
    struct LogEntry logs[100];
    int log_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    while (log_count < 100 && fgets(buffer, sizeof(buffer), stdin)) {
        if (buffer[0] == '\n' || buffer[0] == '\0') break;
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strlen(buffer) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(buffer, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", buffer);
        }
    }
    
    if (log_count == 0) {
        printf("No valid logs to analyze.\n");
        return 1;
    }
    
    printf("\nStructured log analysis:\n");
    analyze_log_recursive(logs, 0, log_count - 1, 0);
    
    printf("\nLog level statistics:\n");
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_logs_by_level(logs, log_count, levels[i], 0);
        printf("%s: %d\n", levels[i], count);
    }
    
    return 0;
}