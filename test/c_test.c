#include <stdio.h>
#include <unistd.h>
typedef unsigned char Byte;//8 bit
typedef signed char SByte;
typedef signed short int Short; //16 bit
typedef unsigned short int UShort;
typedef signed int Int; //32 bit
typedef unsigned int UInt;
typedef signed long long int Long; //64 bit
typedef unsigned long long int ULong;

#define INHERITS(BASECLASS) struct BASECLASS Base
#define OPERATIONS(OPS_STRUCT) struct OPS_STRUCT OPS_STRUCT

typedef Int (*iofunc_t)(void*, Byte*, Int);
struct Stream_ops
{
	iofunc_t Read;
	iofunc_t Write;
	int (*Close)(void*);
	int (*Flush)(void*);
};
struct Stream
{
	OPERATIONS(Stream_ops);
};
void Stream_ctor(struct Stream* obj)
{}
struct FileStream
{
	INHERITS(Stream);
	int fd;
};
Int FileStream_Read(void* obj, Byte* b, Int i)
{
	struct FileStream* obj1=obj;
	return read(obj1->fd,b,i);
}
void FileStream_ctor(struct FileStream* obj)
{
	Stream_ctor(&obj->Base);
	
}

int main()
{
	
}
