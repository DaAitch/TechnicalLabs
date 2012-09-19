
#ifndef __samplegenerator_h__
#	define __samplegenerator_h__

#	include <fff.h>

using namespace fff;
using namespace fff::Buffer::Complex::Host;
using namespace fff::Buffer::Complex::Device;
using namespace fff::Buffer::Complex::Ubiquitous;

namespace TechnicalLabs
{
	namespace Tools
	{
		

		class SampleGenerator
		{
		public:
			template<class SampleType>
			static void add(
				SampleType *samples,
				UInt sampleCount,
				SampleType samplesPerPeriod,
				SampleType phaseSamples=(SampleType)0)
			{
				SampleType radPerPeriod = fff_M_PI * SampleType(2);

				for(
					UInt i = 0;
					i < sampleCount;
					++i)
				{
					samples[i] += cos(
						SampleType(i) * radPerPeriod / samplesPerPeriod +  // frequency [samples]*[rad/period]/[samples/period]
						(phaseSamples / samplesPerPeriod * radPerPeriod)); // phase [samples]/[samples/period]*[rad/period]
				}
			}

            template<
				class SampleType
			>
			static void addReal(
				HostSingleChannel<SampleType> &samples,
				SampleType samplesPerPeriod,
				SampleType phaseSamples=(SampleType)0)
			{
					add<SampleType>(
						samples.getReal().getSamples(),
						samples.getSampleCount(),
						samplesPerPeriod,
						phaseSamples);
			}

			template<
				class SampleType
			>
			static void addReal(
				IHostMultiChannel<SampleType> &samples,
				SampleType samplesPerPeriod,
				SampleType phaseSamples=(SampleType)0)
			{
				for(
					UInt channel = 0;
					channel < samples.getChannelCount();
					++channel)
				{
					add<SampleType>(
						samples.getRawReal(channel),
						samples.getSampleCount(),
						samplesPerPeriod,
						phaseSamples);
				}
			}
		};
	}
}

#endif