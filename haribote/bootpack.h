/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; /* ブートセクタはどこまでディスクを読んだのか */
	char leds; /* ブート時のキーボードのLEDの状態 */
	char vmode; /* ビデオモード  何ビットカラーか */
	char reserve;
	short scrnx, scrny; /* 画面解像度 */
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0
#define ADR_DISKIMG		0x00100000

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
void io_out8(int port, int data);
int io_in32(int port);
void io_out32(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
int load_cr0(void);
void store_cr0(int cr0);
void load_tr(int tr);
void asm_inthandler0c(void);
void asm_inthandler0d(void);
void asm_inthandler20(void);
void asm_inthandler21(void);
void asm_inthandler2c(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void farjmp(int eip, int cs);
void farcall(int eip, int cs);
void asm_hrb_api(void);
void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
void asm_end_app(void);
void asm_dos_api(void);

/* fifo.c */
struct FIFO32 {
	int *buf;
	int p, q, size, free, flags;
	struct TASK *task;
};
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task);
int fifo32_put(struct FIFO32 *fifo, int data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);

/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize);
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

/* dsctbl.c */
struct SEGMENT_DESCRIPTOR {
	unsigned short limit_low, base_low;
	unsigned char base_mid, access_right;
	unsigned char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_LDT			0x0082
#define AR_TSS32		0x0089
#define AR_INTGATE32	0x008e

/* int.c */
void init_pic(void);
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

/* keyboard.c */
void inthandler21(int *esp);
void wait_KBC_sendready(void);
void init_keyboard(struct FIFO32 *fifo, int data0);
#define PORT_KEYDAT		0x0060
#define PORT_KEYCMD		0x0064

/* mouse.c */
struct MOUSE_DEC {
	unsigned char buf[3], phase;
	int x, y, btn;
};
void inthandler2c(int *esp);
void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

/* memory.c */
#define MEMMAN_FREES		4090	/* これで約32KB */
#define MEMMAN_ADDR			0x003c0000
struct FREEINFO {	/* あき情報 */
	unsigned int addr, size;
};
struct MEMMAN {		/* メモリ管理 */
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};
unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);

/* sheet.c */
#define MAX_SHEETS		256
struct SHEET {
	unsigned char *buf;
	int bxsize, bysize, vx0, vy0, col_inv, height, flags;
	struct SHTCTL *ctl;
	struct TASK *task;
};
struct SHTCTL {
	unsigned char *vram, *map;
	int xsize, ysize, top;
	struct SHEET *sheets[MAX_SHEETS];
	struct SHEET sheets0[MAX_SHEETS];
};
struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize);
struct SHEET *sheet_alloc(struct SHTCTL *ctl);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv);
void sheet_updown(struct SHEET *sht, int height);
void sheet_refresh(struct SHEET *sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(struct SHEET *sht, int vx0, int vy0);
void sheet_free(struct SHEET *sht);

/* timer.c */
#define MAX_TIMER		500
struct TIMER {
	struct TIMER *next;
	unsigned int timeout;
	char flags, flags2;
	struct FIFO32 *fifo;
	int data;
};
struct TIMERCTL {
	unsigned int count, next;
	struct TIMER *t0;
	struct TIMER timers0[MAX_TIMER];
};
extern struct TIMERCTL timerctl;
void init_pit(void);
struct TIMER *timer_alloc(void);
void timer_free(struct TIMER *timer);
void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data);
void timer_settime(struct TIMER *timer, unsigned int timeout);
void inthandler20(int *esp);
int timer_cancel(struct TIMER *timer);
void timer_cancelall(struct FIFO32 *fifo);

