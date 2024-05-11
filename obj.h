#ifndef OBJ
#define OBJ
/* register */
#define R_UNDEF -1
#define R_FLAG 0 // FLAG bit
#define R_IP 1	 // reg PC
#define R_BP 2	 // Base Address
#define R_JP 3	 // Jump Address
#define R_TP 4	 // Temp
#define R_GEN 5	 // General starting from 5, ending at 15
#define R_NUM 16

/* frame */
#define FORMAL_OFF -4 /* first formal parameter */
#define OBP_OFF 0			/* dynamic chain */
#define RET_OFF 4			/* ret address */
#define LOCAL_OFF 8		/* local var */

#define MODIFIED 1
#define UNMODIFIED 0

struct rdesc /* Reg descriptor */
{
	struct sym *var; /* Variable in reg */
	int modified;		 /* If needs spilling */
};

extern int tos; /* top of static */
extern int tof; /* top of frame */
extern int oof; /* offset of formal */
extern int oon; /* offset of next frame */

void tac_obj();

#endif