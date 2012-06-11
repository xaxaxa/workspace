#include "filter.H"

namespace xaxaxa
{
	template<class NUMTYPE>class asdfzxcv: public Filter<NUMTYPE>
	{
	public:
		virtual void Process(NUMTYPE* data, UInt length)
		{
			Process(data, data, length);
		}
		virtual void Process(const NUMTYPE* in, NUMTYPE* out, UInt length)
		{
			for(UInt i=0;i<length;i++)
			{
				
			}
		}
	}
	
}
