#include <stdio.h>
#include <postgresql/libpq-fe.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <math.h>
using namespace std;
const int pagesize=1000;
void HTMLEncode(std::string& data) {
    std::string buffer;
    buffer.reserve((int)(data.size()*1.1));
    for(size_t pos = 0; pos != data.size(); ++pos) {
        switch(data[pos]) {
            case '&':  buffer.append("&amp;");       break;
            case '\"': buffer.append("&quot;");      break;
            case '\'': buffer.append("&apos;");      break;
            case '<':  buffer.append("&lt;");        break;
            case '>':  buffer.append("&gt;");        break;
            default:   buffer.append(1, data[pos]); break;
        }
    }
    data.swap(buffer);
}
void getPaging(ofstream* ofs, string url1, string url2, long offset, long total, long pagesize)
{
	long p=(offset/pagesize);
	long pages=(long)ceil(((double)total)/pagesize);
	if(pages<1)pages=1;
	for(long i=0;i<pages;i++) {
		if(i==p)
			*ofs << " " << (i+1) << " ";
		else *ofs << " <a href=\"" << url1 << (i*pagesize) << url2 << "\">" << (i+1) << "</a> ";
	}
}

void endPage(ofstream* ofs, const char* table, int ii, PGresult *res1)
{
	*ofs << "</table>\n<br />";
	getPaging(ofs,"t_"+string(table)+"_",".htm", ii, PQntuples(res1), pagesize);
	*ofs << "</body>\n</html>" << endl;
}

void newPage(ofstream*& ofs, const char* table, int ii, PGresult *res1)
{
	if(ofs!=NULL) {
		endPage(ofs, table, ii-1, res1);
		delete ofs; ofs=NULL;
	}
	int cols1=PQnfields(res1);
	ofs=new ofstream(((stringstream&)(stringstream() << "t_"<<table<<"_" << ii << ".htm")).str());
	*ofs << "<html>\n<head><title>Table " << table
		<< "</title></head>\n<body>\n<h1>Table "
		<< table << " (from row #" << ii
		<< ")</h1>\n<table border=\"1\">\n<tr>" << endl;
	for(int ii=0;ii<cols1;ii++) {
		*ofs << "<td>" << PQfname(res1, ii) << "</td>\n";
	}
	*ofs << "</tr>\n";
}

int main(int argc, char** argv)
{
	
	if(argc<3) {
		cerr << "usage: " << argv[0] << " connectionstring schema" << endl;
		return 1;
	}
	PGconn *conn;
	conn = PQconnectdb(argv[1]);
	if (PQstatus(conn) == CONNECTION_BAD) {
		cerr << "connection error" << endl;
		return 1;
	}
	Oid oid=25/*TEXTOID*/;
	int len=strlen(argv[2]);
	int tmp_i=0;
	PGresult *res=PQexecParams(conn,"select tablename from pg_tables where schemaname=$1",
					1, &oid, argv+2, &len, &tmp_i, 0);
	int rows=PQntuples(res);
	
	
	ofstream index_ss("index.htm");
	index_ss << "<html>\n<head><title>Tables</title></head>\n<body>\n<h1>Tables</h1>\n";
	index_ss << "<ul>" << endl;
	for(int i=0;i<rows;i++) {
		const char* table=PQgetvalue(res, i, 0/*col*/);
		cerr << "dumping table " << table << endl;
		index_ss << "<li><a href=\"t_" << table << "_0.htm\">" << table << "</a></li>" << endl;
		
		
		PGresult *res1=PQexec(conn,("select * from " + string(table) + " order by id desc").c_str());
		int rows1=PQntuples(res1);
		int cols1=PQnfields(res1);
		
		ofstream* ofs=NULL;
		newPage(ofs, table, 0, res1);
		for(int ii=0;ii<rows1;ii++) {
			if(ii % pagesize == 0 && ii!=0) {
				newPage(ofs, table, ii, res1);
			}
			*ofs << "<tr>\n";
			for(int iii=0;iii<cols1;iii++) {
				string s(PQgetvalue(res1, ii, iii));
				HTMLEncode(s);
				*ofs << "<td>" << s << "</td>" << endl;
			}
			*ofs << "</tr>" << endl;
		}
		if(ofs!=NULL) {
			endPage(ofs, table, rows1-1, res1);
			delete ofs; ofs=NULL;
		}
		PQclear(res1);
	}
	index_ss << "</ul>\n</body>\n</html>" << endl;
}
