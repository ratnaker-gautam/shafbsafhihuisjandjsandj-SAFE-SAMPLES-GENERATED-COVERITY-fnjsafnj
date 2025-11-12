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

int parse_log_recursive(FILE *file, struct LogEntry *entries, int max_entries, int current_depth) {
    if (current_depth >= max_entries || feof(file) || current_depth >= MAX_LEVELS) {
        return current_depth;
    }

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return current_depth;
    }

    if (strlen(line) == 0 || line[0] == '\n') {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
    
    int parsed = sscanf(line, "%d %15s %1023[^\n]", &timestamp, level, message);
    
    if (parsed == 3 && timestamp >= 0 && timestamp <= 86400) {
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry->timestamp = timestamp;
        
        for (char *p = entry->level; *p; p++) {
            *p = toupper((unsigned char)*p);
        }
        
        return parse_log_recursive(file, entries, max_entries, current_depth + 1);
    }
    
    return parse_log_recursive(file, entries, max_entries, current_depth);
}

void analyze_logs_recursive(struct LogEntry *entries, int count, int index, int *error_count, int *warning_count, int *info_count) {
    if (index >= count) {
        return;
    }

    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }

    analyze_logs_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void display_logs_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }

    printf("Time: %05d | Level: %-7s | Message: %s\n", 
           entries[index].timestamp, entries[index].level, entries[index].message);
    display_logs_recursive(entries, count, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 12345 INFO System started\n");
    printf("Enter 'quit' on a separate line to finish input\n");
    printf("Maximum 100 entries, timestamp 0-86400\n\n");
    
    char input[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (line_count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (strlen(input) >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipping\n");
            continue;
        }
        
        FILE *temp = tmpfile();
        if (temp == NULL) {
            printf("Error creating temporary file\n");
            return 1;
        }
        
        fputs(input, temp);
        rewind(temp);
        
        int new_count = parse_log_recursive(temp, entries, 100, entry_count);
        fclose(temp);
        
        if (new_count > entry_count) {
            printf("Added log entry: %s\n", input);
            entry_count = new_count;
        } else {
            printf("Invalid format: %s\n", input);
        }
        
        line_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", entry_count);
    
    int error_count = 0, warning_count = 0, info_count = 0;
    analyze_logs_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\n=== Log Entries ===\n");
    display_logs_recursive(entries, entry_count, 0);
    return