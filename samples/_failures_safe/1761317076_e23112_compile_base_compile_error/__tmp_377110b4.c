//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    parts[0] = temp;
    int part_count = 1;
    
    for (char* p = temp; *p && part_count < 3; p++) {
        if (*p == '|') {
            *p = '\0';
            parts[part_count++] = p + 1;
        }
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= 64 || strlen(parts[1]) >= 16 || strlen(parts[2]) >= 256) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], 63);
    entry->timestamp[63] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    return 1;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG", "FATAL"};
    for (int i = 0; i < 5; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void generate_sample_logs(LogEntry* logs, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    const char* messages[] = {
        "User login successful",
        "Database connection established",
        "Memory usage high",
        "Request processed",
        "File not found",
        "Invalid input detected",
        "Cache cleared",
        "Network timeout"
    };
    
    time_t base_time = time(NULL);
    
    for (int i = 0; i < count; i++) {
        if (i >= MAX_ENTRIES) break;
        snprintf(logs[i].timestamp, 64, "%ld", base_time + i);
        strncpy(logs[i].level, levels[i % 4], 15);
        logs[i].level[15] = '\0';
        strncpy(logs[i].message, messages[i % 8], 255);
        logs[i].message[255] = '\0';
    }
}

void analyze_logs(LogEntry* logs, int count) {
    int level_counts[5] = {0};
    const char* level_names[] = {"INFO", "WARN", "ERROR", "DEBUG", "FATAL"};
    
    if (count > MAX_ENTRIES) count = MAX_ENTRIES;
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < 5; j++) {
            if (strcmp(logs[i].level, level_names[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < 5; i++) {
        if (level_counts[i] > 0) {
            double percentage = (double)level_counts[i] / count * 100.0;
            printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i], percentage);
        }
    }
    
    int error_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
            if (error_count <= 5) {
                printf("Error message %d: %s\n", error_count, logs[i].message);
            }
        }
    }
    
    if (error_count > 5) {
        printf("... and %d more error messages\n", error_count - 5);
    }
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 5000;
    
    if (log_count > MAX_ENTRIES) log_count = MAX_ENTRIES;
    
    printf("Generating sample log data...\n");
    generate_sample_logs(logs, log_count);
    
    printf("Analyzing %d log entries...\n", log_count);
    
    clock_t start = clock();
    analyze_logs(logs, log_count);
    clock_t end = clock();
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Analysis completed in %.3f seconds\n", cpu_time);
    
    int valid_count = 0;
    for (int i = 0; i < log_count; i++) {