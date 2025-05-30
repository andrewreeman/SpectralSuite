#include "FrequencyMagnetInteractor.h"
#include "FrequencyMagnetFFTProcessor.h"

void FrequencyMagnetInteractor::prepareProcess(StandardFFTProcessor* spectralProcess)
{
	auto mag = (FrequencyMagnetFFTProcessor*)spectralProcess;
	mag->setFrequency(*m_freq);
	mag->setWidthBias(*m_bias);
	mag->setWidth(*m_width);		
	bool value = (bool)m_params->getUseLegacyLogicValue().getValue();
	mag->setUseLegacyHighFrequencyShift(value);
}

std::unique_ptr<StandardFFTProcessor> FrequencyMagnetInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int, int)
{
	return std::make_unique<FrequencyMagnetFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate, this->getPhaseBuffer());
}

void FrequencyMagnetInteractor::receivedMidi(MidiBuffer& midi) {
    MidiMessage message;

    for (const MidiMessageMetadata metadata : midi)
    {
        message = metadata.getMessage();
        if(message.isNoteOn()) {
            m_params->setFrequency((float)MidiMessage::getMidiNoteInHertz(message.getNoteNumber()));
        }
        else if(message.isControllerOfType(1)) { // mod wheel controller
            m_params->setWidth((float)message.getControllerValue() / 127.f);
        }
    }
}
