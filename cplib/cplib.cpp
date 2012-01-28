//cross platform c++ library
//written by: xaxaxa

#include "headers/cplib.hpp"
using namespace std;

//////////////////////////////////////////////////////////////
namespace xaxaxa
{
#ifdef __debug_obj123
	static int objs;
	void __objs_inc()
	{
		objs++;
	}
	void __objs_dec()
	{
		objs--;
	}
	int __objs_get()
	{
		return objs;
	}
#endif
#ifdef __debug_print123
	int __Buffer_bytes_allocated=0;
	void __Buffer_bytes_inc(int i)
	{
		__Buffer_bytes_allocated+=i;
	}
	void __Buffer_bytes_dec(int i)
	{
		__Buffer_bytes_allocated-=i;
	}
	int __Buffer_bytes_get()
	{
		return __Buffer_bytes_allocated;
	}
#endif
	Stream::Cap operator|(Stream::Cap c1, Stream::Cap c2)
	{
		return (Stream::Cap)((Byte)c1|(Byte)c2);
	}
	Stream::Cap operator&(Stream::Cap c1, Stream::Cap c2)
	{
		return (Stream::Cap)((Byte)c1&(Byte)c2);
	}
	Stream::Cap operator~(Stream::Cap c)
	{
		return (Stream::Cap)(~(Byte)c);
	}
	FileStream::FileStream(File f):f(f)
	{
	}
	FileStream::~FileStream()
	{
		//Close();
	}
	int FileStream::Read(Buffer buf)
	{
		return f.Read(buf);
	}
	void FileStream::Write(Buffer buf)
	{
		int bw=0;
		int off=0;
		while(off<buf.Length && (bw=f.Write(buf.SubBuffer(off)))>0)off+=bw;
	}
	void FileStream::Flush()
	{
		//::fflush(f);
	}
	void FileStream::Close()
	{
		f.Close();
	}

///////////////////////////////////////////////////////////

	StreamReader::StreamReader(Stream& s, int buffersize)
	{
		this->s = &s;
		buf = malloc(buffersize);
		this->buf_size = buffersize;
		this->buf_index = 0;
		this->buf_length = 0;
	}
	StreamReader::~StreamReader()
	{
		free(buf);
	}
	int StreamReader::Read(Buffer buf)
	{
		int br = 0;
		
		int tmp = buf_length;
		if (buf.Length < tmp) tmp = buf.Length;
		if(tmp>0)
		{
			memcpy(buf.Data, ((Byte*) this->buf) + buf_index, tmp);
			buf.Data += tmp;
			buf.Length -= tmp;
			buf_index += tmp;
			buf_length -= tmp;
			br += tmp;
		}
		if (buf.Length > 0)
			br += s->Read(buf);
		return br;
	}
	int StreamReader::Read(StringBuilder& buf, int length)
	{
		int br = 0;
		if (buf_length > 0)
		{
			int tmp = buf_length;
			if (length < tmp) tmp = length;
			Buffer tmpb((char*) this->buf + buf_index, tmp);
			buf.Append(tmpb);
			length -= tmp;
			buf_index += tmp;
			buf_length -= tmp;
			br += tmp;
		}
		if (length > 0)
		{
			buf.EnsureCapacity(buf.Length + length);
			Buffer tmpb((char*) (buf.buf) + buf.Length, length);
			int tmp = s->Read(tmpb);
			buf.Length += tmp;
			br += tmp;
		}
		return br;
	}
	int StreamReader::Read(StringBuilder& buf, const char* delimitors,
			int delimitor_count)
	{
		int br = 0;
		while (1)
		{
			if (buf_length <= 0)
			{
				Buffer tmpb((char*) this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0) return (br == 0 ? -1 : br);
				buf_index = 0;
				buf_length = tmp;
			}
			int tmp2 = buf_length + buf_index;
			for (int i = buf_index; i < tmp2; i++)
			{
				for (int j = 0; j < delimitor_count; j++)
					if (((char*) (this->buf))[i] == delimitors[j])
					{
						int tmp = i - buf_index;
						Buffer tmpb((char*) this->buf + buf_index, tmp);
						buf.Append(tmpb);
						br += tmp;
						tmp++;
						while (tmp < buf_length)
						{
							for (j = 0; j < delimitor_count; j++)
								if (((char*) (this->buf))[buf_index + tmp]
										== delimitors[j]) goto asdfg;
							break;
							asdfg: tmp++;
						}
						buf_index += tmp;
						buf_length -= tmp;
						return br;
					}
			}
			Buffer tmpb((char*) this->buf + buf_index, buf_length);
			buf.Append(tmpb);
			br += buf_length;
			buf_length = 0;
		}
	}
	int StreamReader::Read(Stream& buf, const char* delimitors, int delimitor_count)
	{
		int br = 0;
		while (1)
		{
			if (buf_length <= 0)
			{
				Buffer tmpb((char*) this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0) return (br == 0 ? -1 : br);
				buf_index = 0;
				buf_length = tmp;
			}
			int tmp2 = buf_length + buf_index;
			for (int i = buf_index; i < tmp2; i++)
			{
				for (int j = 0; j < delimitor_count; j++)
					if (((char*) (this->buf))[i] == delimitors[j])
					{
						int tmp = i - buf_index;
						Buffer tmpb((char*) this->buf + buf_index, tmp);
						buf.Write(tmpb);
						br += tmp;
						tmp++;
						while (tmp < buf_length)
						{
							for (j = 0; j < delimitor_count; j++)
								if (((char*) (this->buf))[buf_index + tmp]
										== delimitors[j]) goto asdfg;
							break;
							asdfg: tmp++;
						}
						buf_index += tmp;
						buf_length -= tmp;
						return br;
					}
			}
			Buffer tmpb((char*) this->buf + buf_index, buf_length);
			buf.Write(tmpb);
			br += buf_length;
			buf_length = 0;
		}
	}
	int StreamReader::Read(Stream& buf, const STRING* delimitors,
			int delimitor_count)
	{
		int br = 0;
		while (1)
		{
			if (buf_length <= 0)
			{
				Buffer tmpb((char*) this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0) return (br == 0 ? -1 : br);
				buf_index = 0;
				buf_length = tmp;
			}
			int tmp2 = buf_length + buf_index;
			for (int i = buf_index; i < tmp2; i++)
			{
				for (int j = 0; j < delimitor_count; j++)
				{
					if (((char*) (this->buf))[i] == delimitors[j].c[0])
					{
						for (int ii = 0; ii < delimitors[j].length; ii++)
						{
							if (i + ii >= tmp2) goto cont;
							if (((char*) (this->buf))[i + ii] != delimitors[j].c[ii])
								goto cont;
						}
						int tmp = i - buf_index;
						Buffer tmpb((char*) this->buf + buf_index, tmp);
						buf.Write(tmpb);
						br += tmp;
						tmp += delimitors[j].length;
						/*while (tmp < buf_length)
						 {
						 for (j = 0; j < delimitor_count; j++)
						 if (((char*) (this->buf))[buf_index + tmp]
						 == delimitors[j]) goto asdfg;
						 break;
						 asdfg: tmp++;
						 }*/
						buf_index += tmp;
						buf_length -= tmp;
						return br;
					}
					cont: ;
				}
			}
			Buffer tmpb((char*) this->buf + buf_index, buf_length);
			buf.Write(tmpb);
			br += buf_length;
			buf_length = 0;
		}
	}
	int StreamReader::ReadLine(StringBuilder& buf)
	{
		return Read(buf, "\x0A\x0D", 2);
	}
	void StreamReader::Write(Buffer buf)
	{
		s->Write(buf);
	}
	void StreamReader::Flush()
	{
		s->Flush();
	}
	void StreamReader::Close()
	{
		s->Close();
	}
/////////////////////////////////////////////////////////

