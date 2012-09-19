#ifndef __sessions_h__
#	define __sessions_h__

#	include "usefff.h"
#	include "ILabSession.h"
#	include "svgfastdiagram.h"
#	include "SampleGenerator.h"

#	include <sstream>

/*
	Some technical steps to analyse discrete convolution.

	Steps:
	1. Easy convolution samples
		a, zeros kernel
		b, dirac-impulse kernel
		c, shifted dirac-impulse kernel
		d, 


*/
	using namespace fff;
    using namespace fff::Computing;
    using namespace std;
    using namespace TechnicalLabs::Tools;


#	define TL_BEGIN_SESSION( \
		NAME) \
		\
		namespace TechnicalLabs{ \
			namespace Sessions{ \
				template<class SampleType> \
				class NAME : public ILabSession \
				{ \
				public: \
					typedef SampleType MySample; \
				private: \
					int m_i;  \
				public: \
					NAME() : ILabSession(), m_i(0){} \
						virtual String getName() const { return String(#NAME); } \
						virtual String nextSvg(String name) { stringstream ss; \
							ss << "Analysis/" << getName() << "_" << ++m_i << "_" << name << ".svg"; \
							return ss.str(); } \
					virtual void run(Compiler &_)

#	define TL_END_SESSION()	}; } }

#	define TL_CREATE_SESSION(NAME, SAMPLETYPE) \
		new NAME<SAMPLETYPE>()

#	define TL_DESTROY_SESSION(SESS_OBJ) \
		delete (SESS_OBJ)


	namespace TechnicalLabs
	{
		void run();
	}

#endif