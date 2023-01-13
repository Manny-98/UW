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

  // fork a child
  if ( ( pid = fork( ) ) < 0 ) {
    perror( "fork error" );
  }
  else if ( pid == 0 ) {
      // I'm a child
      
      if (pipe(fds[0])<0){                  // create a pipe using fds[0]
          perror("pipe error")
      }
      
      if ( ( pid = fork( ) ) < 0 ) {        // fork a grand-child
          perror( "fork error" );
      }
      else  if ( pid == 0 ) {
          // if I'm a grand-child
          
          if (pipe(fds[1])<0){                  // create a pipe using fds[1]
              perror("pipe error");
          }
          if ( ( pid = fork( ) ) < 0 ) {        // fork a great-grand-child
              perror( "fork error" );
          }
          else  if ( pid == 0 ) {
              // if I'm a great-grand-child
              //fds[0] set to write
              dup2(fds[0][1],1);
              close(fds[1][0]);
              close(fds[0][0]);
              close(fds[1][1]);
              execlp("ps","ps","-A",NULL);           // execute "ps"
          }
          else
              dup2(fds[0],0,0);
          close(fds[1][0]);
          close(fds[0,1]);
          
          
          
          
          
          
          
          
          // else if I'm a grand-child
          // execute "grep"
          // else if I'm a child
          // execute "wc"
      }
  }
  else {
    // I'm a parent
    wait( NULL );
    cout << "commands completed" << endl;
  }
}




