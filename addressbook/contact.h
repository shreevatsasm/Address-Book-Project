#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct 
{
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct 
{
    Contact contacts[MAX_CONTACTS];
    int contactCount;
} AddressBook;

void initialize(AddressBook *addressBook);
void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void save(AddressBook *addressBook);

void inputName(char *name);
void inputPhone(char *phone, AddressBook *addressBook, int checkUnique);
void inputEmail(char *email, AddressBook *addressBook, int checkUnique, int currentIndex);
#endif
