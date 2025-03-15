#ifndef SYNC
#define SYNC

typedef enum locks {
  Customer = 1,
  Table = 2,
  Waitstaff = 4,
  Cook = 8,
} Lock;

void take(int locks);
void release(int locks);

#endif // !SYNC
