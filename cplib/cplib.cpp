//cross platform c++ library
//written by: xaxaxa
/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
#include "headers/cplib.hpp"
using namespace std;

//////////////////////////////////////////////////////////////
namespace xaxaxa
{
	int objs = 0;
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

///////////////////////////////////////////////////////////
	Long Splice(Stream& from, Stream& to, Long c)
	{ //overload if a more efficient mechanism is available for your stream type, for example
	  //sendfile(2) for file to socket

		//default generic inefficient implementation
		Int c2 = c;
		Int bs = c;
		if (bs > 8192) bs = 8192;
		Buffer b(bs);
		Int br;
		while ((br = from.Read(b)) > 0) {
			to.Write(b.SubBuffer(0, br));
			c2 -= br;
			if (c2 <= 0) break;
			if (c2 > b.Length) b.Clip(0, c2);
		}
		return c - c2;
	}
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
		Close();
		free(buf);
	}
	int StreamReaderWriter::Read(const BufferRef& buf)
	{
		int br = 0;

		int tmp = buf_length;
		if (buf.Length < tmp) tmp = buf.Length;
		if (tmp > 0) {
			memcpy(buf.Data, ((Byte*) this->buf) + buf_index, tmp);
			//buf.Clip(tmp);
			buf_index += tmp;
			buf_length -= tmp;
			br += tmp;
		}
		if (br >= buf.Length) return br;
		if (buf.Length - br > this->buf_size) {
			while ((br >= buf.Length) && (tmp = s->Read(buf.SubBuffer(br))) > 0) {
				br += tmp;
				//buf.Clip(tmp);
			}
			return br;
		}
		while (1) {
			if (buf_length <= 0) {
				Buffer tmpb((char*) this->buf, buf_size);
				tmp = s->Read(tmpb);
				if (tmp <= 0) return br;
				buf_index = 0;
				buf_length = tmp;
			}
			tmp = buf_length;
			if (buf.Length - br < tmp) tmp = buf.Length - br;
			memcpy(buf.Data + br, ((Byte*) this->buf) + buf_index, tmp);
			//buf.Clip(tmp);
			buf_index += tmp;
			buf_length -= tmp;
			br += tmp;
			if (br >= buf.Length) return br;
		}
	}
	int StreamReaderWriter::Read(StringBuilder& buf, int length)
	{
		int br = 0;
		if (buf_length > 0) {
			int tmp = buf_length;
			if (length < tmp) tmp = length;
			Buffer tmpb((char*) this->buf + buf_index, tmp);
			buf.Append(tmpb);
			length -= tmp;
			buf_index += tmp;
			buf_length -= tmp;
			br += tmp;
		}
		if (length > 0) {
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
		while (1) {
			if (buf_length <= 0) {
				Buffer tmpb((char*) this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0) return (br == 0 ? -1 : br);
				buf_index = 0;
				buf_length = tmp;
			}
			int tmp2 = buf_length + buf_index;
			for (int i = buf_index; i < tmp2; i++) {
				for (int j = 0; j < delimitor_count; j++)
					if (((char*) (this->buf))[i] == delimitors[j]) {
						int tmp = i - buf_index;
						Buffer tmpb((char*) this->buf + buf_index, tmp);
						buf.Append(tmpb);
						br += tmp;
						tmp++;
						while (tmp < buf_length) {
							for (j = 0; j < delimitor_count; j++)
								if (((char*) (this->buf))[buf_index + tmp] == delimitors[j]) goto asdfg;
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
	int StreamReaderWriter::Read(Stream& out, const char* delimitors, int delimitor_count)
	{
		int br = 0;
		while (1) {
			if (buf_length <= 0) {
				BufferRef tmpb((char*) this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0) return (br == 0 ? -1 : br);
				buf_index = 0;
				buf_length = tmp;
			}
			int tmp2 = buf_length + buf_index;
			for (int i = buf_index; i < tmp2; i++) {
				for (int j = 0; j < delimitor_count; j++)
					if (((char*) (this->buf))[i] == delimitors[j]) {
						int tmp = i - buf_index;
						BufferRef tmpb((char*) this->buf + buf_index, tmp);
						out.Write(tmpb);
						br += tmp;
						tmp++;
						while (tmp < buf_length) {
							for (j = 0; j < delimitor_count; j++)
								if (((char*) (this->buf))[buf_index + tmp] == delimitors[j]) goto asdfg;
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
	int StreamReaderWriter::Read(Stream& out, const STRING* delimitors, int delimitor_count,
			int* delim_index)
	{
		int br = 0;
		char* cbuf = (char*) this->buf;
		//int delimitor_maxlen = 0;
		int i;
		/*for (i = 0; i < delimitor_count; i++)
		 if (delimitors[i].length > delimitor_maxlen)
		 delimitor_maxlen = delimitors[i].length;*/
		while (1) {
			if (buf_length <= 0) {
				BufferRef tmpb(this->buf, buf_size);
				int tmp = s->Read(tmpb);
				if (tmp <= 0) return (br == 0 ? -1 : br);
				buf_index = 0;
				buf_length = tmp;
			}
			int tmp2 = buf_length + buf_index;
			for (i = buf_index; i < tmp2; i++) {
				for (int j = 0; j < delimitor_count; j++) {
					if (cbuf[i] == delimitors[j].c[0]) {
						for (int ii = 0; ii < delimitors[j].length; ii++) {
							if (i + ii >= tmp2) {
								//goto cont;
								BufferRef tmpb(cbuf + buf_index, buf_length - ii);
								out.Write(tmpb);
								br += tmpb.Length;

								memcpy(cbuf, cbuf + i, ii);
								Buffer tmpb1(cbuf + ii, buf_size - ii);
								int tmp = s->Read(tmpb1);
								if (tmp <= 0) return (br == 0 ? -1 : br);
								buf_index = 0;
								buf_length = tmp + ii;
								i = 0;
							}
							if (cbuf[i + ii] != delimitors[j].c[ii]) goto cont;
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
						if (delim_index != NULL) *delim_index = j;
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
		while (1) {
			if (buf_length <= 0) {
				int tmp = s->Read(tmpb);
				if (tmp <= 0) return (br == 0 ? -1 : br);
				buf_index = 0;
				buf_length = tmp;
			}
			int tmp2 = buf_length + buf_index;
			register int i = buf_index;
			while (true) {
				while (i < tmp2 && cbuf[i] != '\n')
					i++;
				if (!(i < tmp2)) break;
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

	void StreamReaderWriter::Write(const BufferRef& buf)
	{
		if (buf.Length > max_wbuffer_copy) {
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

	/*void StreamReader_async::begin()
	 {

	 }
	 void StreamReader_async::check_buffer()
	 {
	 if (sr->buf_length <= 0)
	 {
	 BufferRef tmpb(sr->buf, sr->buf_size);
	 int tmp = sr->s->Read(tmpb);
	 if (tmp <= 0)
	 {
	 completed(br == 0 ? -1 : br);
	 return;
	 }
	 sr->buf_index = 0;
	 sr->buf_length = tmp;
	 }
	 do_process();
	 }
	 void StreamReader_async::do_process()
	 {
	 Int tmp2 = sr->buf_length + sr->buf_index;
	 Int i;
	 char* cbuf = (char*) sr->buf;
	 for (i = sr->buf_index; i < tmp2; i++)
	 {
	 for (int j = 0; j < delimitor_count; j++)
	 {
	 if (cbuf[i] == delimitors[j].c[0])
	 {
	 for (int ii = 0; ii < delimitors[j].length; ii++)
	 {
	 if (i + ii >= tmp2)
	 {
	 //goto cont;
	 BufferRef tmpb(cbuf + sr->buf_index, sr->buf_length - ii);
	 out->Write(tmpb);
	 br += tmpb.Length;

	 memcpy(cbuf, cbuf + i, ii);
	 Buffer tmpb1(cbuf + ii, sr->buf_size - ii);
	 int tmp = sr->s->Read(tmpb1);
	 if (tmp <= 0)
	 return (br == 0 ? -1 : br);
	 sr->buf_index = 0;
	 sr->buf_length = tmp + ii;
	 i = 0;
	 }
	 if (cbuf[i + ii] != delimitors[j].c[ii])
	 goto cont;
	 }
	 int tmp = i - sr->buf_index;
	 BufferRef tmpb(cbuf + sr->buf_index, tmp);
	 out->Write(tmpb);
	 br += tmp;
	 tmp += delimitors[j].length;

	 sr->buf_index += tmp;
	 sr->buf_length -= tmp;
	 if (delim_index != NULL)
	 *delim_index = j;
	 return br;
	 }
	 cont: ;
	 }
	 }
	 BufferRef tmpb((char*) this->buf + buf_index, buf_length);
	 out->Write(tmpb);
	 br += sr->buf_length;
	 sr->buf_length = 0;
	 check_buffer();
	 }
	 void StreamReader_async::completed(Int ret)
	 {

	 }*/
	/////////////////////////////////////////////////////////
	StringBuilder::StringBuilder(int initsize) :
			buf(initsize), Capacity(initsize), length(0), position(0)
	{
		//this->buf = malloc(initsize);
	}
	StringBuilder::StringBuilder(const Buffer& b) :
			buf(b), Capacity(b.Length), length(0), position(0)
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
		if (this->position > this->length) this->length = this->position;
	}
	void StringBuilder::Append(STRING buf)
	{
		//int tmp=this->Length;
		this->EnsureCapacity(this->position + buf.length);
		memcpy((char*) this->buf.Data + this->position, buf.c, buf.length);
		this->position += buf.length;
		if (this->position > this->length) this->length = this->position;
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
		if (buf.Length <= 0 || this->length <= 0) return -1;
		return memcmp(this->buf.Data, buf.Data,
				buf.Length < this->length ? buf.Length : this->length);
	}
	int StringBuilder::CompareTo(const StringBuilder& sb)
	{
		if (sb.length <= 0 || this->length <= 0) return -1;
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
		if (i <= 0) return 0;
		if (buf.Length < i) i = buf.Length;
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
