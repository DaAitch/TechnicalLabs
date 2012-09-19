#include "labs.h"
#include "ILabSession.h"
#include "session.h"

#include <vector>
#include <iostream>

#include <fff.h>

using namespace TechnicalLabs::Sessions;
using namespace fff::Computing;

namespace TechnicalLabs
{
	using namespace fff;
	using namespace std;

	void run()
	{
		char absolutePath[1024];
#ifdef _WINDOWS
		const char *path = "%FFF%cl";
		
		ExpandEnvironmentStrings(path, absolutePath, 1024);
#else
#	error Only windows allowed or implement other os!
#endif

        

		std::vector<ILabSession *> sessions;
		sessions.push_back(TL_CREATE_SESSION(SimpleTransformations, Sample));
		sessions.push_back(TL_CREATE_SESSION(DirectConvolution, Sample));
        sessions.push_back(TL_CREATE_SESSION(Samples, Sample));
		

		for(
			std::vector<ILabSession *>::iterator it = sessions.begin();
			it != sessions.end();
			++it)
		{
			cout << "Session: " << (*it)->getName() << endl;
			

			OpenCLEnvironment env(CL_DEVICE_TYPE_GPU);
            Compiler c(env, String(absolutePath));
		    c.build();

            fff_EXPECT_VALID_OBJ(env);
            fff_EXPECT_VALID_OBJ(c);

			cout << "run session..." << endl;
			(*it)->run(c);
			TL_DESTROY_SESSION(*it);
			cout << "destroy session..." << endl << endl;
		}

		
	}
}