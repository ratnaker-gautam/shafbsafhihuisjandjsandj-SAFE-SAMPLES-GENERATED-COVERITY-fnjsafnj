//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        if (sscanf(line, "%31[^|]|%15[^|]|%255[^\n]", timestamp, level, message) == 3) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            entries[count].severity = parse_severity(level);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int severity_count[5] = {0};
    int i = 0;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("Log Analysis Results:\n");
    printf("=====================\n");
    
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int j;
    for (j = 0; j < 5; j++) {
        printf("%-8s: %d entries\n", levels[j], severity_count[j]);
    }
    
    int error_count = severity_count[3] + severity_count[4];
    printf("\nTotal errors: %d\n", error_count);
    
    if (count > 0) {
        double error_percentage = (double)error_count / count * 100.0;
        printf("Error percentage: %.2f%%\n", error_percentage);
    }
}

void search_logs(struct LogEntry* entries, int count, const char* keyword) {
    if (!keyword || strlen(keyword) == 0) return;
    
    printf("\nSearch results for '%s':\n", keyword);
    printf("========================\n");
    
    int found = 0;
    int k = 0;
    
    while (k < count) {
        if (strstr(entries[k].message, keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
            found = 1;
        }
        k++;
    }
    
    if (!found) {
        printf("No entries found containing '%s'\n", keyword);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    char search_term[128];
    
    printf("Enter log file name: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count <= 0) {
        printf("Error reading log file or file is empty\n");
        return 1;
    }
    
    printf("Successfully read %d log entries\n\n", entry_count);
    
    analyze_logs(entries, entry_count);
    
    printf("\nEnter search term (or press Enter to skip): ");
    if (fgets(search_term, sizeof(search_term), stdin)) {
        len = strlen(search_term);
        if (len > 0 && search_term[len - 1] == '\n') {
            search_term[len - 1] = '\0';
        }
        
        if (strlen(search_term) > 0) {
            search_logs(entries, entry_count, search_term);
        }
    }
    
    return 0;
}