#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 200
#define MAX_DOCTOR_REMARK 200
#define MAX_PASSWORD_LENGTH 50
#define MAX_USER_NAME 30
#define MAX_PATIENTS 100
#define MAX_DOCTORS 10
#define BLOOD_CHAR_MAX 5
#define MAX_CONTACT_LENGTH 15

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

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

typedef struct doctorInfo
{
    char doctorName[MAX_NAME_LENGTH];
    char doctorAddress[MAX_ADDRESS_LENGTH];
    char specialty[MAX_DOCTOR_REMARK];
    char doctorPass[MAX_PASSWORD_LENGTH];
    int id;
    int experience;
    char doctorContact[MAX_CONTACT_LENGTH];
    char doctorUserName[MAX_USER_NAME];
} doctorInfo;

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

void patientAccountCreation(patientInfo *patient, int *lastAssignedID)
{
    patient->id = ++(*lastAssignedID);

    printf("Assigned unique ID: %d\n", patient->id);

    printf("Input patient username: ");
    fgets(patient->patientUserName, sizeof(patient->patientUserName), stdin);
    strtok(patient->patientUserName, "\n");

    printf("Input patient password: ");
    fgets(patient->patientPass, sizeof(patient->patientPass), stdin);
    strtok(patient->patientPass, "\n");
}

