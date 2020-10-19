#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int  main()
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
	   int    BankAccount = 0;
		 int    Turn = 0;
		 int    i;
	
		 srand(time(NULL));   // Initialization, should only be called once
	   ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0){
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
		 ShmPTR = (int *) shmat(ShmID, NULL, 0);
		 if (*ShmPTR == -1){
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
		 ShmPTR[0] = BankAccount;
     ShmPTR[1] = Turn;
  
		 pid = fork();
		 if (pid < 0){ 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } else if (pid > 0){ 
		  for( i = 0 ; i < 25 ; i++ ) {
				sleep(rand() % 5);
				while(ShmPTR[1] != 0) {}
				if (ShmPTR[0] <= 100) {
					int balance = rand() % 100;
					if ((balance % 2) == 0 ) {
						ShmPTR[0] = ShmPTR[0] + balance;
						printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, ShmPTR[0]);
					} else {
						printf("Dear old Dad: Doesn't have any money to give\n");
					}
					ShmPTR[1] = 1;
				} else {
						printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", ShmPTR[0]);
						ShmPTR[1] = 1;
				}
     } 
		} else { 
			for( i = 0 ; i < 25 ; i++ ) {
				sleep(rand() % 5);
				while(ShmPTR[1] != 1) {}
				int balance = rand() % 50;
				printf("Poor Student needs $%d\n", balance);
				if (balance <= ShmPTR[0]) {
          
					ShmPTR[0] = ShmPTR[0] - balance;
					printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, ShmPTR[0]);
				} else {
					printf("Poor Student: Not Enough Cash ($%d)\n", ShmPTR[0] );
				}
				ShmPTR[1] = 0;
			}
		}
}
	
   