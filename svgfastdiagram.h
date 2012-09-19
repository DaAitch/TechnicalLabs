
#ifndef __svgfastdiagram_h__
#	define __svgfastdiagram_h__

#	include "usefff.h"
#	include <fstream>
#	include <string>
#	include <sstream>
#	include <algorithm>
#	include <iomanip>

#	define SVG_SIZE 200.f
#	define SVG_WIDTH 1000.f

	using namespace fff;
    using namespace fff::Buffer::Complex::Host;



	class svgfastdiagram
	{

	public:
		template<
			class SampleType
		>
		svgfastdiagram(
			const String &filename,
			const HostSingleChannel<SampleType> &samples,
			Bool freq,
            Bool markMaxima=False
			)
		{
			HostSingleChannel<SampleType> *freqSmp = NULL;
			const HostSingleChannel<SampleType> *smp = &samples;

			if(freq)
			{
				freqSmp = new HostSingleChannel<SampleType>(
				samples.getSampleCount());

				const SampleType
					*fromRe = samples.getReal().getSamples(),
					*fromIm = samples.getImag().getSamples();

				SampleType
					*toRe = freqSmp->getReal().getSamples(),
					*toIm = freqSmp->getImag().getSamples();

				for(
					UInt i = 0;
					i < samples.getSampleCount();
					++i)
				{
					toRe[i] = sqrt(
						fromRe[i] * fromRe[i] + fromIm[i] * fromIm[i]);
					toIm[i] = atan2(
						fromIm[i],
						fromRe[i]);
				}

				smp = freqSmp;
			}

			std::fstream f(filename.c_str(), ios::out | ios::binary);
				
			f << std::setiosflags(ios::fixed) << std::setprecision(5);

			f << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";
			f << "<g stroke-width=\"0.6\">";
			const SampleType *real = smp->getReal().getSamples();
			const SampleType *imag = smp->getImag().getSamples();

			SampleType minReal = -.0000001f, maxReal = .0000001f;
			SampleType minImag = -.0000001f, maxImag = .0000001f;

			for(
				UInt i = 0;
				i < smp->getSampleCount();
				++i)
			{
				minReal = min(minReal, real[i]);
				maxReal = max(maxReal, real[i]);
				minImag = min(minImag, imag[i]);
				maxImag = max(maxImag, imag[i]);
			}

			SampleType diffReal = maxReal - minReal;
			SampleType diffImag = maxImag - minImag;

			SampleType yRe0 = SVG_SIZE - (-minReal / diffReal * SVG_SIZE);
			SampleType yIm0 = SVG_SIZE - (-minImag / diffImag * SVG_SIZE);

			f << "<path fill=\"none\" stroke=\"#000000\" d=\"M0 ";
			f << yRe0 << " L" << SVG_WIDTH << " " << yRe0;
			f << "\" />";

			f << "<path fill=\"none\" stroke=\"#000000\" d=\"M0 ";
			f << (SVG_SIZE+yIm0) << " L" << SVG_WIDTH << " " << (SVG_SIZE+yIm0);
			f << "\" />";

            f << "<path fill=\"none\" stroke=\"#000000\" d=\"M" << (SVG_WIDTH/2);
            f << " 0 L" << (SVG_WIDTH/2) << " " << (2*SVG_SIZE) << "\" />";


            // Phase Values
            if(freq)
            {
                for(Int i = -1; i <=1; ++i)
                {
                    SampleType phase = (SampleType)i/2.f * fff_M_PI;

                    SampleType phaseY = SVG_SIZE - ((phase - minImag) / diffImag * SVG_SIZE);

                    if(phaseY > 0.f && phaseY < SVG_SIZE)
                    {
                        f << "<path fill=\"none\" stroke=\"#550000\" d=\"M0 ";
			            f << (SVG_SIZE+phaseY) << " L" << SVG_WIDTH << " " << (SVG_SIZE+phaseY);
			            f << "\" />";
                    }
                }
            }


			f << "<path fill=\"none\" stroke=\"#000000\" d=\"M0 ";
			f << yRe0 << " ";


            

            std::vector<UInt> bins;

			UInt sampleCount = smp->getSampleCount();
			SampleType xproj = SVG_WIDTH/(SampleType)sampleCount;

            SampleType rel[3] = {0,0,0};

			for(
				UInt i = 0;
				i < smp->getSampleCount();
				++i)
			{
                if(markMaxima)
                {
                    rel[0] = rel[1];
                    rel[1] = rel[2];
                    rel[2] = real[i];

                    // relative maxima
                    if(rel[0]+0.05f < rel[1] && rel[1] > rel[2]+0.05f)
                        bins.push_back(i-1);
                }

				SampleType s = SVG_SIZE - ((real[i] - minReal) / diffReal * SVG_SIZE);
				f << " L" << ((SampleType)i*xproj) << " " << s;
			}

			f << "\" />";

			f << "<path fill=\"none\" stroke=\"#000000\" d=\"M0 ";
			f << (SVG_SIZE+yIm0) << " ";

			for(
				UInt i = 0;
				i < smp->getSampleCount();
				++i)
			{
				SampleType s = SVG_SIZE - ((imag[i] - minImag) / diffImag * SVG_SIZE);
				f << " L" << ((SampleType)i*xproj) << " " << (SVG_SIZE+s) << " ";
			}

			f << "\" />";

            
            if(markMaxima)
            {
                for(std::vector<UInt>::const_iterator ci = bins.cbegin(); ci != bins.cend(); ++ci)
                {
                    SampleType ix = xproj*((SampleType)(*ci));
                    f << "<path fill=\"none\" stroke=\"#000000\" d=\"M" << ix;
                    f << " 0 L" << ix << " " << (2*SVG_SIZE) << "\" />";
                }
            }

			f << "</g>";
			
			f << "<text x=\"" << SVG_WIDTH << "\" dx=\"10\" y=\"" << yRe0 << "\">";
			f << "[" << minReal << ";" << maxReal << "]";
			f << "</text>";
				
			f << "<text x=\"" << SVG_WIDTH << "\" dx=\"10\" y=\"" << (SVG_SIZE+yIm0) << "\">";
			f << "[" << minImag << ";" << maxImag << "]";
			f << "</text>";
					
			f << "</svg>";
			f.close();

			if(freqSmp != NULL)
				delete freqSmp;
		}

	private:
	};

#endif