	StringBuilder::StringBuilder(int initsize)
	{
		this->buf = malloc(initsize);
		this->Capacity = initsize;
		this->Length = 0;
	}
	StringBuilder::~StringBuilder()
	{
		free(buf);
	}
	void StringBuilder::Append(Buffer buf)
	{
		//int tmp=this->Length;
		this->EnsureCapacity(this->Length + buf.Length);
		memcpy((char*) this->buf + this->Length, buf.Data, buf.Length);
		this->Length += buf.Length;
	}
	void StringBuilder::Append(STRING buf)
	{
		//int tmp=this->Length;
		this->EnsureCapacity(this->Length + buf.length);
		memcpy((char*) this->buf + this->Length, buf.c, buf.length);
		this->Length += buf.length;
	}
	void StringBuilder::Append(const StringBuilder* s)
	{
		Buffer tmpb((char*) s->buf, s->Length);
		Append(tmpb);
	}
	void StringBuilder::Append(char* buf, int length)
	{
		this->EnsureCapacity(this->Length + length);
		memcpy((char*) this->buf + this->Length, buf, length);
		this->Length += length;
	}
	void StringBuilder::Append(char* buf)
	{
		Append(buf, strlen(buf));
	}
	void StringBuilder::EnsureCapacity(int c)
	{
		if (Capacity >= c) return;
		int tmp = this->Capacity;
		while (tmp < c)
		{
			tmp *= 2;
		}
		this->buf = realloc(this->buf, tmp);
		this->Capacity = tmp;
	}
	int StringBuilder::CompareTo(Buffer buf)
	{
		if (buf.Length <= 0 || this->Length <= 0) return -1;
		return memcmp(this->buf, buf.Data,
				buf.Length < this->Length ? buf.Length : this->Length);
	}
	int StringBuilder::CompareTo(const StringBuilder* sb)
	{
		if (sb->Length <= 0 || this->Length <= 0) return -1;
		return memcmp(buf, sb->buf,
				sb->Length < this->Length ? sb->Length : this->Length);
	}
	STRING StringBuilder::ToString()
	{
		STRING tmp;
		/*tmp.c=(char*)malloc(Length);
		 memcpy(tmp.c,buf,Length);
		 tmp.length=Length;*/
		tmp.c = (char*) this->buf;
		tmp.length = Length;
		return tmp;
	}
	Buffer StringBuilder::ToBuffer()
	{
		return Buffer((char*) this->buf, Length);
	}

	int StringBuilder::Read(Buffer buf)
	{
		return 0;
	}
	void StringBuilder::Write(Buffer buf)
	{
		Append(buf);
	}
	void StringBuilder::Flush()
	{

	}
	void StringBuilder::Close()
	{

	}

//////////////////////////////////////////////////////////////////////////
	static void handle_sig(int sig)
	{
		//asm(".intel_syntax noprefix");
		//asm("pop %eax");
		//asm("ret");
		//return;
		//Exception *ex;
		switch (sig)
		{
			case SIGSEGV:
				throw PointerException();
				break;
			default:
				throw Exception();
		}

	}
	void RegisterExceptions()
	{
		signal(SIGSEGV, &handle_sig);
	}
	Util_c Util;
}
