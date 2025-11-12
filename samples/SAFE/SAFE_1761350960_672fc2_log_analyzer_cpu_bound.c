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
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = 0;
    if (strcmp(level, "ERROR") == 0) {
        entry->error_count = 1;
    }
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    int total_info = 0;
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    while (fgets(line, MAX_LINE_LEN, stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_count++;
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                total_warnings++;
            } else if (strcmp(entry.level, "INFO") == 0) {
                total_info++;
            }
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total lines processed: %d\n", line_count);
    printf("Valid log entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", total_errors);
    printf("WARNING entries: %d\n", total_warnings);
    printf("INFO entries: %d\n", total_info);
    printf("\n");
    
    if (entry_count > 0) {
        printf("Sample of recent entries:\n");
        int start = entry_count > 5 ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    clock_t start_time = clock();
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < strlen(entries[i].message); j++) {
            entries[i].message[j] = (char)toupper((unsigned char)entries[i].message[j]);
        }
    }
    
    int word_count = 0;
    for (int i = 0; i < entry_count; i++) {
        char* msg = entries[i].message;
        int in_word = 0;
        for (int j = 0; msg[j] != '\0'; j++) {
            if (isalpha((unsigned char)msg[j])) {
                if (!in_word) {
                    word_count++;
                    in_word = 1;
                }
            } else {
                in_word = 0;
            }
        }
    }
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\nCPU-intensive analysis completed:\n");
    printf("Total words in log messages: %d\n", word_count);
    printf("Processing time: %.6f seconds\n", cpu_time);
    
    return 0;
}