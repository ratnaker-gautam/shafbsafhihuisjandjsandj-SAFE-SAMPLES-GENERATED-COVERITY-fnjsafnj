//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    int level;
    char message[MAX_LINE_LENGTH];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

const char* level_to_string(int level) {
    switch (level) {
        case 0: return "DEBUG";
        case 1: return "INFO";
        case 2: return "WARNING";
        case 3: return "ERROR";
        case 4: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int* level_counts) {
    if (start > end) return;
    
    if (start == end) {
        if (entries[start].level >= 0 && entries[start].level <= 4) {
            level_counts[entries[start].level]++;
        }
        return;
    }
    
    int mid = start + (end - start) / 2;
    
    analyze_log_recursive(entries, start, mid, level_counts);
    analyze_log_recursive(entries, mid + 1, end, level_counts);
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: LEVEL MESSAGE). Enter 'END' to finish.\n");
    
    while (count < max_entries) {
        printf("Log entry %d: ", count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        int parsed = sscanf(line, "%31s %1023[^\n]", level_str, message);
        
        if (parsed < 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Unknown log level: %s\n", level_str);
            continue;
        }
        
        if (strlen(message) >= MAX_LINE_LENGTH) {
            printf("Message too long\n");
            continue;
        }
        
        entries[count].level = level;
        strncpy(entries[count].message, message, MAX_LINE_LENGTH - 1);
        entries[count].message[MAX_LINE_LENGTH - 1] = '\0';
        count++;
    }
    
    return count;
}

int main() {
    struct LogEntry entries[1000];
    int level_counts[5] = {0};
    
    int entry_count = read_log_entries(entries, 1000);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 0;
    }
    
    analyze_log_recursive(entries, 0, entry_count - 1, level_counts);
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_to_string(i), level_counts[i]);
    }
    
    int total_entries = 0;
    for (int i = 0; i < 5; i++) {
        total_entries += level_counts[i];
    }
    printf("Total   : %d entries\n", total_entries);
    
    return 0;
}