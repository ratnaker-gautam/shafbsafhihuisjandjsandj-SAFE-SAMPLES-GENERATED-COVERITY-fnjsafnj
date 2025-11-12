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
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* tokens[3];
    char* token = strtok(temp_line, "|");
    int token_count = 0;
    
    while (token != NULL && token_count < 3) {
        tokens[token_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (token_count != 3) return 0;
    
    strncpy(entry->timestamp, tokens[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, tokens[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, tokens[2], 255);
    entry->message[255] = '\0';
    
    entry->error_count = 0;
    if (strstr(entry->level, "ERROR") != NULL) {
        entry->error_count = 1;
    }
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    
    printf("Log Analyzer - Processing simulated log data\n");
    printf("Enter log lines in format: timestamp|level|message\n");
    printf("Type 'END' on a separate line to finish input\n\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nProcessing %d log entries...\n", entry_count);
    
    clock_t start_time = clock();
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    int error_entries = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            error_entries++;
        }
    }
    
    double error_rate = (double)error_entries / entry_count * 100.0;
    
    char earliest[32] = "", latest[32] = "";
    if (entry_count > 0) {
        strncpy(earliest, entries[0].timestamp, 31);
        strncpy(latest, entries[entry_count - 1].timestamp, 31);
    }
    
    clock_t end_time = clock();
    double processing_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n=== Analysis Results ===\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Error entries: %d\n", error_entries);
    printf("Error rate: %.2f%%\n", error_rate);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Processing time: %.6f seconds\n", processing_time);
    
    if (error_entries > 0) {
        printf("\n=== Recent Error Entries ===\n");
        int displayed = 0;
        for (int i = entry_count - 1; i >= 0 && displayed < 5; i--) {
            if (entries[i].error_count > 0) {
                printf("%s | %s | %s\n", 
                       entries[i].timestamp, 
                       entries[i].level, 
                       entries[i].message);
                displayed++;
            }
        }
    }
    
    return 0;
}