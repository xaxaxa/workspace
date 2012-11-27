#include "ui_client.H"
//uint32_t cnt=0;
const char* server=getenv("GENERIC_UI_SERVER");
//Poll p;
Socket s(AF_INET,SOCK_STREAM);
UIClient uic;

void* thr1(void* v)
{
	//char s1[4096];
	/*s.repeatRecv(s1, 4096, 0, [s1](int i) {
		
	});
	p.loop();*/
	while(true) {
		int8_t* buf=uic.beginPutData(4096);
		int32_t i=s.recv(buf, 4096, 0);
		if(i<=0) throw runtime_error("disconnected from UI server");
		uic.endPutData(i);
	}
	return NULL;
}

__attribute__((constructor)) void init()
{
	cout << sizeof(generic_ui_cmd) << endl;
	//XXX: port is hardcoded
	s.connect(IPEndPoint(IPAddress(server), 16969));
	uic.inst=GenericUI::instance;
	uic.onWrite=[](const int8_t* buf, int32_t len) {
		s.send(buf, len, 0);
	};
	uic.doListen();
	pthread_t thr;
	pthread_create(&thr, NULL, &thr1, NULL);
	//listen(cb);
}


int main(int argc, char** argv) {
	/*
	//uncomment to test thread pool
	ThreadPool tp;
	tp.invoke([&tp](){
		while(1) {
			string s;
			getline(cin, s);
			printf("threads: %i\n", (int)tp.threads.size());
		}
	});
	tp.invoke([](){
		cout << "aaaaa" << endl;
		sleep(2);
		cout << "aaaaa2" << endl;
		//int* i=(int*)NULL;
		// *i=5;
		throw logic_error("some error");
	});
	tp.invoke([](){
		cout << "bbbbb" << endl;
		sleep(2);
		cout << "bbbbb2" << endl;
	});
	sleep(3);
	tp.invoke([](){
		cout << "zzzzz" << endl;
		sleep(2);
		cout << "xxxxx" << endl;
	});
	while(1)sleep(10000);
	//*/
	
	
	return 0;
}

