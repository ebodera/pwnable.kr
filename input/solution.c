#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char* challenge = "/home/ctf/challenges/input/input";

int main(int argc, char** argv) {
  char* argv2[101];
  char* envp2[2];

  // Stage 1 : argv
  for (int i = 0; i < 100; i++) {
    argv2[i] = "";
  }
  argv2[100] = NULL;
  argv2['A'] = "\x00";
  argv2['B'] = "\x20\x0a\x0d";

  // Stage 2 : stdio
  int stdin2 = open("./stdin2", O_RDWR | O_CREAT, 00777);
  write(stdin2, "\x00\x0a\x00\xff", 4);
  int stderr2 = open("./stderr2", O_RDWR | O_CREAT, 00777);
  write(stderr2, "\x00\x0a\x02\xff", 4);

  lseek(stdin2, 0, SEEK_SET);
  lseek(stderr2, 0, SEEK_SET);

  dup2(stdin2, 0);
  dup2(stderr2, 2);

  // Stage 3 : env
  envp2[0] = "\xde\xad\xbe\xef=\xca\xfe\xba\xbe";
  envp2[1] = NULL;

  // Stage 4 : file
  int fp2 = open("./\x0a", O_RDWR | O_CREAT, 00777);
  write(fp2, "\x00\x00\x00\x00", 4);
  close(fp2);

  // Stage 5 : network
  argv2['C'] = "8080";


  execve(challenge, argv2, envp2);
}
