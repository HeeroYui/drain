/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <audio/drain/Algo.hpp>
#ifdef HAVE_SPEEX_DSP_RESAMPLE
	#include <speex/speex_resampler.h>
#endif
#include <ememory/memory.hpp>

namespace audio {
	namespace drain {
		// data structure.
		class VolumeElement {
			public:
				VolumeElement(const std::string& _name="ERROR-VOLUME-NAME", float _volumedB=0.0f) :
				  m_name(_name),
				  m_volumedB(_volumedB),
				  m_mute(false) {
					
				}
			private:
				std::string m_name;
			public:
				std::string getName() const {
					return m_name;
				}
			private:
				float m_volumedB;
			public:
				float getVolume() const {
					return m_volumedB;
				}
				void setVolume(float _volumedB) {
					m_volumedB = _volumedB;
				}
			private:
				bool m_mute;
			public:
				bool getMute() const {
					return m_mute;
				}
				void setMute(bool _mute) {
					m_mute = _mute;
				}
		};
		// TODO: Optimisation
		// TODO: Zero crossing
		// TODO: Continuous update volume
		// TODO: Manage multiple volume
		// TODO: Manage set volume
		class Volume : public audio::drain::Algo {
			private:
				std::vector<ememory::SharedPtr<drain::VolumeElement> > m_volumeList;
				// for float input :
				float m_volumeAppli;
				// for integer input :
				int32_t m_volumeDecalage; // Volume to apply is simple as : X * m_coef >> m_coef
				int32_t m_volumeCoef;
				// convertion function:
				void (*m_functionConvert)(void* _input, void* _output, size_t _nbSample, int32_t _volumeCoef, int32_t _volumeDecalage, float _volumeAppli);
			protected:
				/**
				 * @brief Constructor
				 */
				Volume();
				void init();
			public:
				static ememory::SharedPtr<Volume> create();
				/**
				 * @brief Destructor
				 */
				virtual ~Volume();
			protected:
				virtual void configurationChange();
			public:
				virtual bool process(audio::Time& _time,
				                     void* _input,
				                     size_t _inputNbChunk,
				                     void*& _output,
				                     size_t& _outputNbChunk);
			public:
				virtual std::vector<audio::format> getFormatSupportedInput();
				virtual std::vector<audio::format> getFormatSupportedOutput();
			public:
				virtual void addVolumeStage(const ememory::SharedPtr<drain::VolumeElement>& _volume);
				virtual bool setParameter(const std::string& _parameter, const std::string& _value);
				virtual std::string getParameter(const std::string& _parameter) const;
				virtual std::string getParameterProperty(const std::string& _parameter) const;
			public:
				void volumeChange();
			public:
				virtual std::string getDotDesc();
		};
	}
}

