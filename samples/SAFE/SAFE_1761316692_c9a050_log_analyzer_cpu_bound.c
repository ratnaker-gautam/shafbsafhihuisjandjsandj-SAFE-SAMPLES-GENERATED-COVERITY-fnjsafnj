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
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void analyze_log_patterns(const LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, "error") || strstr(entries[i].message, "ERROR")) {
            error_count++;
        }
        if (strstr(entries[i].message, "warning") || strstr(entries[i].message, "WARNING")) {
            warning_count++;
        }
        if (strstr(entries[i].message, "info") || strstr(entries[i].message, "INFO")) {
            info_count++;
        }
    }
    
    printf("Pattern analysis:\n");
    printf("  Messages containing 'error': %d\n", error_count);
    printf("  Messages containing 'warning': %d\n", warning_count);
    printf("  Messages containing 'info': %d\n", info_count);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total log entries: %d\n", entry_count);
    
    int error_count = count_log_level(entries, entry_count, "ERROR");
    int warning_count = count_log_level(entries, entry_count, "WARN");
    int info_count = count_log_level(entries, entry_count, "INFO");
    int debug_count = count_log_level(entries, entry_count, "DEBUG");
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARN entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("DEBUG entries: %d\n", debug_count);
    
    printf("\n");
    analyze_log_patterns(entries, entry_count);
    
    printf("\nTop 5 most frequent log levels:\n");
    int counts[4] = {error_count, warning_count, info_count, debug_count};
    const char* levels[4] = {"ERROR", "WARN", "INFO", "DEBUG"};
    
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (counts[j] > counts[i]) {
                int temp_count = counts[i];
                counts[i] = counts[j];
                counts[j] = temp_count;
                
                const char* temp_level = levels[i];
                levels[i] = levels[j];
                levels[j] = temp_level;
            }
        }
    }
    
    for (int i = 0; i < 4 && i < 5; i++) {
        printf("%d. %s: %d entries\n", i + 1, levels[i], counts[i]);
    }
    
    return 0;
}