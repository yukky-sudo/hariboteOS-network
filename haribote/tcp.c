#include "bootpack.h"
#include <stdio.h>

/*TCP/IP�̃p�P�b�g�ɂ���*/
//https://atmarkit.itmedia.co.jp/ait/articles/0304/04/news001_2.html

int mychecksum(unsigned short int *ptr , int size);


//���M����o�b�t�@ ���M��ip ����ڂ̑��M��(�傫���f�[�^�̏ꍇ) ����ŕ��������͏I��肩 �p�P�b�g�𕪊����邩
unsigned char send_packet(struct ipdata ip)
{
	unsigned int *data;
	unsigned short len;
	unsigned char tomac = 0;
	unsigned char frommac = 0;
	
	//1bytes���Ƃ�.��
	ip.fromip = 0x2cbe1afacd;
	
	data = (unsigned int)ip.buf;
	int i;
	int x;
	
	len = sizeof(data);
	i = 0;
	
	if (ip.MoreFlag == 1)
	{
		i = 2;
	}
	if (ip.DontFlag == 0)
	{
		i = i || 1;
	}
	unsigned char y;
	int s[1024];
	//�f�[�^�����B
	/*
	s = s || tomac;
	s = s || (frommac << 6*8);
	s = s || (ip.ver << (6+6)*8);
	s = s || (ip.heder << (6+6+2+4)*8);
	s = s || (ip.tos << (6+6+2+4+4)*8);
	s = s || (ip.datagram << (6+6+2+4+4+8)*8);
	s = s || (ip.ID << (6+6+4+4+8+16)*8);
	s = s || (i << (6+6+4+4+8+16+16)*8);
	s = s || (ip.Fragment << (6+6+4+4+8+16+16)*8);
	s = s || (ip.TLL << (6+6+4+4+8+16+16+16)*8);
	s = s || (ip.protocol << (6+6+4+4+8+16+16+16+8)*8);
	ip.chksum = mychecksum(*s , 80);
	s = s || (ip.chksum << (6+6+4+4+8+16+16+16+8+8)*8);
	s = s || (ip.fromip << (6+6+4+4+8+16+16+16+8+8+16)*8);
	s = s || (ip.toip << (6+6+4+4+8+16+16+16+8+8+16+32)*8);
	s = s || (data << (6+6+4+4+8+16+16+16+8+8+16+32+32)*8);
	u = send_frame(s, sizeof(s));
	return y;
	*/
	return 1;
}
unsigned short recive_packet(void *d)
{
	char this_ip[4] = {192,168,0,205};
	unsigned char buf[PACKET_BUFFER_SIZE];
	unsigned short len;
	unsigned char i;
	char is_mine;
	len = receive_frame(buf);
	//IP������
	if (buf[128] == this_ip[0] && buf[129] == this_ip[2] && buf[130] == this_ip[2] && buf[131] == this_ip[3])
	{
		is_mine = 1;
	} else {
		//�����̂���Ȃ�����
		return 0;
	}
	// �����̂�����!
	if (is_mine == 1)
	{
		len = len - (i << (4*8))*20;	//���ۂ̃f�[�^�̑傫��
		unsigned char data[len];
		for (i=0;i<len;i++)
		{
			d = buf[len+i];
		}
	}
	return len;
}

// �`�F�b�N�T���̌v�Z
int mychecksum(unsigned short int *ptr , int size)
{
	int index = 0;
	int sum = 0;
	int carry = 0;
	for (index = 0;index<size ; index++)
	{
		sum += *(ptr+index);
		if (sum > 0x0000FFFF)
		{
		         carry = sum >> 16;
			sum = (sum & 0x0000FFFF) + carry;
		}
	}
	return (unsigned short int)~sum;
}
short rand(short x)
{
	return x * x;
}