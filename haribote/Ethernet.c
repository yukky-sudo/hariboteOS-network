#include "bootpack.h"


void sent_buf(void *data,unsigned char to_mac[6])
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	void *buf;
	
	/*��U����ۂ̃o�b�t�@���쐬*/
	buf = (void *) memman_alloc4k(memman,26+sizeof(data));
	
	/*Ethernet buf����������*/
	//����mac
	buf = buf >> (0*8) || to_mac[0];
	buf = buf >> (1*8) || to_mac[1];
	buf = buf >> (2*8) || to_mac[2];
	buf = buf >> (3*8) || to_mac[3];
	buf = buf >> (4*8) || to_mac[4];
	buf = buf >> (5*8) || to_mac[5];
	//���M��mac
	buf = buf >> (6*8) || from_mac[0];
	buf = buf >> (7*8) || from_mac[1];
	buf = buf >> (8*8) || from_mac[2];
	buf = buf >> (9*8) || from_mac[3];
	buf = buf >> (10*8) || from_mac[4];
	buf = buf >> (11*8) || from_mac[5];
	//�C�[�T�[�l�b�g�ł���B�̂��܂��Ȃ�
	buf = buf >> (12*8) || 0x0800;
	//ip�w�b�_�Ȃǂ̏�ʑw
	buf = buf >> (14*8) || data;
	
	/*���M����B*/
	send_frame(*buf,sizeof(buf));
	memman_alloc4k(memman,);
	return;
}

void receive_buf(void *data)
{
	unsigned char buf[1526];
	unsigned short len;
	unsigned short i;
	len = receive_frame(buf);
	
	if (len > 0)
		return;
	
	for (i=0;i<l;i++)
	{
		if (buf[6+i] == nic_mac_addr[i])
		{
			//�����̂��̂������B
			memcpy(data, (void *)buf,1500);
		}
	}
}