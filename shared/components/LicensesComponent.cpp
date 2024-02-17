
#include "LicensesComponent.h"


//==============================================================================
LicensesComponent::LicensesComponent(ResourceRepository& resources)
    : backButton("backButton", DrawableButton::ButtonStyle::ImageFitted)
{
    
    backButton.setImages(resources.getNavigateBackIcon());
    backButton.onClick = [this]() {
        this->setVisible(false);
    };
        
    configureLicenseView();
    addAndMakeVisible(&backButton);
    addAndMakeVisible(&licenses);
}

LicensesComponent::~LicensesComponent()
{
}

void LicensesComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void LicensesComponent::resized()
{
    static const int padding = 4;
    static const int componentSpacing = padding * 3;
    
    backButton.setBounds(0, 0, 40, 40);
    auto y = backButton.getBottom() + componentSpacing;

    licenses.setBounds(padding, y, getWidth() - padding, getHeight() - componentSpacing);
}


void LicensesComponent::configureLicenseView() {
     String licenseInfo = "====\n"
    "This product uses KissFFT (https://github.com/mborgerding/kissfft)\n"
    "developed by Mark Borgerding\n\n"
    
    "KissFFT license:\n\n"
    
    "Valid-License-Identifier: BSD-3-Clause\n"
    "SPDX-URL: https://spdx.org/licenses/BSD-3-Clause.html\n"
    "Usage-Guide:\n"
      "\tTo use the BSD 3-clause \"New\" or \"Revised\" License put the following SPDX\n"
      "\ttag/value pair into a comment according to the placement guidelines in\n"
      "\tthe licensing rules documentation:\n"
        "\t\tSPDX-License-Identifier: BSD-3-Clause\n"
    "License-Text:\n\n"
    
    "Copyright (c) <year> <owner> . All rights reserved.\n\n"

    "Redistribution and use in source and binary forms, with or without modification,\n"
    "are permitted provided that the following conditions are met:\n\n"

    "1. Redistributions of source code must retain the above copyright notice,\n"
    "this list of conditions and the following disclaimer.\n\n"

    "2. Redistributions in binary form must reproduce the above copyright notice,\n"
    "this list of conditions and the following disclaimer in the documentation\n"
    "and/or other materials provided with the distribution.\n\n"

    "3. Neither the name of the copyright holder nor the names of its contributors\n"
    "may be used to endorse or promote products derived from this software without\n"
    "specific prior written permission.\n\n"

    "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n"
    "AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n"
    "IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n"
    "ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE\n"
    "LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"
    "DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n"
    "SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n"
    "CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,\n"
    "OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE\n"
    "USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
    "====\n";

    licenses.setMultiLine(true);
    licenses.setEnabled(false);
    licenses.setText(licenseInfo, NotificationType::dontSendNotification);
    licenses.setFont(16.0);
}
