#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include <ctype.h>

void listContacts(AddressBook *addressBook) 
{
    //printf("+----------+----------------------+-----------------+---------------------------+\n");
    printf("| %-8s | %-20s | %-15s | %-25s |\n", "SL.NO", "NAME", "PHONE NO", "EMAIL ID");
    printf("---------------------------------------------------------------------------------\n");

    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        printf("| %-8d | %-20s | %-15s | %-25s |\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    //printf("---------------------------------------------------------------------------------\n");
}


void trimNewline(char *str) 
{
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) 
    {
        str[--len] = '\0';
    }
}

void initialize(AddressBook *addressBook)
{
    FILE *file = fopen("contact.txt", "r");
    if (file == NULL)
    {
        printf("contact.txt not found.\n");
        addressBook->contactCount = 0;
        return;
    }

    addressBook->contactCount = 0;

    while (fscanf(file, " %49[^,],%14[^,],%49[^\n]",
                  addressBook->contacts[addressBook->contactCount].name,
                  addressBook->contacts[addressBook->contactCount].phone,
                  addressBook->contacts[addressBook->contactCount].email) == 3)
    {
        trimNewline(addressBook->contacts[addressBook->contactCount].name);
        trimNewline(addressBook->contacts[addressBook->contactCount].phone);
        trimNewline(addressBook->contacts[addressBook->contactCount].email);

        addressBook->contactCount++;

        if (addressBook->contactCount >= MAX_CONTACTS)
        {
            printf("Reached max contacts limit. Some contacts may not be loaded.\n");
            break;
        }
    }

    fclose(file);
}



void save(AddressBook *addressBook) 
{
    saveContactsToFile(addressBook); 
    //exit(EXIT_SUCCESS); 
}

void createContact(AddressBook *addressBook)
{
    if (addressBook->contactCount >= MAX_CONTACTS)
    {
        printf("Address book is full. Cannot add more contacts.\n");
        return;
    }

    Contact newContact;
    inputName(newContact.name);
    inputPhone(newContact.phone, addressBook, 1);  
    inputEmail(newContact.email, addressBook, 1, -1);

    addressBook->contacts[addressBook->contactCount] = newContact;
    addressBook->contactCount++;
}

// Function for input name
void inputName(char *name)
{
    int valid = 0;
    while (valid != 1)  
    {
        printf("Enter name: ");
        scanf(" %[^\n]", name);

        if (isalpha(name[0]))
        {
            valid = 1;  
        }
        else
        {
            printf("Invalid name. Name must start with an alphabet. Please re-enter.\n");
        }
    }
}

// Function for input phone
void inputPhone(char *phone, AddressBook *addressBook, int checkUnique)
{
    int valid = 0;
    while (!valid)
    {
        printf("Enter phone number (10 digits): ");
        scanf(" %[^\n]", phone);

        int len = strlen(phone);
        valid = 1;

        // Check length 10
        if (len != 10) 
        {
            printf("Invalid phone number. Must be exactly 10 digits.\n");
            valid = 0;
            continue;
        }

        // Check all are digits
        for (int i = 0; i < len; i++) 
        {
            if (!isdigit(phone[i])) 
            {
                printf("Invalid phone number. Only digits allowed.\n");
                valid = 0;
                break;
            }
        }
        if (!valid) 
            continue;

        // Check uniqueness 
        if (checkUnique)
        {
            for (int i = 0; i < addressBook->contactCount; i++) 
            {
                if (strcmp(addressBook->contacts[i].phone, phone) == 0) 
                {
                    printf("Phone number already exists. Please enter a unique number.\n");
                    valid = 0;
                    break;
                }
            }
        }
    }
}

