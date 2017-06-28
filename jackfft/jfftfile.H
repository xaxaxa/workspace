/*
 * functions for reading and writing .jfft filter definition files
 * */


#include <stdint.h>
#include <string>

/*
 * Parse a jfft file and put the results into a coefficient array
 * @param jfftFile contents of the jfft file
 * @param jfftBytes length of the jfft file
 * @param coeff the coefficient array to store results into
 * @param coeffs the length of coeff
 * @param srate sample rate; this sets the frequency scale of coeff
 */
void loadjfft(uint8_t* jfftFile, int jfftBytes, double* coeff, unsigned coeffs, int srate)
{
	struct jfftEntry {
		double freq; double val;
	} *buf;
	buf = (jfftEntry*) jfftFile;
	int entries = jfftBytes/sizeof(jfftEntry);

	unsigned i1=0;
	double last_v=0.5;
	for(int pos=0; pos<entries; pos++) {
		unsigned i2=(unsigned)round(((double)buf[pos].freq/(srate/2))*coeffs);
		//cout << i2 << endl;
		if(i2>coeffs)break;
		for(unsigned i=i1;i<i2;i++)
			coeff[i]=last_v*(i2-i)/(i2-i1)+buf[pos].val*(i-i1)/(i2-i1);
		i1=i2;
		last_v=buf[pos].val;
	}
	for(unsigned i=i1;i<coeffs;i++)
		coeff[i]=last_v;
}

/*
 * Generate a jfft file from coefficients
 * @param coeff array of coefficients
 * @param coeffs length of coeff array
 * @param srate sample rate; this sets the frequency scale of coeff
 * @return a byte array of the jfft file
 */
std::string savejfft(double* coeff, unsigned coeffs, int srate) {
	std::string tmp;
	struct {
		double freq;
		double val;
	} buf;
	for(unsigned i = 0; i < coeffs; i++) {
		buf.freq = double(i) / coeffs * srate / 2;
		buf.val = coeff[i];
		tmp.append((char*)&buf, sizeof(buf));
	}
	return tmp;
}
