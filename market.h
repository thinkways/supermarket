#define hashlen 128
#define wdlimit 128
#define FilePath "backup.bin"

struct goods{
	char *id;   
	char *name;
	char *category;
	double cost;
	double price;
	int amount;
	int reserve;
	struct goods *next;
};

struct cart{
	char *id;
	int amount;
};
