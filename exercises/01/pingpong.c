#include <stdio.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <unistd.h>

int main() {
  int p2c[2];
  int c2p[2];

  int iter = 1000000;

  int mlen = 1;
  char msg = 'M';
  // char parent_buf[mlen + 1];
  // char child_buf[mlen + 1];

  pipe(p2c);
  pipe(c2p);

  struct timeval start, end;
  gettimeofday(&start, NULL);
  write(p2c[1], &msg, mlen);

  if (fork() == 0) {
    char ch;
    for (int i = 0; i < iter; i++) {
      read(p2c[0], &ch, mlen);
      write(c2p[1], &ch, mlen);
    }
  } else {
    char ch;
    for (int i = 0; i < iter; i++) {
      read(c2p[0], &ch, mlen);
      write(p2c[1], &ch, mlen);
    }

    gettimeofday(&end, NULL);
    double t =
        (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("%d exchanges in %.3f seconds = %.0f exchanges/sec\n", iter, t,
           iter / t);
  }

  return 0;
}
