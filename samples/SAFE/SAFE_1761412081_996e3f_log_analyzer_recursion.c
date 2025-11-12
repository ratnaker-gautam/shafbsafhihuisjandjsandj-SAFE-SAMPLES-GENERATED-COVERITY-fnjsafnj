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

void analyze_log_recursive(struct LogEntry* entries, int start, int end, 
                          int* error_count, int* warning_count, int* info_count) {
    if (start > end) return;
    if (!entries || !error_count || !warning_count || !info_count) return;
    
    if (start == end) {
        if (strcmp(entries[start].level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entries[start].level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entries[start].level, "INFO") == 0) {
            (*info_count)++;
        }
        return;
    }
    
    int mid = start + (end - start) / 2;
    
    if (mid < start || mid > end) return;
    
    analyze_log_recursive(entries, start, mid, error_count, warning_count, info_count);
    analyze_log_recursive(entries, mid + 1, end, error_count, warning_count, info_count);
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: timestamp LEVEL message):\n");
    printf("Example: 1234567890 INFO System started\\n");
    printf("Enter empty line to finish.\\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[count] = entry;
            count++;
        } else {
            printf("Invalid log format. Skipping: %s\\n", line);
        }
    }
    
    return count;
}

int main() {
    struct LogEntry entries[100];
    int entry_count;
    
    entry_count = read_log_entries(entries, 100);
    
    if (entry_count <= 0) {
        printf("No valid log entries to analyze.\\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(entries, 0, entry_count - 1, 
                         &error_count, &warning_count, &info_count);
    
    printf("\\nLog Analysis Results:\\n");
    printf("Total entries: %d\\n", entry_count);
    printf("ERROR entries: %d\\n", error_count);
    printf("WARNING entries: %d\\n", warning_count);
    printf("INFO entries: %d\\n", info_count);
    
    return 0;
}