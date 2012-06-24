#define WARNLEVEL 5
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
	LineIterator(StreamReader& sr):sr(sr)
	{}
	LineIterator():eof(true)
	{}
	string operator*()
	{
		return cur_string;
	}
	void operator++()
	{
		eof=(sr->ReadLine(cur_string)<=0);
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
			memcpy(valid_chars+i,".!#$%&'*+-/=?^_`{}|~1234567890",30); i+=30;
			valid_chars_len=i;
			sort(valid_chars,valid_chars+valid_chars_len);
			WARN(8,"valid_chars: "<<string(valid_chars,valid_chars_len));
			Util.ChDir(Util.GetDirFromPath(Util.GetProgramPath()));
			LineIterator it(*newobj<StreamReader>(*newobj<FileStream>(File("tlds",O_RDONLY))));
			tlds.insert<LineIterator>(it,LineIterator());
		}
	};
	static const int email_len_limit=64;
	static static_data s_data;
	CircularStream buf;
	StringBuilder cur_email;
	function<void(const BufferRef&)> email_found;
	enum class states
	{
		reading=0,
		a_found
	} state;
	email_finder():buf(email_len_limit),state(states::reading)
	{}
	int PutData(const BufferRef& b)
	{	//do one iteration
		char* c=(char*)b.Data;
		switch(state)
		{
			case states::reading:
			{
				int i(0);
				while(c[i]!='@' && i<b.Length)i++;
				if(i<b.Length)
				{	//found '@'
					WARN(8,"found '@'");
					state=states::a_found;
					buf.Write(b.SubBuffer(0,i));//append everything before the '@'
					//search for the beginning of the email
					auto it=buf.end();
					while(it!=buf.begin())
					{
						it--;
						WARN(9,*it << " is"<<(binary_search(s_data.valid_chars,s_data.valid_chars+s_data.valid_chars_len,*it)?" ":" not")<<" a valid email char");
						if(!binary_search(s_data.valid_chars,s_data.valid_chars+s_data.valid_chars_len,*it))break;
					}
					cur_email.Clear();
					buf.GetData(it+1,buf.end(),cur_email);
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
				if(i>=email_len_limit)
					goto a;
				cur_email<<BufferRef(c,i);
				email_found(cur_email.ToBuffer());
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
	Buffer b(4096);
	int br;
	email_finder f;
	f.email_found=[&ss](const BufferRef& e)
	{
		cout << string((char*)e.Data,e.Length) << endl;
	};
	while((br=ss.Read(b))>0)
	{
		f.ProcessBuffer(b.SubBuffer(0,br));
	}
	
}

