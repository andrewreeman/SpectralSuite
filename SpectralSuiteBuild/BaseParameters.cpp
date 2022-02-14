#include "BaseParameters.h"

BaseParameters::BaseParameters(AudioProcessor * processor) :
    PluginParameters(processor)
{
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        "mix",
        "Generic mix parameter",
        0.0,
        1.0,
        1.0
    ));
}