/* mtask.c */
#define MAX_TASKS		1000	/* 最大タスク数 */
#define TASK_GDT0		3		/* TSSをGDTの何番から割り当てるのか */
#define MAX_TASKS_LV	100
#define MAX_TASKLEVELS	10
struct TSS32 {
	int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int es, cs, ss, ds, fs, gs;
	int ldtr, iomap;
};
struct TASK {
	int sel, flags; /* selはGDTの番号のこと */
	int level, priority;
	struct FIFO32 fifo;
	struct TSS32 tss;
	struct SEGMENT_DESCRIPTOR ldt[2];
	struct CONSOLE *cons;
	int ds_base, cons_stack;
	struct FILEHANDLE *fhandle;
	int *fat;
	char *cmdline;
	unsigned char langmode, langbyte1;
};
struct TASKLEVEL {
	int running; /* 動作しているタスクの数 */
	int now; /* 現在動作しているタスクがどれだか分かるようにするための変数 */
	struct TASK *tasks[MAX_TASKS_LV];
};
struct TASKCTL {
	int now_lv; /* 現在動作中のレベル */
	char lv_change; /* 次回タスクスイッチのときに、レベルも変えたほうがいいかどうか */
	struct TASKLEVEL level[MAX_TASKLEVELS];
	struct TASK tasks0[MAX_TASKS];
};
extern struct TASKCTL *taskctl;
extern struct TIMER *task_timer;
struct TASK *task_now(void);
struct TASK *task_init(struct MEMMAN *memman);
struct TASK *task_alloc(void);
void task_run(struct TASK *task, int level, int priority);
void task_switch(void);
void task_sleep(struct TASK *task);

/* window.c */
void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l);
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c);
void make_wtitle8(unsigned char *buf, int xsize, char *title, char act);
void change_wtitle8(struct SHEET *sht, char act);

/* console.c */
struct CONSOLE {
	struct SHEET *sht;
	int cur_x, cur_y, cur_c;
	struct TIMER *timer;
};
struct FILEHANDLE {
	char *buf;
	int size;
	int pos;
};
void console_task(struct SHEET *sheet, int memtotal);
void cons_putchar(struct CONSOLE *cons, int chr, char move);
void cons_newline(struct CONSOLE *cons);
void cons_putstr0(struct CONSOLE *cons, char *s);
void cons_putstr1(struct CONSOLE *cons, char *s, int l);
void cons_runcmd(char *cmdline, struct CONSOLE *cons, int *fat, int memtotal);
void cmd_mem(struct CONSOLE *cons, int memtotal);
void cmd_cls(struct CONSOLE *cons);
void cmd_dir(struct CONSOLE *cons);
void cmd_exit(struct CONSOLE *cons, int *fat);
void cmd_start(struct CONSOLE *cons, char *cmdline, int memtotal);
void cmd_ncst(struct CONSOLE *cons, char *cmdline, int memtotal);
void cmd_langmode(struct CONSOLE *cons, char *cmdline);
void cmd_ip(struct CONSOLE *cons);
int cmd_app(struct CONSOLE *cons, int *fat, char *cmdline);
int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);
int *inthandler0d(int *esp);
int *inthandler0c(int *esp);
void hrb_api_linewin(struct SHEET *sht, int x0, int y0, int x1, int y1, int col);

/* file.c */
struct FILEINFO {
	unsigned char name[8], ext[3], type;
	char reserve[10];
	unsigned short time, date, clustno;
	unsigned int size;
};
void file_readfat(int *fat, unsigned char *img);
void file_loadfile(int clustno, int size, char *buf, int *fat, char *img);
struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max);
char *file_loadfile2(int clustno, int *psize, int *fat);

/* tek.c */
int tek_getsize(unsigned char *p);
int tek_decomp(unsigned char *p, char *q, int size);

/* bootpack.c */
struct TASK *open_constask(struct SHEET *sht, unsigned int memtotal);
struct SHEET *open_console(struct SHTCTL *shtctl, unsigned int memtotal);

//マウス座標取得
int get_mx();
int get_my();

/* pci.c */
#define PCI_CONF_DID_VID	0x00
#define PCI_CONF_STATUS_COMMAND	0x04

