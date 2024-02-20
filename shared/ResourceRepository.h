/*
  ==============================================================================

    ResourceRepository.h
    Created: 17 Feb 2024 8:04:46am
    Author:  Andrew Reeman

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class ResourceRepository {
public:
    Drawable* getNavigateBackIcon() {
        if(navigateBackIcon == nullptr)
        {
            navigateBackIcon = Drawable::createFromImageData(BinaryData::baselinearrow_back24px_svg, BinaryData::baselinearrow_back24px_svgSize);
            navigateBackIcon->replaceColour(Colours::black, Colours::white);
        }
                    
        return navigateBackIcon.get();
    }
private:
    std::unique_ptr<Drawable> navigateBackIcon;
};
