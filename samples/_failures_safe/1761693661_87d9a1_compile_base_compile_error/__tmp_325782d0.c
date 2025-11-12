//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

void print_log_entry(const struct LogEntry* entry, int indent) {
    if (indent < 0 || indent > MAX_LEVELS) return;
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

void analyze_log_recursive(struct LogEntry* entries, int count, int current, int depth) {
    if (current >= count || current < 0 || depth >= MAX_LEVELS) return;
    
    struct LogEntry* entry = &entries[current];
    int level = parse_log_level(entry->level);
    
    if (level >= 2) {
        print_log_entry(entry, depth);
    }
    
    if (strstr(entry->message, "start") != NULL || strstr(entry->message, "begin") != NULL) {
        analyze_log_recursive(entries, count, current + 1, depth + 1);
    } else if (strstr(entry->message, "end") != NULL || strstr(entry->message, "complete") != NULL) {
        return;
    } else {
        analyze_log_recursive(entries, count, current + 1, depth);
    }
}

int read_log_entries(FILE* file, struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LENGTH - 1) continue;
        
        char level[16];
        int timestamp;
        char message[MAX_LINE_LENGTH];
        
        int parsed = sscanf(line, "%15s %d %1023[^\n]", level, &timestamp, message);
        if (parsed == 3) {
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            entries[count].level[sizeof(entries[count].level) - 1] = '\0';
            entries[count].timestamp = timestamp;
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            count++;
        }
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[1000];
    int entry_count = 0;
    
    printf("Enter log entries (format: LEVEL TIMESTAMP MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' to finish input\n");
    
    char input[MAX_LINE_LENGTH];
    while (entry_count < 1000) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (strlen(input) >= MAX_LINE_LENGTH - 1) {
            printf("Input too long, skipping\n");
            continue;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "END") == 0) break;
        
        char level[16];
        int timestamp;
        char message[MAX_LINE_LENGTH];
        
        int parsed = sscanf(input, "%15s %d %1023[^\n]", level, &timestamp, message);
        if (parsed == 3) {
            if (parse_log_level(level) >= 0) {
                strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
                entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
                entries[entry_count].timestamp = timestamp;
                strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
                entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
                entry_count++;
            } else {
                printf("Invalid log level: %s\n", level);
            }
        } else {
            printf("Invalid format. Use: LEVEL TIMESTAMP MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze\n");
        return 1;
    }
    
    printf("\nAnalyzing log entries (showing WARNING and above with indentation):\n");
    analyze_log_recursive(entries, entry_count, 0, 0);
    
    return