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
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* token = strtok(temp, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

int count_log_level(const LogEntry* logs, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void generate_sample_logs(LogEntry* logs, int* count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "File not found",
        "Memory allocation failed",
        "Network timeout",
        "Configuration loaded",
        "Invalid input detected",
        "Backup completed",
        "Security alert triggered",
        "Service started"
    };
    
    *count = 5000;
    for (int i = 0; i < *count; i++) {
        int level_idx = rand() % 4;
        int msg_idx = rand() % 10;
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        strftime(logs[i].timestamp, sizeof(logs[i].timestamp), 
                "%Y-%m-%d %H:%M:%S", tm_info);
        
        strcpy(logs[i].level, levels[level_idx]);
        strcpy(logs[i].message, messages[msg_idx]);
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(logs, &log_count);
    
    if (log_count == 0) {
        printf("No logs to analyze.\n");
        return 1;
    }
    
    printf("Analyzing %d log entries...\n", log_count);
    
    int info_count = count_log_level(logs, log_count, "INFO");
    int warning_count = count_log_level(logs, log_count, "WARNING");
    int error_count = count_log_level(logs, log_count, "ERROR");
    int debug_count = count_log_level(logs, log_count, "DEBUG");
    
    printf("\nLog Level Statistics:\n");
    printf("INFO: %d entries (%.1f%%)\n", info_count, 
           (double)info_count / log_count * 100);
    printf("WARNING: %d entries (%.1f%%)\n", warning_count, 
           (double)warning_count / log_count * 100);
    printf("ERROR: %d entries (%.1f%%)\n", error_count, 
           (double)error_count / log_count * 100);
    printf("DEBUG: %d entries (%.1f%%)\n", debug_count, 
           (double)debug_count / log_count * 100);
    
    int total_processed = info_count + warning_count + error_count + debug_count;
    if (total_processed != log_count) {
        printf("Warning: Some entries could not be categorized.\n");
    }
    
    printf("\nAnalysis complete. Processed %d total entries.\n", total_processed);
    
    return 0;
}