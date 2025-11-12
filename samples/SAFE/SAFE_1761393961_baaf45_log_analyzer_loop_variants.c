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
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp)-1);
            strncpy(entries[count].level, level, sizeof(entries[count].level)-1);
            strncpy(entries[count].message, message, sizeof(entries[count].message)-1);
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
    
    while (i < count) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
        i++;
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int j;
    for (j = 0; j < 5; j++) {
        printf("%-8s: %d\n", levels[j], severity_count[j]);
    }
    
    int error_count = 0;
    int k = 0;
    do {
        if (entries[k].severity >= 3) {
            error_count++;
        }
        k++;
    } while (k < count);
    
    printf("Errors and above: %d\n", error_count);
}

void search_logs(struct LogEntry* entries, int count, const char* keyword) {
    if (!keyword || strlen(keyword) == 0) return;
    
    printf("Search results for '%s':\n", keyword);
    printf("====================\n");
    
    int found = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (strstr(entries[i].message, keyword)) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    char keyword[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin)) {
        size_t len = strlen(filename);
        if (len > 0 && filename[len-1] == '\n') {
            filename[len-1] = '\0';
        }
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename.\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count <= 0) {
        printf("Failed to read log file or file is empty.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    printf("\nEnter search keyword (or press Enter to skip): ");
    if (fgets(keyword, sizeof(keyword), stdin)) {
        size_t len = strlen(keyword);
        if (len > 0 && keyword[len-1] == '\n') {
            keyword[len-1] = '\0';
        }
        
        if (strlen(keyword) > 0) {
            search_logs(entries, entry_count, keyword);
        }
    }
    
    return 0;
}