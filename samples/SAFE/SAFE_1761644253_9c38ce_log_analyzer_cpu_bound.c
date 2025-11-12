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

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started", "User login", "File opened", 
        "Database connection lost", "Memory allocation failed",
        "Network timeout", "Invalid input", "Operation completed"
    };
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        time_t log_time = base_time - (count - i) * 60;
        struct tm* tm_info = localtime(&log_time);
        
        strftime(logs[i].timestamp, 32, "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 3;
        strncpy(logs[i].level, levels[level_idx], 15);
        logs[i].level[15] = '\0';
        
        int msg_idx = rand() % 8;
        strncpy(logs[i].message, messages[msg_idx], 255);
        logs[i].message[255] = '\0';
        
        logs[i].error_count = (level_idx == 2) ? 1 : 0;
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    char last_error[256] = "";
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
            strncpy(last_error, logs[i].message, 255);
            last_error[255] = '\0';
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    
    if (error_count > 0) {
        printf("Last error message: %s\n", last_error);
    }
    
    double error_rate = (double)error_count / count * 100.0;
    printf("Error rate: %.2f%%\n", error_rate);
}

int main() {
    srand((unsigned int)time(NULL));
    
    int log_count;
    printf("Enter number of log entries to generate (1-%d): ", MAX_ENTRIES);
    
    if (scanf("%d", &log_count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (log_count < 1 || log_count > MAX_ENTRIES) {
        printf("Count must be between 1 and %d\n", MAX_ENTRIES);
        return 1;
    }
    
    LogEntry logs[MAX_ENTRIES];
    
    generate_sample_logs(logs, log_count);
    
    printf("\nGenerated %d log entries:\n", log_count);
    for (int i = 0; i < (log_count < 5 ? log_count : 5); i++) {
        printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    if (log_count > 5) {
        printf("... and %d more entries\n", log_count - 5);
    }
    
    printf("\n");
    analyze_logs(logs, log_count);
    
    return 0;
}