#define PCI_COM_IO_EN	(1U << 0)
#define PCI_COM_MEM_EN	(1U << 1)
#define PCI_COM_BUS_MASTER_EN	(1U << 2)
#define PCI_COM_SPECIAL_CYCLE	(1U << 3)
#define PCI_COM_MEMW_INV_EN	(1U << 4)
#define PCI_COM_VGA_PAL_SNP	(1U << 5)
#define PCI_COM_PARITY_ERR_RES	(1U << 6)
#define PCI_COM_SERR_EN	(1U << 8)
#define PCI_COM_FAST_BACK2BACK_EN	(1U << 9)
#define PCI_COM_INTR_DIS	(1U << 10)

#define PCI_STAT_INTR	(1U << 3)
#define PCI_STAT_MULT_FUNC	(1U << 4)
#define PCI_STAT_66MHZ	(1U << 5)
#define PCI_STAT_FAST_BACK2BACK	(1U << 7)
#define PCI_STAT_DATA_PARITY_ERR	(1U << 8)
#define PCI_STAT_DEVSEL_MASK	(3U << 9)
#define PCI_STAT_DEVSEL_FAST	(0b00 << 9)
#define PCI_STAT_DEVSEL_MID	(0b01 << 9)
#define PCI_STAT_DEVSEL_LOW	(0b10 << 9)
#define PCI_STAT_SND_TARGET_ABORT	(1U << 11)
#define PCI_STAT_RCV_TARGET_ABORT	(1U << 12)
#define PCI_STAT_RCV_MASTER_ABORT	(1U << 13)
#define PCI_STAT_SYS_ERR	(1U << 14)
#define PCI_STAT_PARITY_ERR	(1U << 15)

#define PCI_CONF_BAR	0x10

#define PCI_BAR_MASK_IO	0x00000001
#define PCI_BAR_MASK_MEM_TYPE	0x00000006
#define PCI_BAR_MEM_TYPE_32BIT	0x00000000
#define PCI_BAR_MEM_TYPE_1M	0x00000002
#define PCI_BAR_MEM_TYPE_64BIT	0x00000004
#define PCI_BAR_MASK_MEM_PREFETCHABLE	0x00000008
#define PCI_BAR_MASK_MEM_ADDR	0xfffffff0
#define PCI_BAR_MASK_IO_ADDR	0xfffffffc

unsigned int get_pci_conf_reg(unsigned char bus, unsigned char dev, unsigned char func,unsigned char reg);
void set_pci_conf_reg(unsigned char bus, unsigned char dev, unsigned char func,unsigned char reg, unsigned int val);
int find_pci(short dev,short ven);

/* nic.c */

/*対応ベンダID*/
#define INTEL_VEND		0x8086
/*対応デバイスID*/
#define E1000_DEV		0x100E
#define E1000_I217		0x153A
#define E1000_82577LM	0x10EA

#define NIC_BUS_NUM		0x00
#define NIC_DEV_NUM		0x03
#define NIC_FUNC_NUM	0x0

#define NIC_REG_EERD	0x0014
#define NIC_REG_IMS		0x00d0
#define NIC_REG_IMC		0x00d8
#define NIC_REG_RCTL	0x0100
#define NIC_REG_TCTL	0x0400
#define NIC_REG_RDBAL	0x2800
#define NIC_REG_RDBAH	0x2804
#define NIC_REG_RDLEN	0x2808
#define NIC_REG_RDH	0x2810
#define NIC_REG_RDT	0x2818
#define NIC_REG_TDBAL	0x3800
#define NIC_REG_TDBAH	0x3804
#define NIC_REG_TDLEN	0x3808
#define NIC_REG_TDH	0x3810
#define NIC_REG_TDT	0x3818
#define NIC_REG_RAL(n)	(0x5400 + ((n) * 8))
#define NIC_REG_RAH(n)	(0x5404 + ((n) * 8))

#define NIC_EERD_START	(1U << 0)
#define NIC_EERD_DONE	(1U << 4)
#define NIC_EERD_ADDRESS_SHIFT	8
#define NIC_EERD_DATA_SHIFT	16

