#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char **argv)
{
    procinfo(atoi(argv[1]));
    exit(0);
}
