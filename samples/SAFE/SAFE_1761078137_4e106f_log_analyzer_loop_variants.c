//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    if (sscanf(temp, "%19s %9s %[^\n]", 
               entry->timestamp, entry->level, entry->message) != 3) {
        return 0;
    }
    
    for (int i = 0; entry->level[i]; i++) {
        entry->level[i] = toupper(entry->level[i]);
    }
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int display_count = (count < 5) ? count : 5;
        for (int i = count - display_count; i < count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, 
                   entries[i].level, entries[i].message);
        }
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log entries (empty line to finish):\n");
    printf("Format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log %d: ", entry_count + 1);
        
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            break;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid format. Please use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}