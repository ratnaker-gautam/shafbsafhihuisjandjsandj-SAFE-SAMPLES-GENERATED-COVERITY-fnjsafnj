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

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

void print_log_entry(const struct LogEntry* entry, int indent) {
    if (entry == NULL || indent < 0 || indent > MAX_LEVELS) return;
    
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

void analyze_log_recursive(struct LogEntry* logs, int start, int end, int current_level, int max_level) {
    if (logs == NULL || start < 0 || end < 0 || start > end || current_level < 0 || current_level > max_level) {
        return;
    }
    
    if (start > end) return;
    
    struct LogEntry current = logs[start];
    int level = parse_log_level(current.level);
    
    if (level >= current_level) {
        print_log_entry(&current, current_level);
    }
    
    if (start + 1 <= end) {
        struct LogEntry next = logs[start + 1];
        int next_level = parse_log_level(next.level);
        
        if (next_level > level) {
            analyze_log_recursive(logs, start + 1, end, current_level + 1, max_level);
        } else {
            analyze_log_recursive(logs, start + 1, end, current_level, max_level);
        }
    }
}

int read_log_file(struct LogEntry* logs, int max_entries) {
    if (logs == NULL || max_entries <= 0) return 0;
    
    printf("Enter log entries (format: LEVEL TIMESTAMP MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' to finish input\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        printf("Log entry %d: ", count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char level[16];
        int timestamp;
        char message[MAX_LINE_LENGTH];
        
        int result = sscanf(line, "%15s %d %1023[^\n]", level, &timestamp, message);
        
        if (result != 3) {
            printf("Invalid format. Use: LEVEL TIMESTAMP MESSAGE\n");
            continue;
        }
        
        if (parse_log_level(level) == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (timestamp < 0) {
            printf("Timestamp must be non-negative\n");
            continue;
        }
        
        if (strlen(message) >= MAX_LINE_LENGTH) {
            printf("Message too long\n");
            continue;
        }
        
        strncpy(logs[count].level, level, sizeof(logs[count].level) - 1);
        logs[count].level[sizeof(logs[count].level) - 1] = '\0';
        logs[count].timestamp = timestamp;
        strncpy(logs[count].message, message, sizeof(logs[count].message) - 1);
        logs[count].message[sizeof(logs[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry logs[100];
    int log_count = read_log_file(logs, 100);
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nLog Analysis (hierarchical view):\n");
    printf("================================\n");
    analyze_log_recursive(logs, 0, log_count - 1, 0, MAX_LEVELS);
    
    printf("\nSummary:\n");
    printf("Total entries: %d\n", log_count);
    
    int level_counts[5] = {0};
    for (int i = 0; i < log_count; i++) {
        int level = parse_log_level(logs[i].level);
        if (level >= 0 && level < 5) {
            level_counts[level]++;
        }
    }