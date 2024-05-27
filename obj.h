#ifndef OBJ
#define OBJ
/* register */
#define R_UNDEF -1
// #define R_FLAG 0	// FLAG bit
#define R_IP "pc" // reg PC
#define R_BP 10		// Base Address
#define R_JP 30		// Jump Address
#define R_FP 29
#define R_TP 0	// Temp
#define R_GEN 0 // General starting from 0, ending at 30
#define R_NUM 31
#define REG_WIDTH 8				 // 64ARM: 8byte
#define REG_OFFSET_STEP -8 // 64ARM: 8byte

/* frame */
#define FORMAL_OFF REG_WIDTH					/* first formal parameter */
#define OBP_OFF 0											/* dynamic chain */
#define RET_OFF REG_OFFSET_STEP				/* ret address */
#define LOCAL_OFF 2 * REG_OFFSET_STEP /* local var */

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

void asm_static(void);
unsigned int stack_alloc(unsigned int bytes);
void stack_free(unsigned int bytes);
void tac_obj();

#endif