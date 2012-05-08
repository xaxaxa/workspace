//cross platform c++ library
//written by: xaxaxa

#include "headers/cplib.hpp"
using namespace std;

//////////////////////////////////////////////////////////////
namespace xaxaxa
{
	int objs=0;
#ifdef __debug_obj123

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
		return (Stream::Cap) ((Byte) c1 | (Byte) c2);
	}
	Stream::Cap operator&(Stream::Cap c1, Stream::Cap c2)
	{
		return (Stream::Cap) ((Byte) c1 & (Byte) c2);
	}
	Stream::Cap operator~(Stream::Cap c)
	{
		return (Stream::Cap) (~(Byte) c);
	}
	FileStream::FileStream(File f) :
			f(f)
	{
	}
	FileStream::~FileStream()
	{
		//Close();
	}
	int FileStream::Read(const BufferRef& buf)
	{
		return f.Read(buf);
	}
	void FileStream::Write(const BufferRef& buf)
	{
		int bw = 0;
		int off = 0;
		while (off < buf.Length && (bw = f.Write(buf.SubBuffer(off))) > 0)
			off += bw;
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

	StreamReaderWriter::StreamReaderWriter(Stream& s, int rbuffersize, int wbuffersize) :
			wbuf(wbuffersize), wbuffersize(wbuffersize), max_wbuffer_copy(1024)
	{
		this->s = &s;
		buf = malloc(rbuffersize);
		this->buf_size = rbuffersize;
		this->buf_index = 0;
		this->buf_length = 0;
	}
	StreamReaderWriter::~StreamReaderWriter()
	{
		free(buf);
	}
	int StreamReaderWriter::Read(const BufferRef& buf)
	{
		int br = 0;

		int tmp = buf_length;
		if (buf.Length < tmp)
			tmp = buf.Length;
		if (tmp > 0)
		{
			memcpy(buf.Data, ((Byte*) this->buf) + buf_index, tmp);
			//buf.Clip(tmp);
			buf_index += tmp;
			buf_length -= tmp;
			br += tmp;
		}
		if (br >= buf.Length)
			return br;
		if (buf.Length - br > this->buf_size)
		{
			while ((br >= buf.Length) && (tmp = s->Read(buf.SubBuffer(br))) > 0)
			{
				br += tmp;
				//buf.Clip(tmp);
			}
			return br;
		}
		while (1)
		{
			if (buf_length <= 0)
			{
				Buffer tmpb((char*) this->buf, buf_size);
				tmp = s->Read(tmpb);
				if (tmp <= 0)
					return br;
				buf_index = 0;
				buf_length = tmp;
			}
			tmp = buf_length;
			if (buf.Length - br < tmp)
				tmp = buf.Length - br;
			memcpy(buf.Data + br, ((Byte*) this->buf) + buf_index, tmp);
			//buf.Clip(tmp);
			buf_index += tmp;
			buf_length -= tmp;
			br += tmp;
			if (br >= buf.Length)
				return br;
		}
	}
	int StreamReaderWriter::Read(StringBuilder& buf, int length)
	{
		int br = 0;
		if (buf_length > 0)
		{
			int tmp = buf_length;
			if (length < tmp)
				tmp = length;
			Buffer tmpb((char*) this->buf + buf_index, tmp);
			buf.Append(tmpb);
			length -= tmp;
			buf_index += tmp;
			buf_length -= tmp;
			br += tmp;
		}
		if (length > 0)
		{
			buf.EnsureCapacity(buf.length + length);
			Buffer tmpb((char*) (buf.buf.Data) + buf.length, length);
			int tmp = s->Read(tmpb);
			buf.length += tmp;
			br += tmp;
		}
		return br;
	}
	int StreamReaderWriter::Read(StringBuilder& buf, const char* delimitors, int delimitor_count)
	{
		int br = 0;
		while (1)
		{
			if (buf_length <= 0)
			{
				Buffer tmpb((char*) this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0)
					return (br == 0 ? -1 : br);
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
								if (((char*) (this->buf))[buf_index + tmp] == delimitors[j])
									goto asdfg;
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
	int StreamReaderWriter::Read(Stream& out,
			const char* delimitors, int delimitor_count)
	{
		int br = 0;
		while (1)
		{
			if (buf_length <= 0)
			{
				BufferRef tmpb((char*) this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0)
					return (br == 0 ? -1 : br);
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
						BufferRef tmpb((char*) this->buf + buf_index, tmp);
						out.Write(tmpb);
						br += tmp;
						tmp++;
						while (tmp < buf_length)
						{
							for (j = 0; j < delimitor_count; j++)
								if (((char*) (this->buf))[buf_index + tmp] == delimitors[j])
									goto asdfg;
							break;
							asdfg: tmp++;
						}
						buf_index += tmp;
						buf_length -= tmp;
						return br;
					}
			}
			BufferRef tmpb((char*) this->buf + buf_index, buf_length);
			out.Write(tmpb);
			br += buf_length;
			buf_length = 0;
		}
	}
	int StreamReaderWriter::Read(Stream& out,
			const STRING* delimitors, int delimitor_count, int* delim_index)
	{
		int br = 0;
		char* cbuf = (char*) this->buf;
		//int delimitor_maxlen = 0;
		int i;
		/*for (i = 0; i < delimitor_count; i++)
		 if (delimitors[i].length > delimitor_maxlen)
		 delimitor_maxlen = delimitors[i].length;*/
		while (1)
		{
			if (buf_length <= 0)
			{
				BufferRef tmpb(this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0)
					return (br == 0 ? -1 : br);
				buf_index = 0;
				buf_length = tmp;
			}
			int tmp2 = buf_length + buf_index;
			for (i = buf_index; i < tmp2; i++)
			{
				for (int j = 0; j < delimitor_count; j++)
				{
					if (cbuf[i] == delimitors[j].c[0])
					{
						for (int ii = 0; ii < delimitors[j].length; ii++)
						{
							//TODO: BUG: when end-of-buffer is encountered
							//in the middle of a delimitor, that delimitor
							//is not dealt with correctly.
							if (i + ii >= tmp2)
							{
								//goto cont;
								BufferRef tmpb(cbuf + buf_index, buf_length - ii);
								out.Write(tmpb);
								br += tmpb.Length;

								memcpy(cbuf, cbuf + i, ii);
								Buffer tmpb1(cbuf + ii, buf_size - ii);
								int tmp = s->Read(tmpb1);
								if (tmp <= 0)
									return (br == 0 ? -1 : br);
								buf_index = 0;
								buf_length = tmp + ii;
								i = 0;
							}
							if (cbuf[i + ii] != delimitors[j].c[ii])
								goto cont;
						}
						int tmp = i - buf_index;
						BufferRef tmpb(cbuf + buf_index, tmp);
						out.Write(tmpb);
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
						if (delim_index != NULL)
							*delim_index = j;
						return br;
					}
					cont: ;
				}
			}
			BufferRef tmpb((char*) this->buf + buf_index, buf_length);
			out.Write(tmpb);
			br += buf_length;
			buf_length = 0;
		}
	}
	int StreamReaderWriter::fast_readline(Stream& out)
	{
		int br = 0;
		Byte* cbuf = (Byte*) this->buf;
		BufferRef tmpb(cbuf, buf_size);
		while (1)
		{
			if (buf_length <= 0)
			{
				int tmp = s->Read(tmpb);
				if (tmp <= 0)
					return (br == 0 ? -1 : br);
				buf_index = 0;
				buf_length = tmp;
			}
			int tmp2 = buf_length + buf_index;
			register int i = buf_index;
			while (true)
			{
				while (i < tmp2 && cbuf[i] != '\n')
					i++;
				if (!(i < tmp2))
					break;
				int tmp = i - buf_index;
				//Buffer tmpb((char*) this->buf + buf_index, tmp);
				//cb({(Byte*) this->buf + buf_index, tmp});
				out.Write(tmpb.SubBuffer(buf_index, tmp));
				br += tmp;
				tmp++;
				buf_index += tmp;
				buf_length -= tmp;
				return br;
			}
			//Buffer tmpb((char*) this->buf + buf_index, buf_length);
			out.Write(tmpb.SubBuffer(buf_index, buf_length));
			br += buf_length;
			buf_length = 0;
		}
	}
	inline int StreamReaderWriter::ReadLine(Stream& buf)
	{
		STRING delim[2]
		{ "\r\n", "\n" };
		return Read(buf, delim, 2);
		//return Read(buf, "\r\n", 2);
	}
	void StreamReaderWriter::Write(const BufferRef& buf)
	{
		if (buf.Length > max_wbuffer_copy)
		{
			s->Write(buf);
			return;
		}
		//s->Write(buf);
		flush_if_full(buf.Length);
		wbuf.Append(buf);
		flush_if_full();
	}
	void StreamReaderWriter::Flush()
	{
		do_flush();
		s->Flush();
	}
	void StreamReaderWriter::Close()
	{
		Flush();
		s->Close();
	}
/////////////////////////////////////////////////////////

	StringBuilder::StringBuilder(int initsize) :
			buf(initsize), Capacity(initsize), length(0), position(0)
	{
		//this->buf = malloc(initsize);
	}
	StringBuilder::~StringBuilder()
	{
		//free(buf);
	}
	void StringBuilder::Append(const BufferRef& buf)
	{
		//int tmp=this->Length;
		this->EnsureCapacity(this->position + buf.Length);
		memcpy((char*) this->buf.Data + this->position, buf.Data, buf.Length);
		this->position += buf.Length;
		if (this->position > this->length)
			this->length = this->position;
	}
	void StringBuilder::Append(STRING buf)
	{
		//int tmp=this->Length;
		this->EnsureCapacity(this->position + buf.length);
		memcpy((char*) this->buf.Data + this->position, buf.c, buf.length);
		this->position += buf.length;
		if (this->position > this->length)
			this->length = this->position;
	}
	void StringBuilder::Append(const StringBuilder* s)
	{
		Buffer tmpb((char*) s->buf.Data, s->length);
		Append(tmpb);
	}
	void StringBuilder::Append(const char* buf)
	{
		Append(buf, strlen(buf));
	}
	int StringBuilder::CompareTo(const BufferRef& buf)
	{
		if (buf.Length <= 0 || this->length <= 0)
			return -1;
		return memcmp(this->buf.Data, buf.Data,
				buf.Length < this->length ? buf.Length : this->length);
	}
	int StringBuilder::CompareTo(const StringBuilder& sb)
	{
		if (sb.length <= 0 || this->length <= 0)
			return -1;
		return memcmp(buf.Data, sb.buf.Data, sb.length < this->length ? sb.length : this->length);
	}
	STRING StringBuilder::ToString()
	{
		return
		{	(char*) this->buf.Data, length};
	}
	Buffer StringBuilder::ToBuffer()
	{
		return buf.SubBuffer(0, length);
	}

	int StringBuilder::Read(const BufferRef& buf)
	{
		int i = length - position;
		if (i <= 0)
			return 0;
		if (buf.Length < i)
			i = buf.Length;
		memcpy(buf.Data, ((char*) this->buf.Data) + position, i);
		position += i;
		return i;
	}
	void StringBuilder::Write(const BufferRef& buf)
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
