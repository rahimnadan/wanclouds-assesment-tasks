#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[]) {
if (argc != 2) {
fprintf(stderr, "Usage: %s <your_name>\n", argv[0]);
return 1;
}
char *name = argv[1];
printf("Running process with name: %s\n", name);
// Simulate some activity by calling a system call in a loop
for (int i = 0; i < 5; i++) {
printf("Process %s is doing some work... (%d)\n", name, i + 1);
sleep(10); // Simulating work with sleep
}

return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/prctl.h>  // Required for prctl and PR_SET_NAME

// int main(int argc, char *argv[]) {
//     if (argc < 3) {
//         fprintf(stderr, "Usage: %s <name> <file_path>\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }

//     // Change the process name to your name, so it can be identified in the Linux kernel
//     char *name = argv[1];
//     prctl(PR_SET_NAME, name);  // Set the process name in the kernel

//     // File path to open
//     char *file_path = argv[2];

//     // Open the file for reading
//     int fd = open(file_path, O_RDONLY);
//     if (fd == -1) {
//         perror("Error opening file");
//         exit(EXIT_FAILURE);
//     }

//     // Read file content
//     char buffer[256];
//     ssize_t bytes_read;
//     while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
//         buffer[bytes_read] = '\0';  // Null-terminate the string
//         printf("%s", buffer);
//     }

//     if (bytes_read == -1) {
//         perror("Error reading file");
//         close(fd);
//         exit(EXIT_FAILURE);
//     }

//     close(fd);  // Close the file
//     printf("\nFile reading completed.\n");
    
//     return 0;
// }
