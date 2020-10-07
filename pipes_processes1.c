// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe
  
    char fixed_str[] = "howard.edu"; 
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of first pipe
  
        // Write input string and close writing end of first pipe.
        write(fd1[1], input_str, strlen(input_str)+1); 
        
  
        // Wait for child to send a string 
        wait(NULL); 
  
        // Close writing end of first pipe
        close(fd1[1]); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipe 
      
        // Read a string using first pipe 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
  
        // Concatenate a fixed string with it 
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        printf("Concatenated string: %s\n", concat_str);
      
        char second_fixed_str[] = "gobison.org"; 
        char second_input_str[100]; 
        pid_t p_nested; 
      
        printf("Enter a second string to concatenate:");
        scanf("%s", second_input_str); 
        p_nested = fork(); 
      
        if (p_nested < 0) 
        { 
            fprintf(stderr, "fork Failed" ); 
            return 1; 
        } else if (p_nested > 0){
          close(fd2[0]);
          // Write input string and close writing end of second pipe. 
          write(fd2[1], second_input_str, strlen(second_input_str)+1); 

          // Wait for child to send a string 
          wait(NULL); 
          close(fd2[1]); 
          
        } else {
          
          close(fd2[1]);  // Close writing end of second pipe 
          
          // Read a string using second pipe 
          char second_concat_str[100]; 
          read(fd2[0], second_concat_str, 100); 
          
          // Concatenate a fixed string with it 
          int kk = strlen(second_concat_str); 
          int ii; 
          for (ii=0; ii<strlen(second_fixed_str); ii++) 
              second_concat_str[kk++] = second_fixed_str[ii]; 

          second_concat_str[kk] = '\0';   // string ends with '\0' 

          printf("Second concatenated string: %s\n", second_concat_str);
          
          // Close reading end 
          close(fd2[0]); 
        }
      
        // Close reading end 
        close(fd1[0]); 
        exit(0); 
    } 
}