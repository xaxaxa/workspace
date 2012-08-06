#define WARNLEVEL 7
#include <iostream>
#include <cplib/cplib.hpp>
#include <set>
#include <algorithm>
using namespace std;
using namespace xaxaxa;


class LineIterator
{
public:
	Property<StreamReader> sr;
	string cur_string;
	bool eof;
	LineIterator(StreamReader& sr):sr(sr),eof(false)
	{}
	LineIterator():eof(true)
	{}
	string operator*()
	{
		return cur_string;
	}
	void operator++()
	{
		cur_string="";
		eof=(sr->ReadLine(cur_string)<0);
		//WARN(9,"cur_string="<<cur_string);
	}
	void operator++(int i)
	{
		operator++();
	}
	bool operator==(const LineIterator& other)
	{
		return eof==other.eof;
	}
	bool operator!=(const LineIterator& other)
	{
		return eof!=other.eof;
	}
};

class email_finder: public Object
{
public:
	struct static_data
	{
		char* valid_chars;
		int valid_chars_len;
		set<string> tlds;
		static_data()
		{
			valid_chars=new char[128];
			int i(0);
			for(char c='a'; c<='z'; c++)
				valid_chars[i++]=c;
			for(char c='A'; c<='Z'; c++)
				valid_chars[i++]=c;
			memcpy(valid_chars+i,".#$%'*+-?^_`{}|~1234567890",26); i+=26;
			valid_chars_len=i;
			sort(valid_chars,valid_chars+valid_chars_len);
			WARN(8,"valid_chars: "<<string(valid_chars,valid_chars_len));
			Util.ChDir(Util.GetDirFromPath(Util.GetProgramPath()));
			LineIterator it(*newobj<StreamReader>(*newobj<FileStream>(File("tlds",O_RDONLY))));
			it++;
			tlds.insert<LineIterator>(it,LineIterator());
			WARN(9,"tlds: " << tlds);
		}
	};
	static const int email_len_limit=64;
	static static_data s_data;
	CircularStream buf;
	StringBuilder cur_email;
	function<void(const Buffer&)> email_found;
	enum class states
	{
		reading=0,
		a_found
	} state;
	email_finder():buf(email_len_limit),state(states::reading)
	{}
	void do_found(const Buffer& e)
	{
		//check TLD
		Byte* v=(Byte*)memrchr(e.Data,'@',e.Length);
		if(v==NULL)return; //BUG
		Byte* v2=(Byte*)memrchr(v,'.',e.Length-(v-e.Data));
		if(v2==NULL)return; //no TLD
		string tld((char*)(v2+1),e.Length-(v2+1-e.Data));
		//WARN(7,"TLD: " << tld);
		if(s_data.tlds.find(tld)==s_data.tlds.end())return;
		email_found(e);
	}
	int PutData(const BufferRef& b)
	{	//do one iteration
		char* c=(char*)b.Data;
		switch(state)
		{
			case states::reading:
			{
				int i(0);
				//while(c[i]!='@' && i<b.Length)i++;
				void* tmpv=memchr(b.Data,'@',b.Length);
				if(tmpv!=NULL)
				{	//found '@'
					WARN(8,"found '@'");
					i=((Byte*)tmpv)-b.Data;
					state=states::a_found;
					buf.Write(b.SubBuffer(0,i));//append everything before the '@'
					//search for the beginning of the email
					auto it=buf.end();
					while(it!=buf.begin())
					{
						it--;
						WARN(9,*it << " is"<<(binary_search(s_data.valid_chars,s_data.valid_chars+s_data.valid_chars_len,*it)?"":" not")<<" a valid email char");
						if(!binary_search(s_data.valid_chars,s_data.valid_chars+s_data.valid_chars_len,*it))
						{
							it++; break;
						}
					}
					if(it==(buf.end()))
					{
						state=states::reading;
						buf.Clear();
						return i+1;
					}
					cur_email.Clear();
					buf.GetData(it,buf.end(),cur_email);
					cur_email << '@';
					return i+1;
				}
				else
				{	//did not find '@'
					buf.Write(b);
					return b.Length;
				}
			}
			case states::a_found:
			{
				int i(0);
				while(i<b.Length && i<email_len_limit && binary_search(s_data.valid_chars,s_data.valid_chars+s_data.valid_chars_len,c[i]))i++;
				if(i<=0 || i>=email_len_limit)
					goto a;
				cur_email<<BufferRef(c,i);
				do_found(cur_email.ToBuffer());
			a:
				state=states::reading;
				return i;
				break;
			}
			default:
				return b.Length;
		}
	}
	void ProcessBuffer(BufferRef b)
	{
		while(b.Length>0)
			b.Clip(PutData(b));
	}
};
email_finder::static_data email_finder::s_data;
int main(int ac, char** av)
{
	StandardStream ss;
	StreamReader sr(ss);
	StreamWriter sw(ss);
	Buffer b(1024*16);
	int br;
	email_finder f;
	bool b1(false);
	f.email_found=[&](const Buffer& e)
	{
		sw.WriteLine(e);
		sw.Flush();
		b1=true;
	};
	while((br=ss.Read(b))>0)
	{
		f.ProcessBuffer(b.SubBuffer(0,br));
	}
	return b1?0:2;
}

