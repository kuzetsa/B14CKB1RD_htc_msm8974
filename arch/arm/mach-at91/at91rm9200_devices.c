/*
 * arch/arm/mach-at91/at91rm9200_devices.c
 *
 *  Copyright (C) 2005 Thibaut VARENE <varenet@parisc-linux.org>
 *  Copyright (C) 2005 David Brownell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <linux/dma-mapping.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/i2c-gpio.h>

#include <mach/board.h>
#include <mach/at91rm9200.h>
#include <mach/at91rm9200_mc.h>
#include <mach/at91_ramc.h>

#include "generic.h"



#if defined(CONFIG_USB_OHCI_HCD) || defined(CONFIG_USB_OHCI_HCD_MODULE)
static u64 ohci_dmamask = DMA_BIT_MASK(32);
static struct at91_usbh_data usbh_data;

static struct resource usbh_resources[] = {
	[0] = {
		.start	= AT91RM9200_UHP_BASE,
		.end	= AT91RM9200_UHP_BASE + SZ_1M - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_UHP,
		.end	= AT91RM9200_ID_UHP,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_usbh_device = {
	.name		= "at91_ohci",
	.id		= -1,
	.dev		= {
				.dma_mask		= &ohci_dmamask,
				.coherent_dma_mask	= DMA_BIT_MASK(32),
				.platform_data		= &usbh_data,
	},
	.resource	= usbh_resources,
	.num_resources	= ARRAY_SIZE(usbh_resources),
};

void __init at91_add_device_usbh(struct at91_usbh_data *data)
{
	int i;

	if (!data)
		return;

	
	for (i = 0; i < data->ports; i++) {
		if (data->overcurrent_pin[i])
			at91_set_gpio_input(data->overcurrent_pin[i], 1);
	}

	usbh_data = *data;
	platform_device_register(&at91rm9200_usbh_device);
}
#else
void __init at91_add_device_usbh(struct at91_usbh_data *data) {}
#endif



#if defined(CONFIG_USB_AT91) || defined(CONFIG_USB_AT91_MODULE)
static struct at91_udc_data udc_data;

static struct resource udc_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_UDP,
		.end	= AT91RM9200_BASE_UDP + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_UDP,
		.end	= AT91RM9200_ID_UDP,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_udc_device = {
	.name		= "at91_udc",
	.id		= -1,
	.dev		= {
				.platform_data		= &udc_data,
	},
	.resource	= udc_resources,
	.num_resources	= ARRAY_SIZE(udc_resources),
};

void __init at91_add_device_udc(struct at91_udc_data *data)
{
	if (!data)
		return;

	if (gpio_is_valid(data->vbus_pin)) {
		at91_set_gpio_input(data->vbus_pin, 0);
		at91_set_deglitch(data->vbus_pin, 1);
	}
	if (gpio_is_valid(data->pullup_pin))
		at91_set_gpio_output(data->pullup_pin, 0);

	udc_data = *data;
	platform_device_register(&at91rm9200_udc_device);
}
#else
void __init at91_add_device_udc(struct at91_udc_data *data) {}
#endif



#if defined(CONFIG_ARM_AT91_ETHER) || defined(CONFIG_ARM_AT91_ETHER_MODULE)
static u64 eth_dmamask = DMA_BIT_MASK(32);
static struct macb_platform_data eth_data;

static struct resource eth_resources[] = {
	[0] = {
		.start	= AT91_VA_BASE_EMAC,
		.end	= AT91_VA_BASE_EMAC + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_EMAC,
		.end	= AT91RM9200_ID_EMAC,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_eth_device = {
	.name		= "at91_ether",
	.id		= -1,
	.dev		= {
				.dma_mask		= &eth_dmamask,
				.coherent_dma_mask	= DMA_BIT_MASK(32),
				.platform_data		= &eth_data,
	},
	.resource	= eth_resources,
	.num_resources	= ARRAY_SIZE(eth_resources),
};

void __init at91_add_device_eth(struct macb_platform_data *data)
{
	if (!data)
		return;

	if (gpio_is_valid(data->phy_irq_pin)) {
		at91_set_gpio_input(data->phy_irq_pin, 0);
		at91_set_deglitch(data->phy_irq_pin, 1);
	}

	
	at91_set_A_periph(AT91_PIN_PA16, 0);	
	at91_set_A_periph(AT91_PIN_PA15, 0);	
	at91_set_A_periph(AT91_PIN_PA14, 0);	
	at91_set_A_periph(AT91_PIN_PA13, 0);	
	at91_set_A_periph(AT91_PIN_PA12, 0);	
	at91_set_A_periph(AT91_PIN_PA11, 0);	
	at91_set_A_periph(AT91_PIN_PA10, 0);	
	at91_set_A_periph(AT91_PIN_PA9, 0);	
	at91_set_A_periph(AT91_PIN_PA8, 0);	
	at91_set_A_periph(AT91_PIN_PA7, 0);	

	if (!data->is_rmii) {
		at91_set_B_periph(AT91_PIN_PB19, 0);	
		at91_set_B_periph(AT91_PIN_PB18, 0);	
		at91_set_B_periph(AT91_PIN_PB17, 0);	
		at91_set_B_periph(AT91_PIN_PB16, 0);	
		at91_set_B_periph(AT91_PIN_PB15, 0);	
		at91_set_B_periph(AT91_PIN_PB14, 0);	
		at91_set_B_periph(AT91_PIN_PB13, 0);	
		at91_set_B_periph(AT91_PIN_PB12, 0);	
	}

	eth_data = *data;
	platform_device_register(&at91rm9200_eth_device);
}
#else
void __init at91_add_device_eth(struct macb_platform_data *data) {}
#endif



#if defined(CONFIG_AT91_CF) || defined(CONFIG_AT91_CF_MODULE)
static struct at91_cf_data cf_data;

#define CF_BASE		AT91_CHIPSELECT_4

static struct resource cf_resources[] = {
	[0] = {
		.start	= CF_BASE,
		
		.end	= CF_BASE + (0x30000000 - 1),
		.flags	= IORESOURCE_MEM | IORESOURCE_MEM_8AND16BIT,
	},
};

static struct platform_device at91rm9200_cf_device = {
	.name		= "at91_cf",
	.id		= -1,
	.dev		= {
				.platform_data		= &cf_data,
	},
	.resource	= cf_resources,
	.num_resources	= ARRAY_SIZE(cf_resources),
};

void __init at91_add_device_cf(struct at91_cf_data *data)
{
	unsigned int csa;

	if (!data)
		return;

	data->chipselect = 4;		

	
	csa = at91_ramc_read(0, AT91_EBI_CSA);
	at91_ramc_write(0, AT91_EBI_CSA, csa | AT91_EBI_CS4A_SMC_COMPACTFLASH);

	at91_ramc_write(0, AT91_SMC_CSR(4),
				  AT91_SMC_ACSS_STD
				| AT91_SMC_DBW_16
				| AT91_SMC_BAT
				| AT91_SMC_WSEN
				| AT91_SMC_NWS_(32)	
				| AT91_SMC_RWSETUP_(6)	
				| AT91_SMC_RWHOLD_(4)	
	);

	
	if (gpio_is_valid(data->irq_pin)) {
		at91_set_gpio_input(data->irq_pin, 1);
		at91_set_deglitch(data->irq_pin, 1);
	}
	at91_set_gpio_input(data->det_pin, 1);
	at91_set_deglitch(data->det_pin, 1);

	
	if (gpio_is_valid(data->vcc_pin))
		at91_set_gpio_output(data->vcc_pin, 0);
	at91_set_gpio_output(data->rst_pin, 0);

	
	at91_set_A_periph(AT91_PIN_PC9, 0);	
	at91_set_A_periph(AT91_PIN_PC10, 0);	
	at91_set_A_periph(AT91_PIN_PC11, 0);	
	at91_set_A_periph(AT91_PIN_PC12, 0);	

	
	at91_set_A_periph(AT91_PIN_PC6, 1);	

	cf_data = *data;
	platform_device_register(&at91rm9200_cf_device);
}
#else
void __init at91_add_device_cf(struct at91_cf_data *data) {}
#endif



#if defined(CONFIG_MMC_AT91) || defined(CONFIG_MMC_AT91_MODULE)
static u64 mmc_dmamask = DMA_BIT_MASK(32);
static struct at91_mmc_data mmc_data;

static struct resource mmc_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_MCI,
		.end	= AT91RM9200_BASE_MCI + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_MCI,
		.end	= AT91RM9200_ID_MCI,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_mmc_device = {
	.name		= "at91_mci",
	.id		= -1,
	.dev		= {
				.dma_mask		= &mmc_dmamask,
				.coherent_dma_mask	= DMA_BIT_MASK(32),
				.platform_data		= &mmc_data,
	},
	.resource	= mmc_resources,
	.num_resources	= ARRAY_SIZE(mmc_resources),
};

void __init at91_add_device_mmc(short mmc_id, struct at91_mmc_data *data)
{
	if (!data)
		return;

	
	if (gpio_is_valid(data->det_pin)) {
		at91_set_gpio_input(data->det_pin, 1);
		at91_set_deglitch(data->det_pin, 1);
	}
	if (gpio_is_valid(data->wp_pin))
		at91_set_gpio_input(data->wp_pin, 1);
	if (gpio_is_valid(data->vcc_pin))
		at91_set_gpio_output(data->vcc_pin, 0);

	
	at91_set_A_periph(AT91_PIN_PA27, 0);

	if (data->slot_b) {
		
		at91_set_B_periph(AT91_PIN_PA8, 1);

		
		at91_set_B_periph(AT91_PIN_PA9, 1);
		if (data->wire4) {
			at91_set_B_periph(AT91_PIN_PA10, 1);
			at91_set_B_periph(AT91_PIN_PA11, 1);
			at91_set_B_periph(AT91_PIN_PA12, 1);
		}
	} else {
		
		at91_set_A_periph(AT91_PIN_PA28, 1);

		
		at91_set_A_periph(AT91_PIN_PA29, 1);
		if (data->wire4) {
			at91_set_B_periph(AT91_PIN_PB3, 1);
			at91_set_B_periph(AT91_PIN_PB4, 1);
			at91_set_B_periph(AT91_PIN_PB5, 1);
		}
	}

	mmc_data = *data;
	platform_device_register(&at91rm9200_mmc_device);
}
#else
void __init at91_add_device_mmc(short mmc_id, struct at91_mmc_data *data) {}
#endif



#if defined(CONFIG_MTD_NAND_ATMEL) || defined(CONFIG_MTD_NAND_ATMEL_MODULE)
static struct atmel_nand_data nand_data;

#define NAND_BASE	AT91_CHIPSELECT_3

static struct resource nand_resources[] = {
	{
		.start	= NAND_BASE,
		.end	= NAND_BASE + SZ_256M - 1,
		.flags	= IORESOURCE_MEM,
	}
};

static struct platform_device at91rm9200_nand_device = {
	.name		= "atmel_nand",
	.id		= -1,
	.dev		= {
				.platform_data	= &nand_data,
	},
	.resource	= nand_resources,
	.num_resources	= ARRAY_SIZE(nand_resources),
};

void __init at91_add_device_nand(struct atmel_nand_data *data)
{
	unsigned int csa;

	if (!data)
		return;

	
	csa = at91_ramc_read(0, AT91_EBI_CSA);
	at91_ramc_write(0, AT91_EBI_CSA, csa | AT91_EBI_CS3A_SMC_SMARTMEDIA);

	
	at91_ramc_write(0, AT91_SMC_CSR(3), AT91_SMC_ACSS_STD | AT91_SMC_DBW_8 | AT91_SMC_WSEN
		| AT91_SMC_NWS_(5)
		| AT91_SMC_TDF_(1)
		| AT91_SMC_RWSETUP_(0)	
		| AT91_SMC_RWHOLD_(1)	
	);

	
	if (gpio_is_valid(data->enable_pin))
		at91_set_gpio_output(data->enable_pin, 1);

	
	if (gpio_is_valid(data->rdy_pin))
		at91_set_gpio_input(data->rdy_pin, 1);

	
	if (gpio_is_valid(data->det_pin))
		at91_set_gpio_input(data->det_pin, 1);

	at91_set_A_periph(AT91_PIN_PC1, 0);		
	at91_set_A_periph(AT91_PIN_PC3, 0);		

	nand_data = *data;
	platform_device_register(&at91rm9200_nand_device);
}
#else
void __init at91_add_device_nand(struct atmel_nand_data *data) {}
#endif



#if defined(CONFIG_I2C_GPIO) || defined(CONFIG_I2C_GPIO_MODULE)

static struct i2c_gpio_platform_data pdata = {
	.sda_pin		= AT91_PIN_PA25,
	.sda_is_open_drain	= 1,
	.scl_pin		= AT91_PIN_PA26,
	.scl_is_open_drain	= 1,
	.udelay			= 2,		
};

static struct platform_device at91rm9200_twi_device = {
	.name			= "i2c-gpio",
	.id			= 0,
	.dev.platform_data	= &pdata,
};

void __init at91_add_device_i2c(struct i2c_board_info *devices, int nr_devices)
{
	at91_set_GPIO_periph(AT91_PIN_PA25, 1);		
	at91_set_multi_drive(AT91_PIN_PA25, 1);

	at91_set_GPIO_periph(AT91_PIN_PA26, 1);		
	at91_set_multi_drive(AT91_PIN_PA26, 1);

	i2c_register_board_info(0, devices, nr_devices);
	platform_device_register(&at91rm9200_twi_device);
}

#elif defined(CONFIG_I2C_AT91) || defined(CONFIG_I2C_AT91_MODULE)

static struct resource twi_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_TWI,
		.end	= AT91RM9200_BASE_TWI + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_TWI,
		.end	= AT91RM9200_ID_TWI,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_twi_device = {
	.name		= "at91_i2c",
	.id		= -1,
	.resource	= twi_resources,
	.num_resources	= ARRAY_SIZE(twi_resources),
};

void __init at91_add_device_i2c(struct i2c_board_info *devices, int nr_devices)
{
	
	at91_set_A_periph(AT91_PIN_PA25, 0);		
	at91_set_multi_drive(AT91_PIN_PA25, 1);

	at91_set_A_periph(AT91_PIN_PA26, 0);		
	at91_set_multi_drive(AT91_PIN_PA26, 1);

	i2c_register_board_info(0, devices, nr_devices);
	platform_device_register(&at91rm9200_twi_device);
}
#else
void __init at91_add_device_i2c(struct i2c_board_info *devices, int nr_devices) {}
#endif



#if defined(CONFIG_SPI_ATMEL) || defined(CONFIG_SPI_ATMEL_MODULE)
static u64 spi_dmamask = DMA_BIT_MASK(32);

static struct resource spi_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_SPI,
		.end	= AT91RM9200_BASE_SPI + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_SPI,
		.end	= AT91RM9200_ID_SPI,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_spi_device = {
	.name		= "atmel_spi",
	.id		= 0,
	.dev		= {
				.dma_mask		= &spi_dmamask,
				.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
	.resource	= spi_resources,
	.num_resources	= ARRAY_SIZE(spi_resources),
};

static const unsigned spi_standard_cs[4] = { AT91_PIN_PA3, AT91_PIN_PA4, AT91_PIN_PA5, AT91_PIN_PA6 };

void __init at91_add_device_spi(struct spi_board_info *devices, int nr_devices)
{
	int i;
	unsigned long cs_pin;

	at91_set_A_periph(AT91_PIN_PA0, 0);	
	at91_set_A_periph(AT91_PIN_PA1, 0);	
	at91_set_A_periph(AT91_PIN_PA2, 0);	

	
	for (i = 0; i < nr_devices; i++) {
		if (devices[i].controller_data)
			cs_pin = (unsigned long) devices[i].controller_data;
		else
			cs_pin = spi_standard_cs[devices[i].chip_select];

		if (devices[i].chip_select == 0)	
			at91_set_A_periph(cs_pin, 0);
		else
			at91_set_gpio_output(cs_pin, 1);


		
		devices[i].controller_data = (void *) cs_pin;
	}

	spi_register_board_info(devices, nr_devices);
	platform_device_register(&at91rm9200_spi_device);
}
#else
void __init at91_add_device_spi(struct spi_board_info *devices, int nr_devices) {}
#endif



#ifdef CONFIG_ATMEL_TCLIB

static struct resource tcb0_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_TCB0,
		.end	= AT91RM9200_BASE_TCB0 + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_TC0,
		.end	= AT91RM9200_ID_TC0,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= AT91RM9200_ID_TC1,
		.end	= AT91RM9200_ID_TC1,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= AT91RM9200_ID_TC2,
		.end	= AT91RM9200_ID_TC2,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_tcb0_device = {
	.name		= "atmel_tcb",
	.id		= 0,
	.resource	= tcb0_resources,
	.num_resources	= ARRAY_SIZE(tcb0_resources),
};

static struct resource tcb1_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_TCB1,
		.end	= AT91RM9200_BASE_TCB1 + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_TC3,
		.end	= AT91RM9200_ID_TC3,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= AT91RM9200_ID_TC4,
		.end	= AT91RM9200_ID_TC4,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		.start	= AT91RM9200_ID_TC5,
		.end	= AT91RM9200_ID_TC5,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_tcb1_device = {
	.name		= "atmel_tcb",
	.id		= 1,
	.resource	= tcb1_resources,
	.num_resources	= ARRAY_SIZE(tcb1_resources),
};

static void __init at91_add_device_tc(void)
{
	platform_device_register(&at91rm9200_tcb0_device);
	platform_device_register(&at91rm9200_tcb1_device);
}
#else
static void __init at91_add_device_tc(void) { }
#endif



#if defined(CONFIG_RTC_DRV_AT91RM9200) || defined(CONFIG_RTC_DRV_AT91RM9200_MODULE)
static struct resource rtc_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_RTC,
		.end	= AT91RM9200_BASE_RTC + SZ_256 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91_ID_SYS,
		.end	= AT91_ID_SYS,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_rtc_device = {
	.name		= "at91_rtc",
	.id		= -1,
	.resource	= rtc_resources,
	.num_resources	= ARRAY_SIZE(rtc_resources),
};

static void __init at91_add_device_rtc(void)
{
	platform_device_register(&at91rm9200_rtc_device);
}
#else
static void __init at91_add_device_rtc(void) {}
#endif



#if defined(CONFIG_AT91RM9200_WATCHDOG) || defined(CONFIG_AT91RM9200_WATCHDOG_MODULE)
static struct platform_device at91rm9200_wdt_device = {
	.name		= "at91_wdt",
	.id		= -1,
	.num_resources	= 0,
};

static void __init at91_add_device_watchdog(void)
{
	platform_device_register(&at91rm9200_wdt_device);
}
#else
static void __init at91_add_device_watchdog(void) {}
#endif



#if defined(CONFIG_ATMEL_SSC) || defined(CONFIG_ATMEL_SSC_MODULE)
static u64 ssc0_dmamask = DMA_BIT_MASK(32);

static struct resource ssc0_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_SSC0,
		.end	= AT91RM9200_BASE_SSC0 + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_SSC0,
		.end	= AT91RM9200_ID_SSC0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_ssc0_device = {
	.name	= "ssc",
	.id	= 0,
	.dev	= {
		.dma_mask		= &ssc0_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
	.resource	= ssc0_resources,
	.num_resources	= ARRAY_SIZE(ssc0_resources),
};

static inline void configure_ssc0_pins(unsigned pins)
{
	if (pins & ATMEL_SSC_TF)
		at91_set_A_periph(AT91_PIN_PB0, 1);
	if (pins & ATMEL_SSC_TK)
		at91_set_A_periph(AT91_PIN_PB1, 1);
	if (pins & ATMEL_SSC_TD)
		at91_set_A_periph(AT91_PIN_PB2, 1);
	if (pins & ATMEL_SSC_RD)
		at91_set_A_periph(AT91_PIN_PB3, 1);
	if (pins & ATMEL_SSC_RK)
		at91_set_A_periph(AT91_PIN_PB4, 1);
	if (pins & ATMEL_SSC_RF)
		at91_set_A_periph(AT91_PIN_PB5, 1);
}

static u64 ssc1_dmamask = DMA_BIT_MASK(32);

static struct resource ssc1_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_SSC1,
		.end	= AT91RM9200_BASE_SSC1 + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_SSC1,
		.end	= AT91RM9200_ID_SSC1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_ssc1_device = {
	.name	= "ssc",
	.id	= 1,
	.dev	= {
		.dma_mask		= &ssc1_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
	.resource	= ssc1_resources,
	.num_resources	= ARRAY_SIZE(ssc1_resources),
};

static inline void configure_ssc1_pins(unsigned pins)
{
	if (pins & ATMEL_SSC_TF)
		at91_set_A_periph(AT91_PIN_PB6, 1);
	if (pins & ATMEL_SSC_TK)
		at91_set_A_periph(AT91_PIN_PB7, 1);
	if (pins & ATMEL_SSC_TD)
		at91_set_A_periph(AT91_PIN_PB8, 1);
	if (pins & ATMEL_SSC_RD)
		at91_set_A_periph(AT91_PIN_PB9, 1);
	if (pins & ATMEL_SSC_RK)
		at91_set_A_periph(AT91_PIN_PB10, 1);
	if (pins & ATMEL_SSC_RF)
		at91_set_A_periph(AT91_PIN_PB11, 1);
}

static u64 ssc2_dmamask = DMA_BIT_MASK(32);

static struct resource ssc2_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_SSC2,
		.end	= AT91RM9200_BASE_SSC2 + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_SSC2,
		.end	= AT91RM9200_ID_SSC2,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device at91rm9200_ssc2_device = {
	.name	= "ssc",
	.id	= 2,
	.dev	= {
		.dma_mask		= &ssc2_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	},
	.resource	= ssc2_resources,
	.num_resources	= ARRAY_SIZE(ssc2_resources),
};

static inline void configure_ssc2_pins(unsigned pins)
{
	if (pins & ATMEL_SSC_TF)
		at91_set_A_periph(AT91_PIN_PB12, 1);
	if (pins & ATMEL_SSC_TK)
		at91_set_A_periph(AT91_PIN_PB13, 1);
	if (pins & ATMEL_SSC_TD)
		at91_set_A_periph(AT91_PIN_PB14, 1);
	if (pins & ATMEL_SSC_RD)
		at91_set_A_periph(AT91_PIN_PB15, 1);
	if (pins & ATMEL_SSC_RK)
		at91_set_A_periph(AT91_PIN_PB16, 1);
	if (pins & ATMEL_SSC_RF)
		at91_set_A_periph(AT91_PIN_PB17, 1);
}

void __init at91_add_device_ssc(unsigned id, unsigned pins)
{
	struct platform_device *pdev;

	switch (id) {
	case AT91RM9200_ID_SSC0:
		pdev = &at91rm9200_ssc0_device;
		configure_ssc0_pins(pins);
		break;
	case AT91RM9200_ID_SSC1:
		pdev = &at91rm9200_ssc1_device;
		configure_ssc1_pins(pins);
		break;
	case AT91RM9200_ID_SSC2:
		pdev = &at91rm9200_ssc2_device;
		configure_ssc2_pins(pins);
		break;
	default:
		return;
	}

	platform_device_register(pdev);
}

#else
void __init at91_add_device_ssc(unsigned id, unsigned pins) {}
#endif



#if defined(CONFIG_SERIAL_ATMEL)
static struct resource dbgu_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_DBGU,
		.end	= AT91RM9200_BASE_DBGU + SZ_512 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91_ID_SYS,
		.end	= AT91_ID_SYS,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct atmel_uart_data dbgu_data = {
	.use_dma_tx	= 0,
	.use_dma_rx	= 0,		
};

static u64 dbgu_dmamask = DMA_BIT_MASK(32);

static struct platform_device at91rm9200_dbgu_device = {
	.name		= "atmel_usart",
	.id		= 0,
	.dev		= {
				.dma_mask		= &dbgu_dmamask,
				.coherent_dma_mask	= DMA_BIT_MASK(32),
				.platform_data		= &dbgu_data,
	},
	.resource	= dbgu_resources,
	.num_resources	= ARRAY_SIZE(dbgu_resources),
};

static inline void configure_dbgu_pins(void)
{
	at91_set_A_periph(AT91_PIN_PA30, 0);		
	at91_set_A_periph(AT91_PIN_PA31, 1);		
}

static struct resource uart0_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_US0,
		.end	= AT91RM9200_BASE_US0 + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_US0,
		.end	= AT91RM9200_ID_US0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct atmel_uart_data uart0_data = {
	.use_dma_tx	= 1,
	.use_dma_rx	= 1,
};

static u64 uart0_dmamask = DMA_BIT_MASK(32);

static struct platform_device at91rm9200_uart0_device = {
	.name		= "atmel_usart",
	.id		= 1,
	.dev		= {
				.dma_mask		= &uart0_dmamask,
				.coherent_dma_mask	= DMA_BIT_MASK(32),
				.platform_data		= &uart0_data,
	},
	.resource	= uart0_resources,
	.num_resources	= ARRAY_SIZE(uart0_resources),
};

static inline void configure_usart0_pins(unsigned pins)
{
	at91_set_A_periph(AT91_PIN_PA17, 1);		
	at91_set_A_periph(AT91_PIN_PA18, 0);		

	if (pins & ATMEL_UART_CTS)
		at91_set_A_periph(AT91_PIN_PA20, 0);	

	if (pins & ATMEL_UART_RTS) {
		at91_set_gpio_output(AT91_PIN_PA21, 1);
	}
}

static struct resource uart1_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_US1,
		.end	= AT91RM9200_BASE_US1 + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_US1,
		.end	= AT91RM9200_ID_US1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct atmel_uart_data uart1_data = {
	.use_dma_tx	= 1,
	.use_dma_rx	= 1,
};

static u64 uart1_dmamask = DMA_BIT_MASK(32);

static struct platform_device at91rm9200_uart1_device = {
	.name		= "atmel_usart",
	.id		= 2,
	.dev		= {
				.dma_mask		= &uart1_dmamask,
				.coherent_dma_mask	= DMA_BIT_MASK(32),
				.platform_data		= &uart1_data,
	},
	.resource	= uart1_resources,
	.num_resources	= ARRAY_SIZE(uart1_resources),
};

static inline void configure_usart1_pins(unsigned pins)
{
	at91_set_A_periph(AT91_PIN_PB20, 1);		
	at91_set_A_periph(AT91_PIN_PB21, 0);		

	if (pins & ATMEL_UART_RI)
		at91_set_A_periph(AT91_PIN_PB18, 0);	
	if (pins & ATMEL_UART_DTR)
		at91_set_A_periph(AT91_PIN_PB19, 0);	
	if (pins & ATMEL_UART_DCD)
		at91_set_A_periph(AT91_PIN_PB23, 0);	
	if (pins & ATMEL_UART_CTS)
		at91_set_A_periph(AT91_PIN_PB24, 0);	
	if (pins & ATMEL_UART_DSR)
		at91_set_A_periph(AT91_PIN_PB25, 0);	
	if (pins & ATMEL_UART_RTS)
		at91_set_A_periph(AT91_PIN_PB26, 0);	
}

static struct resource uart2_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_US2,
		.end	= AT91RM9200_BASE_US2 + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_US2,
		.end	= AT91RM9200_ID_US2,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct atmel_uart_data uart2_data = {
	.use_dma_tx	= 1,
	.use_dma_rx	= 1,
};

static u64 uart2_dmamask = DMA_BIT_MASK(32);

static struct platform_device at91rm9200_uart2_device = {
	.name		= "atmel_usart",
	.id		= 3,
	.dev		= {
				.dma_mask		= &uart2_dmamask,
				.coherent_dma_mask	= DMA_BIT_MASK(32),
				.platform_data		= &uart2_data,
	},
	.resource	= uart2_resources,
	.num_resources	= ARRAY_SIZE(uart2_resources),
};

static inline void configure_usart2_pins(unsigned pins)
{
	at91_set_A_periph(AT91_PIN_PA22, 0);		
	at91_set_A_periph(AT91_PIN_PA23, 1);		

	if (pins & ATMEL_UART_CTS)
		at91_set_B_periph(AT91_PIN_PA30, 0);	
	if (pins & ATMEL_UART_RTS)
		at91_set_B_periph(AT91_PIN_PA31, 0);	
}

static struct resource uart3_resources[] = {
	[0] = {
		.start	= AT91RM9200_BASE_US3,
		.end	= AT91RM9200_BASE_US3 + SZ_16K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= AT91RM9200_ID_US3,
		.end	= AT91RM9200_ID_US3,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct atmel_uart_data uart3_data = {
	.use_dma_tx	= 1,
	.use_dma_rx	= 1,
};

static u64 uart3_dmamask = DMA_BIT_MASK(32);

static struct platform_device at91rm9200_uart3_device = {
	.name		= "atmel_usart",
	.id		= 4,
	.dev		= {
				.dma_mask		= &uart3_dmamask,
				.coherent_dma_mask	= DMA_BIT_MASK(32),
				.platform_data		= &uart3_data,
	},
	.resource	= uart3_resources,
	.num_resources	= ARRAY_SIZE(uart3_resources),
};

static inline void configure_usart3_pins(unsigned pins)
{
	at91_set_B_periph(AT91_PIN_PA5, 1);		
	at91_set_B_periph(AT91_PIN_PA6, 0);		

	if (pins & ATMEL_UART_CTS)
		at91_set_B_periph(AT91_PIN_PB1, 0);	
	if (pins & ATMEL_UART_RTS)
		at91_set_B_periph(AT91_PIN_PB0, 0);	
}

static struct platform_device *__initdata at91_uarts[ATMEL_MAX_UART];	

void __init at91_register_uart(unsigned id, unsigned portnr, unsigned pins)
{
	struct platform_device *pdev;
	struct atmel_uart_data *pdata;

	switch (id) {
		case 0:		
			pdev = &at91rm9200_dbgu_device;
			configure_dbgu_pins();
			break;
		case AT91RM9200_ID_US0:
			pdev = &at91rm9200_uart0_device;
			configure_usart0_pins(pins);
			break;
		case AT91RM9200_ID_US1:
			pdev = &at91rm9200_uart1_device;
			configure_usart1_pins(pins);
			break;
		case AT91RM9200_ID_US2:
			pdev = &at91rm9200_uart2_device;
			configure_usart2_pins(pins);
			break;
		case AT91RM9200_ID_US3:
			pdev = &at91rm9200_uart3_device;
			configure_usart3_pins(pins);
			break;
		default:
			return;
	}
	pdata = pdev->dev.platform_data;
	pdata->num = portnr;		

	if (portnr < ATMEL_MAX_UART)
		at91_uarts[portnr] = pdev;
}

void __init at91_set_serial_console(unsigned portnr)
{
	if (portnr < ATMEL_MAX_UART) {
		atmel_default_console_device = at91_uarts[portnr];
		at91rm9200_set_console_clock(at91_uarts[portnr]->id);
	}
}

void __init at91_add_device_serial(void)
{
	int i;

	for (i = 0; i < ATMEL_MAX_UART; i++) {
		if (at91_uarts[i])
			platform_device_register(at91_uarts[i]);
	}

	if (!atmel_default_console_device)
		printk(KERN_INFO "AT91: No default serial console defined.\n");
}
#else
void __init at91_register_uart(unsigned id, unsigned portnr, unsigned pins) {}
void __init at91_set_serial_console(unsigned portnr) {}
void __init at91_add_device_serial(void) {}
#endif



static int __init at91_add_standard_devices(void)
{
	at91_add_device_rtc();
	at91_add_device_watchdog();
	at91_add_device_tc();
	return 0;
}

arch_initcall(at91_add_standard_devices);
