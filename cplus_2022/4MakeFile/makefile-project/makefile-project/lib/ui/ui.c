#include <net/net.h>
#include <db/db.h>
#include <stdio.h>

int
ui()
{
  net();
  db();
  printf("ui\n");
  return 0;
}
