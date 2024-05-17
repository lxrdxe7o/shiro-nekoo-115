#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 200
#define MAX_DOCTOR_REMARK 200
#define MAX_PASSWORD_LENGTH 50
#define MAX_USER_NAME 30
#define MAX_PATIENTS 100
#define BLOOD_CHAR_MAX 5
#define MAX_CONTACT_LENGTH 15

typedef struct patientInfo
{
    char patientName[MAX_NAME_LENGTH];
    char patientAddress[MAX_ADDRESS_LENGTH];
    char prescription[MAX_DOCTOR_REMARK];
    char patientPass[MAX_PASSWORD_LENGTH];
    int age;
    int id;
    float height;
    float weight;
    char patientContact[MAX_CONTACT_LENGTH];
    char emergencyContact[MAX_CONTACT_LENGTH];
    char patientUserName[MAX_USER_NAME];
    char bloodType[BLOOD_CHAR_MAX];
} patientInfo;

void getPatientInfo(patientInfo *patient)
{
    printf("Input name: ");
    fgets(patient->patientName, sizeof(patient->patientName), stdin);
    strtok(patient->patientName, "\n");

    printf("Input age: ");
    scanf("%d", &patient->age);
    getchar();

    printf("Input height (in centimeters): ");
    scanf("%f", &patient->height);
    getchar();

    printf("Input weight (in kilograms): ");
    scanf("%f", &patient->weight);
    getchar();

    printf("Input blood group: ");
    fgets(patient->bloodType, sizeof(patient->bloodType), stdin);
    strtok(patient->bloodType, "\n");

    printf("Input patient contact number: ");
    fgets(patient->patientContact, sizeof(patient->patientContact), stdin);
    strtok(patient->patientContact, "\n");

    printf("Input address: ");
    fgets(patient->patientAddress, sizeof(patient->patientAddress), stdin);
    strtok(patient->patientAddress, "\n");

    printf("Input emergency contact number: ");
    fgets(patient->emergencyContact, sizeof(patient->emergencyContact), stdin);
    strtok(patient->emergencyContact, "\n");
}

void getInfo_patient(patientInfo patients[MAX_PATIENTS])
{
    int numPatients;
    printf("How many patients do you want to add (maximum %d)? ", MAX_PATIENTS);
    scanf("%d", &numPatients);
    getchar();  // Consume newline character left by scanf

    if (numPatients > MAX_PATIENTS)
    {
        printf("You can add a maximum of %d patients.\n", MAX_PATIENTS);
        numPatients = MAX_PATIENTS;
    }

    for (int i = 0; i < numPatients; i++)
    {
        printf("\nInput details for patient %d:\n", i + 1);
        getPatientInfo(&patients[i]);
    }
}

int searchPatient(patientInfo patients[MAX_PATIENTS], const char* name)
{
    for (int i = 0; i < MAX_PATIENTS; i++)
    {
        if (strcmp(patients[i].patientName, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void deletePatient(patientInfo patients[MAX_PATIENTS], const char* name)
{
    int index = searchPatient(patients, name);
    if (index != -1)
    {
        for (int i = index; i < MAX_PATIENTS - 1; i++)
        {
            patients[i] = patients[i + 1];
        }
        memset(&patients[MAX_PATIENTS - 1], 0, sizeof(patientInfo));
        printf("Patient '%s' deleted successfully.\n", name);
    }
    else
    {
        printf("Patient '%s' not found.\n", name);
    }
}

void displayPatient(const patientInfo* patient)
{
    printf("\n╔═══════════════════════════════════╗\n");
    printf("║        Patient Information        ║\n");
    printf("╚═══════════════════════════════════╝\n");
    printf("╔═══════════════════════════════════╗\n");
    printf("║ Name: %-27s ║\n", patient->patientName);
    printf("║ Age: %-28d ║\n", patient->age);
    printf("║ Height: %-25.2f ║\n", patient->height);
    printf("║ Weight: %-25.2f ║\n", patient->weight);
    printf("║ Blood Type: %-22s║\n", patient->bloodType);
    printf("║ Contact Number: %-18s║\n", patient->patientContact);
    printf("║ Address: %-25s║\n", patient->patientAddress);
    printf("║ Emergency Contact: %-15s║\n", patient->emergencyContact);
    printf("╚═══════════════════════════════════╝\n");
}

void addPatient(patientInfo patients[MAX_PATIENTS])
{
    for (int i = 0; i < MAX_PATIENTS; i++)
    {
        if (strlen(patients[i].patientName) == 0)
        {
            printf("Adding new patient:\n");
            getPatientInfo(&patients[i]);
            printf("Patient '%s' added successfully.\n", patients[i].patientName);
            return;
        }
    }
    printf("Cannot add patient: Maximum number of patients reached.\n");
}

void displayMenu()
{
    printf("\n╔═══════════════════════════════════╗\n");
    printf("║           Patient System          ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║ 1. Add patients                   ║\n");
    printf("║ 2. Search patient                 ║\n");
    printf("║ 3. Delete patient                 ║\n");
    printf("║ 4. Display patient information    ║\n");
    printf("║ 5. Exit                           ║\n");
    printf("╚═══════════════════════════════════╝\n");
    printf("Enter your choice: ");
}

int main()
{
    patientInfo patients[MAX_PATIENTS] = {0};
    int choice;

    while (1)
    {
        displayMenu();
        scanf("%d", &choice);
        getchar();  // Consume newline character left by scanf

        switch (choice)
        {
            case 1:
                getInfo_patient(patients);
                break;
            case 2:
            {
                char searchName[MAX_NAME_LENGTH];
                printf("Enter the name of the patient to search: ");
                fgets(searchName, sizeof(searchName), stdin);
                strtok(searchName, "\n");

                int index = searchPatient(patients, searchName);
                if (index != -1)
                {
                    printf("Patient found:\n");
                    displayPatient(&patients[index]);
                }
                else
                {
                    printf("Patient not found.\n");
                }
                break;
            }
            case 3:
            {
                char deleteName[MAX_NAME_LENGTH];
                printf("Enter the name of the patient to delete: ");
                fgets(deleteName, sizeof(deleteName), stdin);
                strtok(deleteName, "\n");

                deletePatient(patients, deleteName);
                break;
            }
            case 4:
            {
                char displayName[MAX_NAME_LENGTH];
                printf("Enter the name of the patient to display: ");
                fgets(displayName, sizeof(displayName), stdin);
                strtok(displayName, "\n");

                int index = searchPatient(patients, displayName);
                if (index != -1)
                {
                    displayPatient(&patients[index]);
                }
                else
                {
                    printf("Patient not found.\n");
                }
                break;
            }
            case 5:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
