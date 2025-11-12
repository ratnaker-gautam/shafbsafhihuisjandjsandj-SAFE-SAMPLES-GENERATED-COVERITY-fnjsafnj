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

void print_log_entry(const struct LogEntry* entry) {
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

int count_logs_by_level(struct LogEntry* logs, int start, int end, int target_level) {
    if (start > end) return 0;
    
    int current_count = 0;
    int current_level = parse_log_level(logs[start].level);
    if (current_level == target_level) {
        current_count = 1;
    }
    
    return current_count + count_logs_by_level(logs, start + 1, end, target_level);
}

void filter_logs_by_level(struct LogEntry* logs, int start, int end, int min_level, int* filtered_count) {
    if (start > end) return;
    
    int current_level = parse_log_level(logs[start].level);
    if (current_level >= min_level && current_level != -1) {
        print_log_entry(&logs[start]);
        (*filtered_count)++;
    }
    
    filter_logs_by_level(logs, start + 1, end, min_level, filtered_count);
}

int read_log_file(struct LogEntry* logs, int max_entries) {
    printf("Enter log entries (format: LEVEL TIMESTAMP MESSAGE). Enter 'END' to finish.\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        printf("Log entry %d: ", count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        int timestamp;
        char level[16];
        char message[MAX_LINE_LENGTH];
        
        int parsed = sscanf(line, "%15s %d %1023[^\n]", level, &timestamp, message);
        
        if (parsed == 3 && timestamp >= 0 && parse_log_level(level) != -1) {
            strncpy(entry.level, level, sizeof(entry.level) - 1);
            entry.level[sizeof(entry.level) - 1] = '\0';
            strncpy(entry.message, message, sizeof(entry.message) - 1);
            entry.message[sizeof(entry.message) - 1] = '\0';
            entry.timestamp = timestamp;
            
            logs[count] = entry;
            count++;
        } else {
            printf("Invalid format. Use: LEVEL TIMESTAMP MESSAGE\n");
        }
    }
    
    return count;
}

int main() {
    struct LogEntry logs[100];
    int log_count = read_log_file(logs, 100);
    
    if (log_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total logs: %d\n", log_count);
    
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int i = 0; i < 5; i++) {
        int count = count_logs_by_level(logs, 0, log_count - 1, i);
        printf("%s: %d\n", levels[i], count);
    }
    
    printf("\n=== High Severity Logs (WARNING and above) ===\n");
    int high_severity_count = 0;
    filter_logs_by_level(logs, 0, log_count - 1, 2, &high_severity_count);
    printf("Total high severity logs: %d\n", high_severity_count);
    
    return 0;
}