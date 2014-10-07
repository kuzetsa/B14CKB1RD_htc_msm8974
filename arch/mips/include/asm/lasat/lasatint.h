#ifndef __ASM_LASAT_LASATINT_H
#define __ASM_LASAT_LASATINT_H

#define LASAT_INT_STATUS_REG_100	(KSEG1ADDR(0x1c880000))
#define LASAT_INT_MASK_REG_100		(KSEG1ADDR(0x1c890000))
#define LASATINT_MASK_SHIFT_100		0

#define LASAT_INT_STATUS_REG_200	(KSEG1ADDR(0x1104003c))
#define LASAT_INT_MASK_REG_200		(KSEG1ADDR(0x1104003c))
#define LASATINT_MASK_SHIFT_200		16

#endif 