#define NIC_RCTL_EN	(1U << 1)
#define NIC_RCTL_SBP	(1U << 2)
#define NIC_RCTL_UPE	(1U << 3)
#define NIC_RCTL_MPE	(1U << 4)
#define NIC_RCTL_LPE	(1U << 5)
#define NIC_RCTL_LBM_NO_LOOPBACK	(0b00 << 6)
#define NIC_RCTL_LBM_LOOPBACK	(0b11 << 6)
#define NIC_RCTL_RDMTS_TH_1_2	(0b00 << 8)
#define NIC_RCTL_RDMTS_TH_1_4	(0b01 << 8)
#define NIC_RCTL_RDMTS_TH_1_8	(0b10 << 8)
#define NIC_RCTL_MO_B47_36	(0b00 << 12)
#define NIC_RCTL_MO_B46_35	(0b01 << 12)
#define NIC_RCTL_MO_B45_34	(0b10 << 12)
#define NIC_RCTL_MO_B43_32	(0b11 << 12)
#define NIC_RCTL_BAM	(1U << 15)
#define NIC_RCTL_BSIZE_2048B	(0b00 << 16)
#define NIC_RCTL_BSIZE_1024B	(0b01 << 16)
#define NIC_RCTL_BSIZE_512B	(0b10 << 16)
#define NIC_RCTL_BSIZE_256B	(0b11 << 16)
#define NIC_RCTL_BSIZE_16384B	(0b01 << 16)
#define NIC_RCTL_BSIZE_8192B	(0b10 << 16)
#define NIC_RCTL_BSIZE_4096B	(0b11 << 16)
#define NIC_RCTL_VFE	(1U << 18)
#define NIC_RCTL_CFIEN	(1U << 19)
#define NIC_RCTL_CFI	(1U << 20)
#define NIC_RCTL_DPF	(1U << 22)
#define NIC_RCTL_PMCF	(1U << 23)
#define NIC_RCTL_BSEX	(1U << 25)
#define NIC_RCTL_SECRC	(1U << 26)

#define NIC_TCTL_EN	(1U << 1)
#define NIC_TCTL_PSP	(1U << 3)
#define NIC_TCTL_CT_SHIFT	4
#define NIC_TCTL_COLD_SHIFT	12
#define NIC_TCTL_SWXOFF	(1U << 22)
#define NIC_TCTL_RTLC	(1U << 24)
#define NIC_TCTL_NRTU	(1U << 25)

#define NIC_RDESC_STAT_DD	(1U << 0)
#define NIC_RDESC_STAT_EOP	(1U << 1)
#define NIC_RDESC_STAT_IXSM	(1U << 2)
#define NIC_RDESC_STAT_VP	(1U << 3)
#define NIC_RDESC_STAT_TCPCS	(1U << 5)
#define NIC_RDESC_STAT_IPCS	(1U << 6)
#define NIC_RDESC_STAT_PIF	(1U << 7)

#define NIC_TDESC_CMD_EOP	(1U << 0)
#define NIC_TDESC_CMD_IFCS	(1U << 1)
#define NIC_TDESC_CMD_IC	(1U << 2)
#define NIC_TDESC_CMD_RS	(1U << 3)
#define NIC_TDESC_CMD_RPS	(1U << 4)
#define NIC_TDESC_CMD_DEXT	(1U << 5)
#define NIC_TDESC_CMD_VLE	(1U << 6)
#define NIC_TDESC_CMD_IDE	(1U << 7)

#define NIC_TDESC_STA_DD	(1U << 0)
#define NIC_TDESC_STA_EC	(1U << 1)
#define NIC_TDESC_STA_LC	(1U << 2)
#define NIC_TDESC_STA_TU	(1U << 3)

#define PACKET_BUFFER_SIZE	1024
#define PACKET_RBSIZE_BIT	NIC_RCTL_BSIZE_1024B

extern unsigned char nic_mac_addr[6];

void nic_init(void);
unsigned int get_nic_reg_base(void);
unsigned int get_nic_reg(unsigned short reg);
void set_nic_reg(unsigned short reg, unsigned int val);

/*フレーム受信*/
unsigned short receive_frame(void *buf);
/*フレーム送信*/
unsigned char send_frame(void *buf, unsigned short len);

void get_mac_addr(void);
void get_mac_addr_eeprom(void);
void get_mac_addr_rar(void);
