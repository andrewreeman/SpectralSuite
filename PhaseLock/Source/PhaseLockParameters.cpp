#include "PhaseLockParameters.h"
#include "PhaseLockInteractor.h"

PhaseLockParameters::ParameterKey PhaseLockParameters::KEY_PHASE_LOCK = ParameterID("phaselock", 1);
PhaseLockParameters::ParameterKey PhaseLockParameters::KEY_MAG_LOCK = ParameterID("maglock", 1);

PhaseLockParameters::ParameterKey PhaseLockParameters::KEY_PHASE_MIX = ParameterID("phasemix", 1);
PhaseLockParameters::ParameterKey PhaseLockParameters::KEY_MAG_MIX = ParameterID("magmix", 1);

PhaseLockParameters::ParameterKey PhaseLockParameters::KEY_MAG_TRACK = ParameterID("magtrack", 1);
PhaseLockParameters::ParameterKey PhaseLockParameters::KEY_RAND_PHASE = ParameterID("randphase", 1);

PhaseLockParameters::ParameterKey PhaseLockParameters::KEY_MORPH_MAG_AND_PHASE = ParameterID("morphmagphase", 1);
PhaseLockParameters::ParameterKey PhaseLockParameters::KEY_MORPH_DURATION = ParameterID("morphduration", 1);

PhaseLockParameters::PhaseLockParameters(AudioProcessor * processor) : 
	PluginParameters(processor)
{
    createAndAddParameter(std::make_unique<AudioParameterBool>(
		KEY_PHASE_LOCK,
		"Phase Lock",
		false
    ));
    
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        KEY_PHASE_MIX,
        "Phase mix",
        0.0,
        1.0,
        1.0
    ));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>(
       KEY_MAG_LOCK,
       "Frequency Lock",
       false
   ));
       
   createAndAddParameter(std::make_unique<AudioParameterFloat>(
       KEY_MAG_MIX,
       "Frequency mix",
       0.0,
       1.0,
       1.0
   ));
   
   createAndAddParameter(std::make_unique<AudioParameterFloat>(
       KEY_MAG_TRACK,
       "Magnitude Tracking",
       0.0,
       1.0,
       0.0
   ));
         
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
       KEY_RAND_PHASE,
       "Random phase",
       0.0,
       1.0,
       0.0
   ));
   
    createAndAddParameter(std::make_unique<AudioParameterBool>(
       KEY_MORPH_MAG_AND_PHASE,
       "Morph freq and phase",
       false
   ));
   
   createAndAddParameter(std::make_unique<AudioParameterInt>(
        KEY_MORPH_DURATION,
        "Morph duration",
        1,
        30,
        2
    ));
}
