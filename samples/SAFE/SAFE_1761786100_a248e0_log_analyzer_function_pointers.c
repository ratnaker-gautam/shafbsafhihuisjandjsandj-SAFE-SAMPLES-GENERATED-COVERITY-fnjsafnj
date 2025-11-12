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

        char* timestamp = strtok(line, " ");
        char* level = strtok(NULL, " ");
        char* message = strtok(NULL, "\n");

        if (timestamp != NULL && level != NULL && message != NULL) {
            if (strlen(timestamp) < sizeof(entry.timestamp) &&
                strlen(level) < sizeof(entry.level) &&
                strlen(message) < sizeof(entry.message)) {
                strcpy(entry.timestamp, timestamp);
                strcpy(entry.level, level);
                strcpy(entry.message, message);
                processor(&entry);
            }
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
    }

    fclose(file);
}

int main() {
    const char* log_files[] = {
        "system.log",
        "application.log",
        "security.log"
    };
    const int num_files = sizeof(log_files) / sizeof(log_files[0]);

    LogProcessor processors[] = {
        count_entries,
        show_errors,
        show_warnings
    };
    const int num_processors = sizeof(processors) / sizeof(processors[0]);

    printf("Log Analysis Results:\n");
    printf("====================\n");

    for (int i = 0; i < num_files; i++) {
        printf("\nProcessing file: %s\n", log_files[i]);
        for (int j = 0; j < num_processors; j++) {
            process_log_file(log_files[i], processors[j]);
        }
    }

    printf("\nAnalysis complete.\n");
    return 0;
}