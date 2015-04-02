/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license APACHE-2 (see license file)
 */

#include <ewol/ewol.h>
#include <appl/debug.h>
#include <appl/Windows.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Slider.h>

#undef __class__
#define __class__ "Windows"

appl::Windows::Windows() :
  m_sampleRate(48000) {
	addObjectType("appl::Windows");
	m_listSampleRate.push_back(192000);
	m_listSampleRate.push_back(176400);
	m_listSampleRate.push_back(96000);
	m_listSampleRate.push_back(88200);
	m_listSampleRate.push_back(48000);
	m_listSampleRate.push_back(44100);
	m_listSampleRate.push_back(32000);
	m_listSampleRate.push_back(22050);
	m_listSampleRate.push_back(16000);
	m_listSampleRate.push_back(11025);
	m_listSampleRate.push_back(8000);
	m_listSampleRate.push_back(4000);
	m_listGain.resize(10, 0.0f);
}

void appl::Windows::init() {
	ewol::widget::Windows::init();
	setTitle("Drain Equalizer Profiler");
	
	m_gui = ewol::widget::Composer::create(ewol::widget::Composer::file, "DATA:gui.xml");
	if (m_gui != nullptr) {
		setSubWidget(m_gui);
	}
	subBind(ewol::widget::Button, "sample-rate-low", signalPressed, shared_from_this(), &appl::Windows::onCallbackSampleRateLow);
	subBind(ewol::widget::Button, "sample-rate-up", signalPressed, shared_from_this(), &appl::Windows::onCallbackSampleRateUp);
	
	subBind(ewol::widget::Slider, "gain-0", signalChange, shared_from_this(), &appl::Windows::onCallbackGain0);
	subBind(ewol::widget::Slider, "gain-1", signalChange, shared_from_this(), &appl::Windows::onCallbackGain1);
	subBind(ewol::widget::Slider, "gain-2", signalChange, shared_from_this(), &appl::Windows::onCallbackGain2);
	subBind(ewol::widget::Slider, "gain-3", signalChange, shared_from_this(), &appl::Windows::onCallbackGain3);
	subBind(ewol::widget::Slider, "gain-4", signalChange, shared_from_this(), &appl::Windows::onCallbackGain4);
	subBind(ewol::widget::Slider, "gain-5", signalChange, shared_from_this(), &appl::Windows::onCallbackGain5);
	subBind(ewol::widget::Slider, "gain-6", signalChange, shared_from_this(), &appl::Windows::onCallbackGain6);
	subBind(ewol::widget::Slider, "gain-7", signalChange, shared_from_this(), &appl::Windows::onCallbackGain7);
	subBind(ewol::widget::Slider, "gain-8", signalChange, shared_from_this(), &appl::Windows::onCallbackGain8);
	subBind(ewol::widget::Slider, "gain-9", signalChange, shared_from_this(), &appl::Windows::onCallbackGain9);
	
	subBind(ewol::widget::Button, "display16", signalPressed, shared_from_this(), &appl::Windows::onCallbackStart16);
	subBind(ewol::widget::Button, "displayFloat", signalPressed, shared_from_this(), &appl::Windows::onCallbackStartFloat);
	m_displayer = std11::dynamic_pointer_cast<appl::widget::DisplayFrequency>(getSubObjectNamed("displayer"));
	onCallbackStart();
}


void appl::Windows::onCallbackSampleRateUp() {
	for (int32_t iii=0; iii<m_listSampleRate.size(); ++iii) {
		if (m_sampleRate == m_listSampleRate[iii]) {
			iii++;
			if (iii<m_listSampleRate.size()) {
				m_sampleRate = m_listSampleRate[iii];
			} else {
				m_sampleRate = m_listSampleRate[0];
			}
			ewol::parameterSetOnObjectNamed("sample-rate", "value", etk::to_string(m_sampleRate));
			onCallbackStart();
			return;
		}
	}
	m_sampleRate = m_listSampleRate[0];
	ewol::parameterSetOnObjectNamed("sample-rate", "value", etk::to_string(m_sampleRate));
	onCallbackStart();
}

void appl::Windows::onCallbackSampleRateLow() {
	for (int32_t iii=0; iii<m_listSampleRate.size(); ++iii) {
		if (m_sampleRate == m_listSampleRate[iii]) {
			iii--;
			if (iii>=0) {
				m_sampleRate = m_listSampleRate[iii];
			} else {
				m_sampleRate = m_listSampleRate[m_listSampleRate.size()-1];
			}
			ewol::parameterSetOnObjectNamed("sample-rate", "value", etk::to_string(m_sampleRate));
			onCallbackStart();
			return;
		}
	}
	m_sampleRate = m_listSampleRate[0];
	ewol::parameterSetOnObjectNamed("sample-rate", "value", etk::to_string(m_sampleRate));
	onCallbackStart();
}

void appl::Windows::onCallbackGain(const float& _value, int32_t _id) {
	m_listGain[_id] = _value;
	onCallbackStart();
}


#include <river/debug.h>

