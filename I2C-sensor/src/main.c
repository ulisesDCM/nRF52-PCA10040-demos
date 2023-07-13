#include <stdio.h>
#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#define STTS751_TEMP_HIGH_REG		(0x00)
#define STTS751_TEMP_LOW_REG		(0x02)
#define STTS751_CONFIG_REG			(0x03)

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);
#define I2C1_NODE DT_NODELABEL(mysensor)

static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C1_NODE);

int main(void)
{
	if(device_is_ready(dev_i2c.bus)){
		LOG_ERR("I2C bus is not ready");
		return -1;
	}

	uint8_t config[2] = {STTS751_CONFIG_REG, 0x8C};
	int ret;
	ret = i2c_write_dt(&dev_i2c, config, sizeof(config));
	if(ret != 0){
		LOG_ERR("Failed to write in the I2C Bus");
		return -1;
	}

	uint8_t temp_reading[2] = {0};
	uint8_t sensor_regs[2] = {STTS751_TEMP_LOW_REG,STTS751_TEMP_HIGH_REG};
	ret = i2c_write_read_dt(&dev_i2c, &sensor_regs[0],1,&temp_reading[0],1);
	if(ret != 0){
		LOG_ERR("Failed write/read in the I2C Bus");
		return -1;
	}	
	ret = i2c_write_read_dt(&dev_i2c, &sensor_regs[1], 1, &temp_reading[1], 1);
	if(ret != 0){
		LOG_ERR("Failed write/read in the I2C Bus");
		return -1;
	}	

	while(1){
		int temp = ((int)temp_reading[1] * 256 + ((int)temp_reading[0] & 0xF0)) / 16;
		if(temp > 2047)
		{
			temp -= 4096;
		}

		// Convert to engineering units 
		double cTemp = temp * 0.0625;
		double fTemp = cTemp * 1.8 + 32;

		//Print reading to console  
		printk("Temperature in Celsius : %.2f C \n", cTemp);
		printk("Temperature in Fahrenheit : %.2f F \n", fTemp);
	}
	
}
