
#ifndef __ilabsession_h__
#	define __ilabsession_h__

#	include <fff.h>

	namespace TechnicalLabs
	{
		using namespace fff;

		class ILabSession
		{
		public:

			virtual void run(Compiler &_) = 0;
			virtual String getName() const = 0;
            virtual String nextSvg(String name) = 0;
		};
	}



#endif