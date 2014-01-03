#define CRYPTOPP_DISABLE_AESNI 1
#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/base64.h>
#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <ext/stdio_filebuf.h>
#include <pthread.h>
using namespace std;
using namespace CryptoPP;

int Open(const char* file, bool write) {
	int fd=open(file,write?O_RDWR:O_RDONLY,0644);
	if(fd<0) throw runtime_error(string(strerror(errno))+": "+string(file));
	return fd;
}
void Read(int fd, char* buf, int len) {
	int off = 0;
	while (off < len) {
		int tmp = read(fd, buf + off, len - off);
		if (tmp <= 0) throw runtime_error("read failed");
		off += tmp;
	}
}
void LoadKey(const char* file, RSA::PrivateKey& key)
{
    ByteQueue q;
    FileSource KeyFile(file, true, new Base64Decoder());
    KeyFile.TransferTo(q);
    key.BERDecodePrivateKey(q,false,0); // last 2 params unused
}
void doDecode(const char* file, const string& outPath, RandomNumberGenerator& rng, PK_Decryptor& dec) {
	int fd=Open(file,false);
	string aeskey;
	
	
	int keylen,ivlen;
	if(read(fd,&keylen,sizeof(int))!=sizeof(int))goto out;
	if(read(fd,&ivlen,sizeof(int))!=sizeof(int))goto out;
	keylen=ntohl(keylen); ivlen=ntohl(ivlen);
	{
		char buf[keylen];
		Read(fd,buf,keylen);
		StringSource ss((byte*)buf,keylen, true,
			new PK_DecryptorFilter(rng, dec,
				new StringSink(aeskey)
			)
		);
		
		char iv[ivlen];
		Read(fd,iv,ivlen);
		
		string fn;
		fn=outPath+basename(file);
		
		__gnu_cxx::stdio_filebuf<char> filebuf(fd, std::ios::in);
		istream is(&filebuf);
		CBC_Mode<AES>::Decryption aes_dec((byte*)aeskey.data(),aeskey.length(),(byte*)iv);
		FileSource fs(is, true,
			new StreamTransformationFilter(aes_dec,
				new FileSink(fn.c_str(),true)
			)
		);
	}
out:
	close(fd);
}
struct tInfo
{
	AutoSeededRandomPool rng;
	pthread_t th;
	int startIndex;
	int items;
};
RSA::PrivateKey prk;
string outPath;
vector<string> files;
void* thread(void* v) {
	tInfo* inf=(tInfo*)v;
	RSAES_OAEP_SHA_Decryptor dec(prk);
	int x=inf->startIndex+inf->items;
	for(int i=inf->startIndex;i<x;i++) {
		doDecode(files[i].c_str(),outPath,inf->rng,dec);
	}
	return NULL;
}
int main(int argc, char** argv) {
	if(argc<3) {
		printf("usage: %s file.pem file1 file2 ... fileN outputDir [options]\nor: %s file.pem - outputDir [options] (reads filenames from stdin)\noptions: -t threadcount (default=sysconf(_SC_NPROCESSORS_CONF))\n",argv[0],argv[0]);
		return 1;
	}
	LoadKey(argv[1],prk);
	
	int nthreads=sysconf(_SC_NPROCESSORS_CONF);
	for(int i=2;i<argc;i++) {
		if(argv[i][0]=='-') {
			switch(argv[i][1]) {
				case 0:
					while(true) {
						char* line=NULL;
						size_t n=0;
						if(getline(&line,&n,stdin)<0) break;
						int l=strlen(line);
						for(l--;l>=0;l--)
							if(line[l]!='\r' && line[l]!='\n') break;
						files.push_back(string(line,l+1));
					}
					break;
				case 't':
					if(i+1>=argc) throw runtime_error("option -t requires a value");
					nthreads=atoi(argv[++i]);
					break;
				default:
					throw runtime_error("unrecognized option: "+string(argv[i]));
			}
		} else {
			files.push_back(argv[i]);
		}
	}
	outPath=files[files.size()-1];
	if(outPath.length()>0 && outPath.data()[outPath.length()-1]!='/')
		outPath+='/';
	files.resize(files.size()-1);
	int minItemsPerThread=files.size()/nthreads;
	int leftOver=files.size()%nthreads;
	tInfo infs[nthreads];
	int ind=0;
	for(int i=0;i<nthreads;i++) {
		int allocation=minItemsPerThread;
		if(i<leftOver)allocation++;
		infs[i].startIndex=ind;
		ind+=(infs[i].items=allocation);
		if(pthread_create(&infs[i].th,NULL,&thread,infs+i)!=0) {
			perror("pthread_create");
			return 1;
		}
	}
	for(int i=0;i<nthreads;i++)
		pthread_join(infs[i].th,NULL);
	return 0;
}
