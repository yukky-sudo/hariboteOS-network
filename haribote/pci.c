#include "bootpack.h"


#define CONFIG_ADDRESS	0x0cf8
#define CONFIG_DATA	0x0cfc

union pci_config_address {
	unsigned int raw;
	struct __attribute__((packed)) {
		unsigned int reg_addr:8;
		unsigned int func_num:3;
		unsigned int dev_num:5;
		unsigned int bus_num:8;
		unsigned int _reserved:7;
		unsigned int enable_bit:1;
	};
};

unsigned int get_pci_conf_reg(
	unsigned char bus, unsigned char dev, unsigned char func,
	unsigned char reg)
{
	/* CONFIG_ADDRESSを設定 */
	union pci_config_address conf_addr;
	conf_addr.raw = 0;
	conf_addr.bus_num = bus;
	conf_addr.dev_num = dev;
	conf_addr.func_num = func;
	conf_addr.reg_addr = reg;
	conf_addr.enable_bit = 1;
	io_out32(CONFIG_ADDRESS, conf_addr.raw);

	/* CONFIG_DATAを読み出す */
	return io_in32(CONFIG_DATA);
}

void set_pci_conf_reg(unsigned char bus, unsigned char dev, unsigned char func,
		      unsigned char reg, unsigned int val)
{
	/* CONFIG_ADDRESSを設定 */
	union pci_config_address conf_addr;
	conf_addr.raw = 0;
	conf_addr.bus_num = bus;
	conf_addr.dev_num = dev;
	conf_addr.func_num = func;
	conf_addr.reg_addr = reg;
	conf_addr.enable_bit = 1;
	io_out32(CONFIG_ADDRESS, conf_addr.raw);

	/* CONFIG_DATAへvalを書き込む */
	io_out32(CONFIG_DATA, val);
}

//pciポート検索
int find_pci(short dev,short ven)
{
	int b;
	int d;
	for (d=0;d<32;d++)
	{
		for (b=0;b<32;b++)
		{
			int conf_data = get_pci_conf_reg(b,d,0,PCI_CONF_DID_VID);
			if (conf_data & 0x0000ffff == ven)
			{
				//これはベンダーIDと一致している
				if (conf_data >> 16 == dev)
				{
					//ベンダーIDとも一致しているし、デバイスも一致している
					return b || d << 16;	//見つかった
				}
			}
		}
	}
	return -1;		//見つからなかった
}