void getInfo_patient(patientInfo patients[MAX_PATIENTS], int *lastAssignedID)
{
    int numPatients;
    printf("How many patients do you want to add (maximum %d)? ", MAX_PATIENTS);
    scanf("%d", &numPatients);
    getchar();

    if (numPatients > MAX_PATIENTS)
    {
        printf("You can add a maximum of %d patients.\n", MAX_PATIENTS);
        numPatients = MAX_PATIENTS;
    }

    for (int i = 0; i < numPatients; i++)
    {
        printf("\nInput details for patient %d:\n", i + 1);
        getPatientInfo(&patients[i]);
        patientAccountCreation(&patients[i], lastAssignedID);
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
    printf("\n%s╔═══════════════════════════════════╗%s\n", CYAN, RESET);
    printf("%s║        Patient Information        ║%s\n", CYAN, RESET);
    printf("%s╚═══════════════════════════════════╝%s\n", CYAN, RESET);
    printf("%s╔═══════════════════════════════════╗%s\n", CYAN, RESET);
    printf("%s║ Name: %-27s ║%s\n", CYAN, patient->patientName, RESET);
    printf("%s║ Age: %-28d ║%s\n", CYAN, patient->age, RESET);
    printf("%s║ Height: %-25.2f ║%s\n", CYAN, patient->height, RESET);
    printf("%s║ Weight: %-25.2f ║%s\n", CYAN, patient->weight, RESET);
    printf("%s║ Blood Type: %-22s║%s\n", CYAN, patient->bloodType, RESET);
    printf("%s║ Contact Number: %-18s║%s\n", CYAN, patient->patientContact, RESET);
    printf("%s║ Address: %-25s║%s\n", CYAN, patient->patientAddress, RESET);
    printf("%s║ Emergency Contact: %-15s║%s\n", CYAN, patient->emergencyContact, RESET);
    printf("%s║ Username: %-23s ║%s\n", CYAN, patient->patientUserName, RESET);
    printf("%s║ Password: %-23s ║%s\n", CYAN, patient->patientPass, RESET);
    printf("%s║ ID: %-29d ║%s\n", CYAN, patient->id, RESET);
    printf("%s╚═══════════════════════════════════╝%s\n", CYAN, RESET);
}

void getDoctorInfo(doctorInfo *doctor)
{
    printf("Input name: ");
    fgets(doctor->doctorName, sizeof(doctor->doctorName), stdin);
    strtok(doctor->doctorName, "\n");

    printf("Input address: ");
    fgets(doctor->doctorAddress, sizeof(doctor->doctorAddress), stdin);
    strtok(doctor->doctorAddress, "\n");

    printf("Input specialty: ");
    fgets(doctor->specialty, sizeof(doctor->specialty), stdin);
    strtok(doctor->specialty, "\n");

    printf("Input years of experience: ");
    scanf("%d", &doctor->experience);
    getchar();

    printf("Input contact number: ");
    fgets(doctor->doctorContact, sizeof(doctor->doctorContact), stdin);
    strtok(doctor->doctorContact, "\n");
}

void doctorAccountCreation(doctorInfo *doctor, int *lastAssignedID)
{
    doctor->id = ++(*lastAssignedID);

    printf("Assigned unique ID: %d\n", doctor->id);

    printf("Input doctor username: ");
    fgets(doctor->doctorUserName, sizeof(doctor->doctorUserName), stdin);
    strtok(doctor->doctorUserName, "\n");

    printf("Input doctor password: ");
    fgets(doctor->doctorPass, sizeof(doctor->doctorPass), stdin);
    strtok(doctor->doctorPass, "\n");
}

void getInfo_doctor(doctorInfo doctors[MAX_DOCTORS], int *lastAssignedID)
{
    int numDoctors;
    printf("How many doctors do you want to add (maximum %d)? ", MAX_DOCTORS);
    scanf("%d", &numDoctors);
    getchar();

    if (numDoctors > MAX_DOCTORS)
    {
        printf("You can add a maximum of %d doctors.\n", MAX_DOCTORS);
        numDoctors = MAX_DOCTORS;
    }

    for (int i = 0; i < numDoctors; i++)
    {
        printf("\nInput details for doctor %d:\n", i + 1);
        getDoctorInfo(&doctors[i]);
        doctorAccountCreation(&doctors[i], lastAssignedID);
    }
}

int searchDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name)
{
    for (int i = 0; i < MAX_DOCTORS; i++)
    {
        if (strcmp(doctors[i].doctorName, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void deleteDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name)
{
    int index = searchDoctor(doctors, name);
    if (index != -1)
    {
        for (int i = index; i < MAX_DOCTORS - 1; i++)
        {
            doctors[i] = doctors[i + 1];
        }
        memset(&doctors[MAX_DOCTORS - 1], 0, sizeof(doctorInfo));
        printf("Doctor '%s' deleted successfully.\n", name);
    }
    else
    {
        printf("Doctor '%s' not found.\n", name);
    }
}

void displayDoctor(const doctorInfo* doctor)
{
    printf("\n%s╔═══════════════════════════════════╗%s\n", GREEN, RESET);
    printf("%s║        Doctor Information         ║%s\n", GREEN, RESET);
    printf("%s╚═══════════════════════════════════╝%s\n", GREEN, RESET);
    printf("%s╔═══════════════════════════════════╗%s\n", GREEN, RESET);
    printf("%s║ Name: %-27s ║%s\n", GREEN, doctor->doctorName, RESET);
    printf("%s║ Address: %-25s║%s\n", GREEN, doctor->doctorAddress, RESET);
    printf("%s║ Specialty: %-23s║%s\n", GREEN, doctor->specialty, RESET);
    printf("%s║ Experience: %-22d║%s\n", GREEN, doctor->experience, RESET);
    printf("%s║ Contact Number: %-18s║%s\n", GREEN, doctor->doctorContact, RESET);
    printf("%s║ Username: %-23s ║%s\n", GREEN, doctor->doctorUserName, RESET);
    printf("%s║ Password: %-23s ║%s\n", GREEN, doctor->doctorPass, RESET);
    printf("%s║ ID: %-29d ║%s\n", GREEN, doctor->id, RESET);
    printf("%s╚═══════════════════════════════════╝%s\n", GREEN, RESET);
}

void displayPatientMenu()
{
    printf("\n%s╔════════════════════════════════════════╗%s\n", YELLOW, RESET);
    printf("%s║              Patient Panel             ║%s\n", YELLOW, RESET);
    printf("%s╠════════════════════════════════════════╣%s\n", YELLOW, RESET);
    printf("%s║ %s1.%s Add patients                        ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("%s║ %s2.%s Search patient                      ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("%s║ %s3.%s Delete patient                      ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("%s║ %s4.%s Display patient information         ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("%s║ %s5.%s Back to main menu                   ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("%s╚════════════════════════════════════════╝%s\n", YELLOW, RESET);
    printf("Enter your choice: ");
}

void displayDoctorMenu()
{
    printf("\n%s╔════════════════════════════════════════╗%s\n", MAGENTA, RESET);
    printf("%s║              Doctor Panel              ║%s\n", MAGENTA, RESET);
    printf("%s╠════════════════════════════════════════╣%s\n", MAGENTA, RESET);
    printf("%s║ %s1.%s Add doctors                         ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("%s║ %s2.%s Search doctor                       ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("%s║ %s3.%s Delete doctor                       ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("%s║ %s4.%s Display doctor information          ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("%s║ %s5.%s Back to main menu                   ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("%s╚════════════════════════════════════════╝%s\n", MAGENTA, RESET);
    printf("Enter your choice: ");
}

void displayMainMenu()
{
    printf("\n%s╔════════════════════════════════════════╗%s\n", BLUE, RESET);
    printf("%s║                 Main Menu              ║%s\n", BLUE, RESET);
    printf("%s╠════════════════════════════════════════╣%s\n", BLUE, RESET);
    printf("%s║ %s1.%s Patient Panel                       ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("%s║ %s2.%s Doctor Panel                        ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("%s║ %s3.%s Exit                                ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("%s╚════════════════════════════════════════╝%s\n", BLUE, RESET);
    printf("Enter your choice: ");
}

int main()
{
    static int lastAssignedPatientID = 0;
    static int lastAssignedDoctorID = 0;
    patientInfo patients[MAX_PATIENTS] = {0};
    doctorInfo doctors[MAX_DOCTORS] = {0};
    int choice;

    while (1)
    {
        displayMainMenu();
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
            {
                int patientChoice;
                while (1)
                {
                    displayPatientMenu();
                    scanf("%d", &patientChoice);
                    getchar();

                    switch (patientChoice)
                    {
                        case 1:
                            getInfo_patient(patients, &lastAssignedPatientID);
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
                            goto mainMenu;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;
            }
            case 2:
            {
                int doctorChoice;
                while (1)
                {
                    displayDoctorMenu();
                    scanf("%d", &doctorChoice);
                    getchar();

                    switch (doctorChoice)
                    {
                        case 1:
                            getInfo_doctor(doctors, &lastAssignedDoctorID);
                            break;
                        case 2:
                        {
                            char searchName[MAX_NAME_LENGTH];
                            printf("Enter the name of the doctor to search: ");
                            fgets(searchName, sizeof(searchName), stdin);
                            strtok(searchName, "\n");

                            int index = searchDoctor(doctors, searchName);
                            if (index != -1)
                            {
                                printf("Doctor found:\n");
                                displayDoctor(&doctors[index]);
                            }
                            else
                            {
                                printf("Doctor not found.\n");
                            }
                            break;
                        }
                        case 3:
                        {
                            char deleteName[MAX_NAME_LENGTH];
                            printf("Enter the name of the doctor to delete: ");
                            fgets(deleteName, sizeof(deleteName), stdin);
                            strtok(deleteName, "\n");

                            deleteDoctor(doctors, deleteName);
                            break;
                        }
                        case 4:
                        {
                            char displayName[MAX_NAME_LENGTH];
                            printf("Enter the name of the doctor to display: ");
                            fgets(displayName, sizeof(displayName), stdin);
                            strtok(displayName, "\n");

                            int index = searchDoctor(doctors, displayName);
                            if (index != -1)
                            {
                                displayDoctor(&doctors[index]);
                            }
                            else
                            {
                                printf("Doctor not found.\n");
                            }
                            break;
                        }
                        case 5:
                            goto mainMenu;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;
            }
            case 3:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
        mainMenu:;
    }

    return 0;
}
