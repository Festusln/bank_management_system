#include<stdio.h>
#include<string.h>
#include<sqlite3.h>

#include "cJSON.h"
#include "cJSON_Utils.h"


#include "database.c"
#include "transaction.c"
#include "data.h"


int main()
{
    AccountDetails customer;
    initDB();
    createDBSchema();
    

    while (1)

    {
    AccountDetails customer;
    printf("1: Create a New Account\n");
    printf("2: Deposit Money\n");
    printf("3: Withdraw Money\n");
    printf("4: Check Account Info\n");
    printf("5: Delete Acount\n");

    int choice = 0;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        if (createNewAccount(&customer))
        continue;

        break;
    
    case 2:
        if (depositMoney(&customer))
        continue;

        break;

    case 3:
        if (withdraw_money(&customer))
        continue;

        break;


    case 4:
        
        if (printAccount (&customer))
            continue;

            break;

    case 5:
        if (deleteAccount(&customer))
            continue;


            break;
    
    default:
    printf("Invalid selection\n");
        break;
    }

        break;
    }


    return 0;
}