std11::shared_ptr<drain::Equalizer> appl::Windows::createEqualizer(enum audio::format _format) {
	std::vector<audio::channel> map;
	map.push_back(audio::channel_frontCenter);
	drain::IOFormatInterface format(map, _format, m_sampleRate);
	// create equalizer
	std11::shared_ptr<drain::Equalizer> out = drain::Equalizer::create();
	// configure input
	out->setInputFormat(format);
	// configure output
	out->setOutputFormat(format);
	// create ejson:
	std::string conf;
	conf += "{\n";
	conf += "	global: [\n";
	for (size_t iii=3; iii<m_listGain.size(); ++iii) {
		float cutFrequency = 49000;
		switch (iii) {
			case 0:cutFrequency = 31.25; break;
			case 1:cutFrequency = 62.5; break;
			case 2:cutFrequency = 125; break;
			case 3:cutFrequency = 250; break;
			case 4:cutFrequency = 500; break;
			
			case 5:cutFrequency = 1000; break;
			case 6:cutFrequency = 2000; break;
			case 7:cutFrequency = 4000; break;
			case 8:cutFrequency = 8000; break;
			case 9:cutFrequency = 16000; break;
		}
		
		if (iii != 0) {
			conf += "		,\n";
		}
		conf += "		{\n";
		conf += "			type:'peak',\n";
		conf += "			quality:2,\n";
		conf += "			cut-frequency:" + etk::to_string(cutFrequency) + ",\n";
		conf += "			gain:" + etk::to_string(m_listGain[iii]) + "\n";
		conf += "		}\n";
	}
	conf += "	]\n";
	conf += "}\n";
	APPL_INFO("config : " << conf);
	// configure:
	out->setParameter("config", conf);
	return out;
}

void appl::Windows::onCallbackStart() {
	APPL_INFO("start ");
	std11::shared_ptr<drain::Equalizer> eq = appl::Windows::createEqualizer();
	std::vector<std::pair<float,float> > theory = eq->calculateTheory();
	m_displayer->clear();
	m_displayer->setValue(theory);
}


void appl::Windows::onCallbackStart16() {
	APPL_INFO("start ");
	std11::shared_ptr<drain::Equalizer> eq = appl::Windows::createEqualizer(audio::format_int16);
	std::vector<std::pair<float,float> > pratic;
	size_t len = 512;
	std::vector<int16_t> data;
	data.resize(16000, 0);
	for (size_t iii=1; iii < len; iii++) {
		float freq = iii / (len - 1.0) * m_sampleRate / 2.0;
		// To reset filter
		eq->setParameter("reset", "");
		double m_phase = 0;
		double baseCycle = 2.0*M_PI/double(m_sampleRate) * double(freq);
		float gain = 0;
		for (int32_t iii=0; iii<data.size(); iii++) {
			data[iii] = cos(m_phase) * 32000;
			m_phase += baseCycle;
			if (m_phase >= 2*M_PI) {
				m_phase -= 2*M_PI;
			}
		}
		// process
		int16_t* output = nullptr;
		void* outputVoid = nullptr;
		size_t outputNbChunk = 0;
		std11::chrono::system_clock::time_point time;
		RIVER_SAVE_FILE_MACRO(int16_t, "aaa_test_INPUT.raw", &data[0], data.size());
		eq->process(time, &data[0], data.size(), outputVoid, outputNbChunk);
		output = static_cast<int16_t*>(outputVoid);
		RIVER_SAVE_FILE_MACRO(int16_t, "aaa_test_OUTPUT.raw", output, outputNbChunk);
		int16_t value = 0;
		for (size_t iii=800; iii<outputNbChunk-200; ++iii) {
			value = std::max(value, output[iii]);
		}
		gain = 20.0 * std::log10(double(value)/32000.0);
		APPL_VERBOSE("LEVEL " << iii << " out = " << value << " % : " << gain);
		pratic.push_back(std::make_pair<float, float>(float(freq),float(gain)));
	}
	m_displayer->setValue(pratic);
}

void appl::Windows::onCallbackStartFloat() {
	APPL_INFO("start ");
	std11::shared_ptr<drain::Equalizer> eq = appl::Windows::createEqualizer(audio::format_float);
	std::vector<std::pair<float,float> > pratic;
	size_t len = 512;
	std::vector<float> data;
	data.resize(16000, 0);
	for (size_t iii=1; iii < len; iii++) {
		float freq = iii / (len - 1.0) * m_sampleRate / 2.0;
		// To reset filter
		eq->setParameter("reset", "");
		double m_phase = 0;
		double baseCycle = 2.0*M_PI/double(m_sampleRate) * double(freq);
		float gain = 0;
		for (int32_t iii=0; iii<data.size(); iii++) {
			data[iii] = cos(m_phase);
			m_phase += baseCycle;
			if (m_phase >= 2*M_PI) {
				m_phase -= 2*M_PI;
			}
		}
		// process
		float* output = nullptr;
		void* outputVoid = nullptr;
		size_t outputNbChunk = 0;
		std11::chrono::system_clock::time_point time;
		RIVER_SAVE_FILE_MACRO(int16_t,"aaa_test_INPUT_F.raw",&data[0],data.size());
		eq->process(time, &data[0], data.size(), outputVoid, outputNbChunk);
		output = static_cast<float*>(outputVoid);
		RIVER_SAVE_FILE_MACRO(int16_t,"aaa_test_OUTPUT_F.raw",output, outputNbChunk);
		float value = 0;
		for (size_t iii=800; iii<outputNbChunk-200; ++iii) {
			value = std::max(value, output[iii]);
		}
		gain = 20.0 * std::log10(double(value)/1.0);
		APPL_VERBOSE("LEVEL " << iii << " out = " << value << " % : " << gain);
		pratic.push_back(std::make_pair<float, float>(float(freq),float(gain)));
	}
	m_displayer->setValue(pratic);
}