// Function for email input
void inputEmail(char *email, AddressBook *addressBook, int checkUnique, int currentIndex)
{
    int valid = 0;

    while (!valid)
    {
        printf("Enter email: ");
        scanf(" %49[^\n]", email);

        // 1. Check lowercase
        int isLower = 1;
        for (int i = 0; email[i]; i++)
        {
            if (isalpha(email[i]) && !islower(email[i]))
            {
                isLower = 0;
                break;
            }
        }

        if (!isLower)
        {
            printf("Invalid email. Use only lowercase letters.\n");
            continue;
        }

        // Check one '@'
        int atCount = 0;
        for (int i = 0; email[i]; i++)
            if (email[i] == '@')
                atCount++;

        if (atCount != 1)
        {
            printf("Invalid email. Must contain exactly one '@'.\n");
            continue;
        }

        // Find ".com"
        char *dotCom = strstr(email, ".com");
        if (!dotCom)
        {
            printf("Invalid email. '.com' is missing.\n");
            continue;
        }

        //Ends with ".com"
        if (strcmp(dotCom, ".com") != 0)
        {
            printf("Invalid email. .\n");
            continue;
        }

        //Only one ".com"
        if (strstr(dotCom + 1, ".com") != NULL)
        {
            printf("Invalid email. Multiple '.com' found.\n");
            continue;
        }

        // 6. Uniqueness check 
        if (checkUnique)
        {
            int unique = 1;
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (i == currentIndex)
                    continue;  

                if (strcmp(addressBook->contacts[i].email, email) == 0)
                {
                    unique = 0;
                    break;
                }
            }

            if (!unique)
            {
                printf("Email already exists. Please enter a unique email.\n");
                continue;
            }
        }

        valid = 1;  
    }
}

// Simplified email input for searching
void inputEmailForSearch(char *email)
{
    printf("Enter email: ");
    scanf(" %49[^\n]", email);
    for (int i = 0; email[i]; i++)
    {
        email[i] = tolower((unsigned char)email[i]);
    }
}

// SearchContact function
void searchContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0)
    {
        printf("Address book is empty.\n");
        return;
    }

    int choice;
    char searchValue[50];

    printf("\nSearch by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    int found = 0;

    switch (choice)
    {
        case 1:
            inputName(searchValue);  
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, searchValue) == 0)
                {
                    printf("%-6d %-25s %-15s %-30s\n", i + 1,
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                    found = 1;
                }
            }
            break;

        case 2:
            inputPhone(searchValue, addressBook, 0);  
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].phone, searchValue) == 0)
                {
                    printf("%-6d %-25s %-15s %-30s\n", i + 1,
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                    found = 1;
                    break;  
                }
            }
            break;

        case 3:
            inputEmailForSearch(searchValue); 
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].email, searchValue) == 0)
                {
                    printf("%-6d %-25s %-15s %-30s\n", i + 1,
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                    found = 1;
                    break;  
                }
            }
            break;

        default:
            printf("Invalid choice.\n");
            return;
    }

    if (!found)
    {
        printf("No matching contact found.\n");
    }
}

