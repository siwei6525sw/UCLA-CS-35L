#include "randcpuid.h"

#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool put_together(signed long long x, int byte_count) {
  int digit_count = byte_count * 2;
  do {
    if (putchar("0123456789abcdef"[x & 0xf]) < 0) return false;
    x >>= 4;
    digit_count--;
  } while (0 < digit_count);
  return 0 <= putchar('\n');
}

int main(int argc, char **argv)
{
  bool valid  = false;
  long long byte_count;
  if (argc == 2) {
    char *endptr;
    errno = 0;
    byte_count = strtoll(argv[1], &endptr, 10);
    if (errno) perror(argv[1]);
    else valid  = !*endptr && 0 <= byte_count;
  }
  if (!valid) {
    fprintf(stderr, "%s: usage: %s byte_count\n", argv[0], argv[0]);
    return 1;
  }
  if (byte_count == 0)
    return 0;
  unsigned long long(*rand64) (void);

  void* lib;
  if (rdrand_supported())
    {
      lib = dlopen("randlibhw.so", RTLD_NOW);
      if (lib == NULL) {
	printf("Error: cannot open randlibhw.so: %s\n", dlerror());
	exit(1);
      }
      rand64 = dlsym(lib, "hardware_rand64");
      if (dlerror()) {
	printf("Error: cannot find hardware_rand64: %s\n", dlerror());
	exit(1);
      }
    }
  else {
    lib = dlopen("randlibsw.so", RTLD_NOW);
    if (lib == NULL) {
      printf("Error: cannot open randlibsw.so: %s \n", dlerror());
      exit(1);
    }
    rand64 = dlsym(lib, "software_rand64");
    if (dlerror()) {
      printf("Error: cannot find software_rand64: %s\n", dlerror());
      exit(1);
    }
  }
  int wordsize = sizeof rand64();
  int output_errno = 0;
  do {
    unsigned long long x = rand64();
    int outbytes = byte_count < wordsize ? byte_count : wordsize;
    if (!put_together(x, outbytes))
      {
	output_errno = errno;
	break;
      }
    byte_count -= outbytes;
  } while (0 < byte_count);
  if (fclose(stdout) != 0)
    output_errno = errno;
  if (output_errno) {
    errno = output_errno;
    perror("output");
    return 1;
  }
  dlclose(lib);
  return 0;
}
