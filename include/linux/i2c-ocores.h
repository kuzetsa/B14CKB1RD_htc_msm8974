/*
 * i2c-ocores.h - definitions for the i2c-ocores interface
 *
 * Peter Korsgaard <jacmet@sunsite.dk>
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2.  This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#ifndef _LINUX_I2C_OCORES_H
#define _LINUX_I2C_OCORES_H

struct ocores_i2c_platform_data {
	u32 regstep;   
	u32 clock_khz; 
	u8 num_devices; 
	struct i2c_board_info const *devices; 
};

#endif 
