#include <stdlib.h>

typedef struct s_sem
{	int count;	//indica se o recurso est� ocupado ou n�o (livre > 0, ocupado <= 0).
	PFILA2 file;	//ponteiro para uma fila de threads bloqueadas no sem�foro.
} csem_t;