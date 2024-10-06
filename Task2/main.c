#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Student structure
struct Student {
    int id;                    
    char name[50];             
    struct Student* next;       
    struct Student* prev;       
};

// Function to create a new node
struct Student* createStudent(int id, char* name) {
    struct Student* newStudent = (struct Student*)malloc(sizeof(struct Student));
    newStudent->id = id;
    strcpy(newStudent->name, name);
    newStudent->next = NULL;
    newStudent->prev = NULL;
    return newStudent;
}

// Insert a new node at the head of the doubly linked list
void insertAtHead(struct Student** head, int id, char* name) {
    struct Student* newStudent = createStudent(id, name);
    newStudent->next = *head;
    
    if (*head != NULL) {
        (*head)->prev = newStudent;
    }
    *head = newStudent;
}

// Display all candidates data in the list
void displayList(struct Student* head) {
    struct Student* current = head;
    if (current == NULL) {
        printf("No student data available.\n");
    } else {
        while (current != NULL) {
            printf("ID: %d, Name: %s\n", current->id, current->name);
            current = current->next;
        }
    }
}

// Delete a scandidate from the list by ID
void deleteStudentByID(struct Student** head, int id) {
    struct Student* current = *head;

    // Find the candidate with the specified ID
    while (current != NULL && current->id != id) {
        current = current->next;
    }

    // If the candidate was not found
    if (current == NULL) {
        printf("Student with ID %d not found.\n", id);
        return;
    }

    // If the candidate is the head of the list
    if (current == *head) {
        *head = current->next;
        if (*head != NULL) {
            (*head)->prev = NULL;
        }
    } else {
        // Unlink the  node from the list
        if (current->next != NULL) {
            current->next->prev = current->prev;
        }
        if (current->prev != NULL) {
            current->prev->next = current->next;
        }
    }

    free(current);
    printf("Student with ID %d deleted.\n", id);
}

// Sort the list based on the unique identifier (ID) using bubble sort
void sortListByID(struct Student** head) {
    struct Student* current;
    struct Student* next;
    int tempID;
    char tempName[50];

    if (*head == NULL)
        return;

    // Simple bubble sort algorithm for the doubly linked list
    for (current = *head; current != NULL; current = current->next) {
        for (next = current->next; next != NULL; next = next->next) {
            if (current->id > next->id) {
                // Swap the IDs
                tempID = current->id;
                current->id = next->id;
                next->id = tempID;

                // Swap the names
                strcpy(tempName, current->name);
                strcpy(current->name, next->name);
                strcpy(next->name, tempName);
            }
        }
    }
}

int main() {
    struct Student* studentList = NULL;
    char name[50];
    int id, choice;

    while (1) {
        printf("\nMenu:\n1. Insert Student\n2. Display Students\n3. Delete Student by ID\n4. Sort by ID\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\n******************************\n");

        switch (choice) {
            case 1: // Insert Student
                printf("Enter student ID: ");
                scanf("%d", &id);
                printf("Enter student name: ");
                scanf(" %[^\n]%*c", name); // To read string with spaces
                insertAtHead(&studentList, id, name);
                printf("******************************");
                break;

            case 2: // Display all students
                printf("Student list:\n");
                displayList(studentList);
                printf("******************************");
                break;

            case 3: // Delete student by ID
                printf("Enter student ID to delete: ");
                scanf("%d", &id);
                deleteStudentByID(&studentList, id);
                printf("******************************");
                break;

            case 4: // Sort by ID
                sortListByID(&studentList);
                printf("Student list sorted by ID.\n");
                printf("******************************");
                break;

            case 5: // Exit
                exit(0);

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
