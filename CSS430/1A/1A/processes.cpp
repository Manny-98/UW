#include <sys/types.h>   // for fork, wait
#include <sys/wait.h>    // for wait
#include <unistd.h>      // for fork, pipe, dup, close
#include <stdio.h>       // for NULL, perror
#include <stdlib.h>      // for exit
#include <iostream>      // for cout

using namespace std;

int main( int argc, char** argv ) {
    int fds[2][2];
    int pid;
    
    if ( argc != 2 ) {
        cerr << "Usage: processes command" << endl;
        exit( -1 );
    }
    if (pipe(fds[0])<0){                                // create a pipe using fds[0]
        perror("pipe error");
    }
    
    if (pipe(fds[1])<0){                                // create a pipe using fds[1]
        perror("pipe fds[1] error");
    }
    
    if ( ( pid = fork( ) ) < 0 ) {                      // fork a child
        perror( "fork child error" );
    }
    else if ( pid == 0 ) {                              // I'm a child
                                       
        

        
        if ( ( pid = fork( ) ) < 0 ) {                  // fork a grand-child
            perror( "fork grand-child error" );
        }
        else  if ( pid == 0 ) {                         // if I'm a grand-child
            
            if ( ( pid = fork( ) ) < 0 ) {              // fork a great-grand-child
                perror( "fork great-grand-child error" );
            }
            else  if ( pid == 0 ) {                     // if I'm a great-grand-child
                dup2(fds[0][1],1);                      
                close(fds[1][0]);                       // close unused ends
                close(fds[0][0]);
                close(fds[1][1]);
                execlp("ps","ps","-A",NULL);            // execute "ps"
            }
            
            else                                        // else if I'm a grand-child
            dup2(fds[0][0],0);
            dup2(fds[1][1],1);
            close(fds[1][0]);
            close(fds[0][1]);
            execlp("grep","grep",argv[1],NULL);         // execute "grep"
        }
        
        else{                                           // else if I'm a child
            close(fds[0][1]);
            close(fds[0][0]);
            close(fds[1][1]);
            dup2(fds[1][0],0);
            execlp("wc","wc","-l",NULL);                // execute "wc"
        }
        
        
    }
    else {                                              // I'm a parent
       
        close(fds[0][0]);                               // close all pipes
        close(fds[0][1]);
        close(fds[1][0]);
        close(fds[1][1]);
        wait( NULL );                                   // wait for the child process to finish
        cout << "commands completed" << endl;
    }
}




