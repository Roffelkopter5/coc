typedef struct SEM SEM;

SEM *semCreate(int initVal);

void semDestroy(SEM *sem);

void P(SEM *sem);

void V(SEM *sem);