// Edit contact function
void editContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0)
    {
        printf("Address book is empty.\n");
        return;
    }

    int foundIndex = -1;
    char searchValue[50];
    int searchChoice;

    do 
    {
        printf("\nSearch contact to edit by:\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        printf("Enter your choice: ");
        scanf("%d", &searchChoice);

        if (searchChoice < 1 || searchChoice > 3)
            printf("Invalid choice. Please try again.\n");
    } while (searchChoice < 1 || searchChoice > 3);

    switch (searchChoice)
    {
        case 1:
            inputName(searchValue);
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, searchValue) == 0)
                {
                    foundIndex = i;
                    break;
                }
            }
            break;

        case 2:
            inputPhone(searchValue, addressBook, 0);
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].phone, searchValue) == 0)
                {
                    foundIndex = i;
                    break;
                }
            }
            break;

        case 3:
            inputEmailForSearch(searchValue);  
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].email, searchValue) == 0)
                {
                    foundIndex = i;
                    break;
                }
            }
            break;
    }

    if (foundIndex == -1)
    {
        printf("Contact not found.\n");
        return;
    }

    printf("\nContact found:\n");
    printf("Name : %s\n", addressBook->contacts[foundIndex].name);
    printf("Phone: %s\n", addressBook->contacts[foundIndex].phone);
    printf("Email: %s\n", addressBook->contacts[foundIndex].email);

    int editChoice;
    char temp[50];

    do
    {
        printf("\nWhat do you want to edit?\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        printf("Enter your choice: ");
        scanf("%d", &editChoice);

        switch (editChoice)
        {
            case 1:
                inputName(temp);
                strcpy(addressBook->contacts[foundIndex].name, temp);
                printf("Name updated successfully.\n");
                break;

            case 2:
                inputPhone(temp, addressBook, 1);
                strcpy(addressBook->contacts[foundIndex].phone, temp);
                printf("Phone updated successfully.\n");
                break;

            case 3:
                inputEmail(temp, addressBook, 1, foundIndex); 
                strcpy(addressBook->contacts[foundIndex].email, temp);
                 printf("Email updated successfully.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                editChoice = 0;
        }
    } while (editChoice == 0);

    printf("\nUpdated contact details:\n");
    printf("Name : %s\n", addressBook->contacts[foundIndex].name);
    printf("Phone: %s\n", addressBook->contacts[foundIndex].phone);
    printf("Email: %s\n", addressBook->contacts[foundIndex].email);
}






void deleteContact(AddressBook *addressBook)
{
    int searchChoice;
    char searchValue[50];
    int matchedIndices[MAX_CONTACTS];
    int matchCount = 0;

    printf("\nDelete contact by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &searchChoice);

    // Input search value depending on choice
    switch (searchChoice)
    {
        case 1:
            inputName(searchValue);
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, searchValue) == 0)
                {
                    matchedIndices[matchCount++] = i;
                }
            }
            break;

            
        case 2:
            inputPhone(searchValue, addressBook, 0); // no uniqueness check for searching
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].phone, searchValue) == 0)
                {
                    matchedIndices[matchCount++] = i;
                }
            }
            break;

        case 3:
            inputEmailForSearch(searchValue); 
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].email, searchValue) == 0)
                {
                    matchedIndices[matchCount++] = i;
                }
            }
            break;

        default:
            printf("Invalid choice.\n");
            return;
    }

    if (matchCount == 0)
    {
        printf("No matching contact found.\n");
        return;
    }
    else if (matchCount == 1)
    {
        int index = matchedIndices[0];
        printf("\nContact found:\n");
        printf("Name : %s\n", addressBook->contacts[index].name);
        printf("Phone: %s\n", addressBook->contacts[index].phone);
        printf("Email: %s\n", addressBook->contacts[index].email);

        char confirm;
        printf("Are you sure you want to delete this contact? (y/n): ");
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y')
        {
            for (int i = index; i < addressBook->contactCount - 1; i++)
            {
                addressBook->contacts[i] = addressBook->contacts[i + 1];
            }
            addressBook->contactCount--;
            printf("Contact deleted successfully.\n");
        }
        else
        {
            printf("Deletion cancelled.\n");
        }
    }
    else
    {
        // Multiple matches found
        printf("\nMultiple contacts found:\n");
        for (int i = 0; i < matchCount; i++)
        {
            int idx = matchedIndices[i];
            printf("%d. Name: %s, Phone: %s, Email: %s\n",
                   i + 1,
                   addressBook->contacts[idx].name,
                   addressBook->contacts[idx].phone,
                   addressBook->contacts[idx].email);
        }

        int choice;
        printf("Enter the number of the contact you want to delete (0 to cancel): ");
        scanf("%d", &choice);

        if (choice > 0 && choice <= matchCount)
        {
            int index = matchedIndices[choice - 1];
            char confirm;
            printf("Are you sure you want to delete this contact? (y/n): ");
            scanf(" %c", &confirm);

            if (confirm == 'y' || confirm == 'Y')
            {
                for (int i = index; i < addressBook->contactCount - 1; i++)
                {
                    addressBook->contacts[i] = addressBook->contacts[i + 1];
                }
                addressBook->contactCount--;
                printf("Contact deleted successfully.\n");
            }
            else
            {
                printf("Deletion cancelled.\n");
            }
        }
        else
        {
            printf("Deletion cancelled or invalid choice.\n");
        }
    }
}
