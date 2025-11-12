//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

void generate_sample_logs(LogEntry* entries, int* count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login attempt",
        "Database connection established",
        "Memory usage high",
        "Network timeout",
        "File not found",
        "Permission denied",
        "Configuration updated"
    };
    
    *count = 5000;
    if (*count > MAX_ENTRIES) *count = MAX_ENTRIES;
    
    for (int i = 0; i < *count; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        strftime(entries[i].timestamp, 31, "%Y-%m-%d %H:%M:%S", tm_info);
        
        strncpy(entries[i].level, levels[level_idx], 15);
        entries[i].level[15] = '\0';
        strncpy(entries[i].message, messages[msg_idx], 255);
        entries[i].message[255] = '\0';
        
        entries[i].error_count = (level_idx == 2) ? 1 : 0;
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(entries, &entry_count);
    
    printf("Analyzing %d log entries...\n", entry_count);
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    
    if (entry_count > 0) {
        printf("\nFirst 5 entries (sorted by timestamp):\n");
        for (int i = 0; i < 5 && i < entry_count; i++) {
            printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
        
        printf("\nLast 5 entries (sorted by timestamp):\n");
        for (int i = entry_count - 5; i < entry_count && i >= 0; i++) {
            printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    double error_rate = (double)error_count / entry_count * 100.0;
    printf("\nError rate: %.2f%%\n", error_rate);
    
    if (error_rate > 10.0) {
        printf("WARNING: High error rate detected