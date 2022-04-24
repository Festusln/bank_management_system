#include "data.h"
#include <stdlib.h>
#include<time.h>


short generateAccntNo(AccountDetails *trans)
{
    srand(time(0));
    int num = rand();
    trans->id = num;

	return 0;
}


short createNewAccount (AccountDetails *trans)
{
    char fName[20] = {'\0'};
    char lName[20] = {'\0'};
    char mName[20] = {'\0'};

    AccountType accountType = INAVALID_ACCOUNT;
    
    memset(trans, 0x00, sizeof(AccountDetails));

    printf("Enter your first name: ");
    scanf("%s", fName);
    strncpy(trans->firstName, fName, sizeof(trans->firstName));
    
    printf("Enter your last name: ");
    scanf("%s", lName);
    strncpy(trans->lastName, lName, sizeof(trans->lastName));

    printf("Enter your middle name: ");
    scanf("%s", mName);
    strncpy(trans->middleName, mName, sizeof(trans->middleName));

    int selection = 0;
    printf("Select Account Type: \n");
    printf("1: SAVINGS Account\n");
    printf("2: CURRENT Account\n");
    scanf("%d", &selection);
    if (selection == 1)
    accountType = SAVINGS_ACCOUNT;
    else if (selection == 2)
    accountType = CURRENT_ACCOUNT;
    else
    accountType = INAVALID_ACCOUNT;
    
    if (accountType == INAVALID_ACCOUNT)
    {
       printf("Invalid selection\n");
       return -1;
    }

    else
    {
    trans->customerAccountType = accountType;
    }

    if (generateAccntNo(trans))                 //function o generate account id.
    
    {
        printf("Unable to create account\n");
        return -1;
    }

    else
    {
        printf("\nAccount number created successfully\n");
        printf("Your Account number is %d\n", trans->id);
    }

    insertIntoDB(trans);

    return 0;
}




short depositMoney (AccountDetails *trans)
{
    double amount = 0.00;

    memset(trans, 0x00, sizeof(AccountDetails));

    if (selectRecordByAccountID(trans))
    {
        return -1;
    }
    
    printf("Enter your deposit amount: ");
    scanf("%lf", &amount);
    printf("Deposit amount of: %.2lf successful\n\n", amount);

    trans->currentBalance += amount;


    if (updateRecord(trans))
    {
        return -1;
    }



    return 0;
}



short withdraw_money (AccountDetails *trans)
{
    double amount = 0.00;

    memset(trans, 0x00, sizeof(AccountDetails));

    if (selectRecordByAccountID(trans))
    {
        return -1;
    }

    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);
    if (amount > trans->currentBalance)
    {
        printf("Insufficient balance\n\n");
        return -1;
    }else
       
        trans->currentBalance -= amount;
      
    if (updateRecord(trans))
    {
        printf("Error occured\n"); //Printing error message to show error while updating balance on database
        return -1;
    }

    printf("Withdrawal approved\n\n");
      
    return 0;

}


short printAccount(AccountDetails *trans)
{
    if (selectRecordByAccountID(trans))
    {
        return -1;
    }

    //printf("Account Name: %s, %s %s\n", trans->lastName, trans->firstName, trans->middleName);
    printf("Account Type: %s\n", trans->customerAccountType==SAVINGS_ACCOUNT ? "Savings Account":"Current Account");
    printf("Account Balance: %.2f\n", trans->currentBalance);

   return 0;

}


short deleteAccount(AccountDetails *trans)
{
    if (deleteRecord(trans))
    {
        return -1;
    }

    return 0;
}