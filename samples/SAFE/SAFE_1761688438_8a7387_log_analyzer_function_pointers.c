//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    if (filename == NULL || processor == NULL) {
        fprintf(stderr, "Invalid input parameters\n");
        return;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            fprintf(stderr, "Too many log entries\n");
            break;
        }

        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Line too long at line %d\n", line_count);
            continue;
        }

        LogEntry entry;
        memset(&entry, 0, sizeof(entry));

        char* tokens[3];
        char* token = strtok(line, "|");
        int token_count = 0;

        while (token != NULL && token_count < 3) {
            tokens[token_count] = token;
            token_count++;
            token = strtok(NULL, "|");
        }

        if (token_count == 3) {
            strncpy(entry.timestamp, tokens[0], sizeof(entry.timestamp) - 1);
            strncpy(entry.level, tokens[1], sizeof(entry.level) - 1);
            strncpy(entry.message, tokens[2], sizeof(entry.message) - 1);
            
            entry.timestamp[sizeof(entry.timestamp) - 1] = '\0';
            entry.level[sizeof(entry.level) - 1] = '\0';
            entry.message[sizeof(entry.message) - 1] = '\0';
            
            processor(&entry);
        } else {
            fprintf(stderr, "Invalid log format at line %d\n", line_count);
        }
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Error closing file\n");
    }
}

void generate_sample_log(const char* filename) {
    if (filename == NULL) return;

    FILE* file = fopen(filename, "w");
    if (file == NULL) return;

    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started",
        "User login successful",
        "Disk space low",
        "Network connection lost",
        "Database query failed",
        "Backup completed"
    };

    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    for (int i = 0; i < 10; i++) {
        char timestamp[32];
        tm_info->tm_hour = (tm_info->tm_hour + i) % 24;
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = i % 3;
        int msg_idx = i % 6;
        
        fprintf(file, "%s|%s|%s\n", timestamp, levels[level_idx], messages[msg_idx]);
    }

    fclose(file);
}

int main() {
    const char* logfile = "sample.log";
    
    printf("Generating sample log file...\n");
    generate_sample_log(logfile);
    
    printf("\nProcessing log entries:\n");
    process_log_file(logfile, count_entries);
    
    printf("\nError entries:\n");
    process_log_file(logfile, show_errors);
    
    printf("\nWarning entries:\n");
    process_log_file(logfile, show_warnings);
    
    return 0;
}