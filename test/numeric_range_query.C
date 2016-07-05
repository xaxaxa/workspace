#include <stdio.h>
#include <map>
#include <unordered_set>
#include <vector>
#include <string.h>
using namespace std;

vector<string> docs;

map<double,vector<int> > terms;


// destroys s
void parseAndIndex(char* s) {
	int len=strlen(s);
	unordered_set<double> myterms;
	int lastNonDigit=-1;
	
	int docid=docs.size();
	docs.push_back(s);
	
	for(int i=0;i<len;i++) {
		char c=s[i];
		if(!isdigit(c)) {
			if(i!=lastNonDigit+1) {	//we have a number
				s[i]=0;
				double num=strtod(s+lastNonDigit+1, NULL);
				myterms.insert(num);
			}
			lastNonDigit=i;
		}
	}
	if(lastNonDigit!=len-1) {	//we have a number at the end
		double num=strtod(s+lastNonDigit+1, NULL);
		myterms.insert(num);
	}
	
	for(auto it=myterms.begin();it!=myterms.end();it++) {
		terms[(*it)].push_back(docid);
	}
}
int main(int argc, char** argv) {
	if(argc<2) {
		fprintf(stderr, "usage: %s FILE\nFILE is a text file containing items, one line each\n", argv[0]);
		return 1;
	}
	FILE* f=fopen(argv[1],"rb");
	if(f==NULL) {
		perror("fopen");
		return 1;
	}
	while(true) {
		char* line=NULL;
		size_t n=0;
		if(getline(&line,&n,f)<0) break;
		n=strlen(line);
		
		if(n>0 && line[n-1]=='\n') line[n-1]=0;
		parseAndIndex(line);
		free(line);
	}
	
	printf("docs:\n");
	for(int i=0;i<(int)docs.size();i++) {
		printf("%d: %s\n",i,docs[i].c_str());
	}
	printf("terms:\n");
	for(auto it=terms.begin();it!=terms.end();it++) {
		printf("%lf: ",(*it).first);
		auto& pl=(*it).second;
		for(int i=0;i<(int)pl.size();i++) {
			printf("%d ",pl[i]);
		}
		printf("\n");
	}
}
