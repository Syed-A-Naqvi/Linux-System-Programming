#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>


void* transaction(void* args){

    int random = rand();
    int* balance = (int*)args;

    int amount = 10 + random%91;
    int w_or_d = random%2;

    if (w_or_d == 0){

        printf("Withdrawing: $%d\n", amount);
        *balance -= amount;
        printf("Balance after transaction: $%d:\n\n", *balance);

    }
    else {

        printf("Depositing: $%d\n", amount);
        *balance += amount;
        printf("Balance after transaction: $%d\n\n", *balance);

    }

}

int main(int argc, char const *argv[])
{
    int balance = 1000;

    printf("Initial balance is $%d\n\n", balance);

    pthread_t tids[5];

    for (int i = 0; i < 5; i++)
    {
        sleep(1);
        srand(time(NULL));
        printf("Customer %d: ", i+1);
        pthread_create(&tids[i], NULL, transaction, (void*)&balance);
        pthread_join(tids[i],NULL);
    }

    return 0;
}
