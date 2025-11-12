//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token && count < 3) {
        parts[count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp)) return 0;
    if (strlen(parts[1]) >= sizeof(entry->level)) return 0;
    if (strlen(parts[2]) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, parts[0]);
    strcpy(entry->level, parts[1]);
    strcpy(entry->message, parts[2]);
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || 
           strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0;
}

void analyze_logs(LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    char last_timestamp[32] = "";
    int total_messages = 0;
    
    for (int i = 0; i < count; i++) {
        if (!is_valid_level(entries[i].level)) continue;
        
        total_messages++;
        
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        
        if (strlen(entries[i].timestamp) > 0) {
            strcpy(last_timestamp, entries[i].timestamp);
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total messages processed: %d\n", total_messages);
    printf("INFO level messages: %d\n", info_count);
    printf("WARN level messages: %d\n", warn_count);
    printf("ERROR level messages: %d\n", error_count);
    
    if (strlen(last_timestamp) > 0) {
        printf("Last timestamp: %s\n", last_timestamp);
    }
    
    if (total_messages > 0) {
        printf("INFO percentage: %.2f%%\n", (info_count * 100.0) / total_messages);
        printf("WARN percentage: %.2f%%\n", (warn_count * 100.0) / total_messages);
        printf("ERROR percentage: %.2f%%\n", (error_count * 100.0) / total_messages);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count++] = entry;
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}