#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *file = fopen("contact.txt", "w");


    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(file, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(file);
}


