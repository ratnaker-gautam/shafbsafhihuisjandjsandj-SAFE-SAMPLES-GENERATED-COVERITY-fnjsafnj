//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int validate_timestamp(const char* timestamp) {
    if (timestamp == NULL) return 0;
    
    int year, month, day, hour, min, sec;
    if (sscanf(timestamp, "%d-%d-%dT%d:%d:%d", 
               &year, &month, &day, &hour, &min, &sec) != 6) return 0;
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log lines (format: YYYY-MM-DDTHH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (validate_timestamp(entry.timestamp) && 
                (strcmp(entry.level, "INFO") == 0 || 
                 strcmp(entry.level, "WARNING") == 0 || 
                 strcmp(entry.level, "ERROR") == 0)) {
                
                entries[entry_count] = entry;
                total_errors += entry.error_count;
                entry_count++;
            }
        }
    }
    
    printf("\\nAnalysis Results:\\n");
    printf("Total log entries: %d\\n", entry_count);
    printf("Total errors: %d\\n", total_errors);
    
    if (entry_count > 0) {
        int info_count = 0, warning_count = 0, error_count = 0;
        
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "INFO") == 0) info_count++;
            else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
            else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        }
        
        printf("INFO entries: %d (%.1f%%)\\n", info_count, 
               (float)info_count / entry_count * 100);
        printf("WARNING entries: %d (%.1f%%)\\n", warning_count, 
               (float)warning_count / entry_count * 100);
        printf("ERROR entries: %d (%.1f%%)\\n", error_count, 
               (float)error_count / entry_count * 100);
        
        if (error_count > 0) {
            printf("\\nError messages:\\n");
            for (int i = 0; i < entry_count; i++) {
                if (strcmp(entries[i].level, "ERROR") == 0) {
                    printf("- %s: %s\\n", entries[i].timestamp, entries[i].message);
                }
            }
        }
    }
    
    return 0;
}