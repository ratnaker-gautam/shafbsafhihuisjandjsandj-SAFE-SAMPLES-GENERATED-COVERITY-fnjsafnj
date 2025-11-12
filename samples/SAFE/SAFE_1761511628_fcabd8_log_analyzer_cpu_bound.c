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
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token && count < 3) {
        parts[count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp|level|message). Enter 'END' to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count++] = entry;
            
            if (strcmp(entry.level, "ERROR") == 0) {
                error_count++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                warning_count++;
            } else if (strcmp(entry.level, "INFO") == 0) {
                info_count++;
            }
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (entry_count > 0) {
        printf("\nSample of processed entries:\n");
        int samples = (entry_count < 5) ? entry_count : 5;
        for (int i = 0; i < samples; i++) {
            printf("%d: [%s] %s - %s\n", i + 1, 
                   entries[i].timestamp, entries[i].level, entries[i].message);
        }
        
        clock_t start = clock();
        
        int total_chars = 0;
        for (int i = 0; i < entry_count; i++) {
            total_chars += strlen(entries[i].message);
        }
        
        double avg_length = (entry_count > 0) ? (double)total_chars / entry_count : 0.0;
        
        clock_t end = clock();
        double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        
        printf("\nCPU-intensive analysis completed in %.6f seconds\n", cpu_time);
        printf("Average message length: %.2f characters\n", avg_length);
    }
    
    return 0;
}