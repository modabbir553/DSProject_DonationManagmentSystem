#include <stdio.h>
#include <string.h>

#define SIZE 200

int totalFund = 0;

// Structure for person info
typedef struct {
    int phnNumber;
    char name[20];
    char address[50];
    int totalDonation;
} person;

//Structure for person data base
typedef struct {
    person personinfo[SIZE];
    int totalMember;
} personDatabase;

// Structure for donation info
typedef struct {
    int accountNumber;
    int date;
    int month;
    int year;
    int tk;
} Donation;

//Structure for current donation data base
typedef struct {
    Donation donate[SIZE]; // To save last donation info
} donationDataBase;

//Function prototype
void createAccount(personDatabase *pdb);
void Donate(donationDataBase *ddb, personDatabase *pdb);
void accountInfo(donationDataBase *ddb, personDatabase *pdb);
void printMainMenu();
void donerList(personDatabase *pdb);
void loadPersonDatabase(personDatabase *pdb);
void loadDonationDatabase(donationDataBase *ddb);
void loadTotalFund();
void savePersonDatabase(personDatabase *pdb);
void saveDonationDatabase(donationDataBase *ddb);
void saveTotalFund();


int main() {
    personDatabase pdb = {0}; // Declaring a instance to access database
    donationDataBase ddb = {0}; // Declaring a instance to access database

    loadPersonDatabase(&pdb);
    loadDonationDatabase(&ddb);
    loadTotalFund();

    int choice, pass;
    printf("\nPASSWORD: ");
    scanf("%d", &pass);
    while (pass != 553) {
        printf("Invalid Password!\n");
        printf("\nPASSWORD: ");
        scanf("%d", &pass);
    }

    while (1) {
        printMainMenu();
        printf("\nENTER YOUR CHOICE: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case 1:
                createAccount(&pdb);
                break;
            case 2:
                Donate(&ddb, &pdb); 
                break;
            case 3:
                accountInfo(&ddb,&pdb); 
                break;
            case 4:
                if(totalFund == 0){
                    printf("No fund available!\n");
                    break;
                }
                int amount;
                printf("Enter the amount: ");
                scanf("%d",&amount);
                totalFund -= amount;
                printf("Withdraw Successful!\n");
                saveTotalFund(); 
                break;
            case 5:
                printf("Total Fund: %d\n",totalFund);
                break;
            case 6:
                donerList(&pdb);
                break;
            case 7:
                printf("Log Out Successful\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}

void printMainMenu() {
    printf("\nMain Menu:\n");
    printf("1. Create Account\n");
    printf("2. Donate\n");
    printf("3. Account Info\n");
    printf("4. Withdraw\n");
    printf("5. Total Fund\n");
    printf("6. Doner List\n");
    printf("7. Log Out\n");
    printf("..........\n");
}

void createAccount(personDatabase *pdb) {
    if (pdb->totalMember >= SIZE) {
        printf("Error: Cannot add more accounts. Maximum limit reached.\n");
        return;
    }

    char name[20];
    int phnNumber;
    char address[50];

    printf("Name: ");
    scanf(" %[^\n]s", name);
    printf("Phone Number: ");
    scanf("%d", &phnNumber);
    printf("Enter Address: ");
    scanf(" %[^\n]s", address);

    person newAccount = {phnNumber, "", "", 0};
    strcpy(newAccount.name, name);
    strcpy(newAccount.address, address);

    pdb->personinfo[pdb->totalMember] = newAccount;
    pdb->totalMember++;

    savePersonDatabase(pdb);
    printf("\nAccount Successfully Created.\n");
    return;
}

void Donate(donationDataBase *ddb, personDatabase *pdb) {

    int phnNumber, date, month, year, tk;
    int i, found = 0;

    printf("Enter Phone Number: ");
    scanf("%d", &phnNumber);

    for (i = 0; i < pdb->totalMember; i++) {
        if (pdb->personinfo[i].phnNumber == phnNumber) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No account found for this number!\n");
        return;
    }

    printf("Enter Date: ");
    scanf("%d", &date);
    printf("Enter Month: ");
    scanf("%d",&month);
    printf("Enter Year: ");
    scanf("%d",&year);
    printf("Enter amount: ");
    scanf("%d", &tk);

    Donation newDonation = {phnNumber, date, month, year, tk};
    ddb->donate[i] = newDonation;
    pdb->personinfo[i].totalDonation += tk;

    totalFund += tk;

    savePersonDatabase(pdb); 
    saveDonationDatabase(ddb); 
    saveTotalFund();
    printf("\n%d TK. Donation Successfully Recorded!\n", tk);
    return;
}

void accountInfo(donationDataBase *ddb, personDatabase *pdb) {
    int number, found = 0;

    printf("Enter phone number: ");
    scanf("%d", &number);
    printf("\n");

    printf("__Account Info__\n\n");
    for (int i = 0; i < pdb->totalMember; i++) {
        if (pdb->personinfo[i].phnNumber == number) {
            printf("Name: %s\nNumber: 0%d\nAddress: %s\nLast Donation: %d (%d.%d.%d)\nTotal Donation: %d\n",
                pdb->personinfo[i].name,
                pdb->personinfo[i].phnNumber,
                pdb->personinfo[i].address,
                ddb->donate[i].tk,
                ddb->donate[i].date,
                ddb->donate[i].month,
                ddb->donate[i].year,
                pdb->personinfo[i].totalDonation);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No account found for this number!\n");
    }
    return;
}

void donerList(personDatabase *pdb){
    if(pdb->totalMember == 0){
        printf("No member in the list!");
        return;
    }

    int j = 1;
    printf("                  __Doner list__\n\n");
    for(int i = 0; i<pdb->totalMember; i++){
        printf("%d. Name: %s, Number: 0%d, Address: %s\n",
        j,pdb->personinfo[i].name,pdb->personinfo[i].phnNumber,pdb->personinfo[i].address);
        j++;
    }
    return;
}

void loadPersonDatabase(personDatabase *pdb) {
    FILE *file = fopen("personDatabase.txt", "r");
    if (!file) {
        
        printf("Creating personDatabase.txt file...\n");
        file = fopen("personDatabase.txt", "w");
        if(!file) {
            printf("Error creating file: ");
        }
    } else {
        fread(pdb, sizeof(personDatabase), 1, file);
        fclose(file);
    }
}

void loadDonationDatabase(donationDataBase *ddb) {
    FILE *file = fopen("donationDatabase.txt", "r");
    if (!file) {
        
        printf("Creating donationDatabase.txt file...\n");
        file = fopen("donationDatabase.txt", "w");
        if(!file) {
            printf("Error creating file: ");
        }
    } else {
        fread(ddb, sizeof(donationDataBase), 1, file);
        fclose(file);
    }
}

void loadTotalFund() {
    FILE *file = fopen("totalFund.txt", "r");
    if (!file) {
        
        printf("Creating totalFund.txt file...\n");
        file = fopen("totalFund.txt", "w");
        if(!file) {
            printf("Error creating file: ");
        }
    } else {
        fread(&totalFund, sizeof(int), 1, file);
        fclose(file);
    }
}

void savePersonDatabase(personDatabase *pdb) {
    FILE *file = fopen("personDatabase.txt", "w");
    if (file) {
        fwrite(pdb, sizeof(personDatabase), 1, file);
        fclose(file);
    } else {
        printf("Error saving person database file: ");
    }
}

void saveDonationDatabase(donationDataBase *ddb) {
    FILE *file = fopen("donationDatabase.txt", "w");
    if (file) {
        fwrite(ddb, sizeof(donationDataBase), 1, file);
        fclose(file);
    } else {
        printf("Error saving donation database file: ");
    }
}

void saveTotalFund() {
    FILE *file = fopen("totalFund.txt", "w");
    if (file) {
        fwrite(&totalFund, sizeof(int), 1, file);
        fclose(file);
    } else {
        printf("Error saving total fund file: ");
    }
}