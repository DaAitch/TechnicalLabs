
#ifndef __session_h__
#	define __session_h__
	
#	include "labs.h"



using namespace fff::Buffer::Complex::Host;
using namespace fff::Buffer::Complex::Device;
using namespace fff::Buffer::Complex::Ubiquitous;
using namespace fff::Computing::Kernel;



    TL_BEGIN_SESSION(Samples)
	{
        HostSingleChannel<Float> x(1024);

        // undersampling problem
        x.init();
        SampleGenerator::addReal<MySample>(x, 100,   0);
        svgfastdiagram(nextSvg("100-x-signal"), x, False);
        x.init();
        SampleGenerator::addReal<MySample>(x, 105,   0);
        svgfastdiagram(nextSvg("105-x-sampling"), x, False, True);


    } TL_END_SESSION()

	TL_BEGIN_SESSION(SimpleTransformations)
	{
        typedef fff::Factory::UbiMultiChannelFactory<MySample> mcf;

        OpenCLEnvironment &env = _.getEnv();
        mcf::multichannel x, X;
        mcf::create(x, env, CL_MEM_READ_ONLY, 1, 1024);
        mcf::create(X, env, CL_MEM_READ_WRITE, 1, 1024);
		
        Transformation<MySample> tra(_, *x, *X, True);


		// ---------------------------------------------------+
		SampleGenerator::addReal<MySample>(*(x.host), 64, 16);

		tra.invokeAndWait();
        
		svgfastdiagram(nextSvg("64,16-x"), (*((mcf::hmc*)x.host))[0], False);
		svgfastdiagram(nextSvg("64,16-X-P"), (*((mcf::hmc*)X.host))[0], True);
        svgfastdiagram(nextSvg("64,16-X-C"), (*((mcf::hmc*)X.host))[0], False);

        // ---------------------------------------------------+
        x.host->init();
		SampleGenerator::addReal<MySample>(*(x.host), 64);

		tra.invokeAndWait();
        
		svgfastdiagram(nextSvg("64,0-x"), (*((mcf::hmc*)x.host))[0], False);
		svgfastdiagram(nextSvg("64,0-X-P"), (*((mcf::hmc*)X.host))[0], True);
        svgfastdiagram(nextSvg("64,0-X-C"), (*((mcf::hmc*)X.host))[0], False);

        // ---------------------------------------------------+
        x.host->init();
		SampleGenerator::addReal<MySample>(*(x.host), 63);

		tra.invokeAndWait();
        
		svgfastdiagram(nextSvg("63,0-x"), (*((mcf::hmc*)x.host))[0], False);
		svgfastdiagram(nextSvg("63,0-X-P"), (*((mcf::hmc*)X.host))[0], True);
        svgfastdiagram(nextSvg("63,0-X-C"), (*((mcf::hmc*)X.host))[0], False);

		// ---------------------------------------------------+
		x.host->init();
        SampleGenerator::addReal<MySample>(*(x.host), 5,0);
		SampleGenerator::addReal<MySample>(*(x.host), 10,0);
		SampleGenerator::addReal<MySample>(*(x.host), 100,  0);

		tra.invokeAndWait();
        
		svgfastdiagram(nextSvg("5,0-10,0-100,0-x"), (*((mcf::hmc*)x.host))[0], False);
		svgfastdiagram(nextSvg("5,0-10,0-100,0-X-P"), (*((mcf::hmc*)X.host))[0], True);
        svgfastdiagram(nextSvg("5,0-10,0-100,0-X-C"), (*((mcf::hmc*)X.host))[0], False);
		
        // ---------------------------------------------------+
		x.host->init();
        SampleGenerator::addReal<MySample>(*(x.host), 5,    0);
		SampleGenerator::addReal<MySample>(*(x.host), 10,   5);
		SampleGenerator::addReal<MySample>(*(x.host), 100,  75);

		tra.invokeAndWait();

		svgfastdiagram(nextSvg("5,0-10,5-100,75-x"), (*((mcf::hmc*)x.host))[0], False);
		svgfastdiagram(nextSvg("5,0-10,5-100,75-X-P"), (*((mcf::hmc*)X.host))[0], True);
        svgfastdiagram(nextSvg("5,0-10,5-100,75-X-C"), (*((mcf::hmc*)X.host))[0], False);


        // ---------------------------------------------------+
		x.host->init();
		SampleGenerator::addReal<MySample>(*(x.host), 4,   0);
        SampleGenerator::addReal<MySample>(*(x.host), 8,   0);
        SampleGenerator::addReal<MySample>(*(x.host), 16,   0);
        SampleGenerator::addReal<MySample>(*(x.host), 32,   0);

		tra.invokeAndWait();

		svgfastdiagram(nextSvg("4-0,8-0,16-0,32-0-x"), (*((mcf::hmc*)x.host))[0], False);
		svgfastdiagram(nextSvg("4-0,8-0,16-0,32-0-X-P"), (*((mcf::hmc*)X.host))[0], True);
        svgfastdiagram(nextSvg("4-0,8-0,16-0,32-0-X-C"), (*((mcf::hmc*)X.host))[0], False);
        
        

	} TL_END_SESSION()

	TL_BEGIN_SESSION(DirectConvolution)
	{
        typedef fff::Factory::UbiMultiChannelFactory<MySample> mcf;

        OpenCLEnvironment &env = _.getEnv();
        mcf::multichannel x, X, h, H, hc, Hc, cy, cyc, y, Y, yc, Yc;

        mcf::create(x, env, CL_MEM_READ_WRITE, 1, 1024);
        mcf::create(X, env, CL_MEM_READ_WRITE, 1, 1024);

		mcf::create(h, env, CL_MEM_READ_WRITE, 1, 256);
        mcf::create(H, env, CL_MEM_READ_WRITE, 1, 256);

        mcf::create(hc, env, CL_MEM_READ_WRITE, 1, 1024);
        mcf::create(Hc, env, CL_MEM_READ_WRITE, 1, 1024);

		mcf::create(cy, env, CL_MEM_READ_WRITE, 1, 
            x.host->getSampleCount() +
            h.host->getSampleCount() - 1);

        mcf::create(cyc, env, CL_MEM_READ_WRITE, 1, 
            x.host->getSampleCount() +
            hc.host->getSampleCount() - 1);

        mcf::create(y, env, CL_MEM_READ_WRITE, 1,
            fff::cl::multiple2In(cy.host->getSampleCount()));
        mcf::create(Y, env, CL_MEM_READ_WRITE, 1,
            y.host->getSampleCount());

        mcf::create(yc, env, CL_MEM_READ_WRITE, 1,
            fff::cl::multiple2In(cyc.host->getSampleCount()));
        mcf::create(Yc, env, CL_MEM_READ_WRITE, 1,
            yc.host->getSampleCount());

		Convolution<MySample> conv(_, *x, *h, *cy);
        Convolution<MySample> convC(_, *x, *hc, *cyc);

        

		SampleGenerator::addReal<MySample>(
			*(x.host),
			30, 23);

        SampleGenerator::addReal<MySample>(
			*(x.host),
			17, 10);

        SampleGenerator::addReal<MySample>(
			*(x.host),
			5, 0);


        SampleGenerator::addReal<MySample>(
            *(h.host),
            30, 0);

        SampleGenerator::addReal<MySample>(
            *(h.host),
            17, 10);

        SampleGenerator::addReal<MySample>(
            *(hc.host),
            30, 0);

        SampleGenerator::addReal<MySample>(
            *(hc.host),
            17, 10);


		conv.invokeAndWait();
        convC.invokeAndWait();

        svgfastdiagram(nextSvg("30,23-17,10-5,0-x"),  (*((mcf::hmc*)x.host))[0], False);
		svgfastdiagram(nextSvg("30,0-17,10-h"),  (*((mcf::hmc*)h.host))[0], False);
		svgfastdiagram(nextSvg("30-17-y"),  (*((mcf::hmc*)cy.host))[0], False);

		svgfastdiagram(nextSvg("30,0-17,10-hc"),  (*((mcf::hmc*)hc.host))[0], False);
		svgfastdiagram(nextSvg("30-17-yc"),  (*((mcf::hmc*)cyc.host))[0], False);

        copySamplesCyclic(
            y.host->getRawReal(0),
            y.host->getSampleCount(),
            cy.host->getRawReal(0),
            cy.host->getSampleCount());

        copySamplesCyclic(
            y.host->getRawImag(0),
            y.host->getSampleCount(),
            cy.host->getRawImag(0),
            cy.host->getSampleCount());

        copySamplesCyclic(
            yc.host->getRawReal(0),
            yc.host->getSampleCount(),
            cyc.host->getRawReal(0),
            cyc.host->getSampleCount());

        copySamplesCyclic(
            yc.host->getRawImag(0),
            yc.host->getSampleCount(),
            cyc.host->getRawImag(0),
            cyc.host->getSampleCount());


		

        Transformation<MySample> hTra(_, *h, *H);
        Transformation<MySample> hcTra(_, *hc, *Hc);
        Transformation<MySample> xTra(_, *x, *X);
        Transformation<MySample> yTra(_, *y, *Y);
        Transformation<MySample> ycTra(_, *yc, *Yc);

        hTra.invokeAndWait();
        hcTra.invokeAndWait();
        xTra.invokeAndWait();
        yTra.invokeAndWait();
        ycTra.invokeAndWait();

        svgfastdiagram(nextSvg("30,23-17,10-5,0-X-P"),  (*((mcf::hmc*)X.host))[0], True);
        svgfastdiagram(nextSvg("30,23-17,10-5,0-X-C"),  (*((mcf::hmc*)X.host))[0], False);
		svgfastdiagram(nextSvg("30,0-17,10-H-P"),  (*((mcf::hmc*)H.host))[0], True);
        svgfastdiagram(nextSvg("30,0-17,10-H-C"),  (*((mcf::hmc*)H.host))[0], False);
		svgfastdiagram(nextSvg("30-17-Y-P"),  (*((mcf::hmc*)Y.host))[0], True);
        svgfastdiagram(nextSvg("30-17-Y-C"),  (*((mcf::hmc*)Y.host))[0], False);

        svgfastdiagram(nextSvg("30,0-17,10-Hc-P"),  (*((mcf::hmc*)Hc.host))[0], True);
        svgfastdiagram(nextSvg("30,0-17,10-Hc-C"),  (*((mcf::hmc*)Hc.host))[0], False);
		svgfastdiagram(nextSvg("30-17-Yc-P"),  (*((mcf::hmc*)Yc.host))[0], True);
        svgfastdiagram(nextSvg("30-17-Yc-C"),  (*((mcf::hmc*)Yc.host))[0], False);
		
	} TL_END_SESSION()
#endif