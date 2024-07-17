#include "FrequencyMagnetFFTProcessor.h"
#include "../../shared/utilities.h"

void FrequencyMagnetFFTProcessor::spectral_process(const PolarVector &in, PolarVector &out, int bins) {
    /*Frequency magnet dynamically merges all bins to a single bin. It is intended to explore spectral width. The x-axis controls the `width'.
    The y-axis controls the target frequency. When the width is zero all frequencies are shifted to a single bin.
    When the width is one the full un-shifted spectrum is presented. The interest comes from the range in between the extremes.
    When the width is at 0.5 the bins below the target frequency are shifted up in varying amounts.
    The bins above the target frequency are shifted lower by varying amounts. So we can hear a focus upon a single frequency emerge. */

    // we will use the width bias input to limit and curve the width paramter

    const float widthBias = m_widthBias*3.f; //this will be the exponent for the width
    const float widthLimit = 1.f-pow(m_widthBias, 0.125f); // this is the lower limit for the width (less width, narrower band ... higher volume!)
    
    int target_bin = (int)((m_freq / (float)this->getSampleRate()) * (float)m_fftSize);
    if (target_bin > bins)
    {
        for(int i=0; i<bins; ++i)
        {
            out[i] = in[i];
        }
        
        return;
    }
    float width = m_width;
    width = utilities::clip(width, widthLimit, 1.f);
    width = pow(width, widthBias);

    float line;
    float index_below;
    float index_above;

    std::vector<Polar<FftDecimal> > temp(bins);
    utilities::emptyPolar(temp);
    utilities::emptyPolar(out);

    long nextIndexBelow;
    //This loop will work on the frequencies below the target freq.
    for(int i=0; i<target_bin; ++i){
        line = float(i)/target_bin;
        // The less width the more convex shape the line will become. Biasing towards one value. If width is zero index will always be one.
        
        // scale
        index_below = (pow(line, width*8.f / 8.f) * (float)target_bin);

        temp[(int)index_below].m_mag += in[i].m_mag;
        out[i].m_phase = in[i].m_phase;
        
        nextIndexBelow = (long)index_below + 1l;
        out[(int)index_below].m_mag = utilities::interp_lin(temp[(int)index_below].m_mag, temp[nextIndexBelow].m_mag, (float)index_below);
    }

    if(target_bin < 1) target_bin = 1;

    const int range = bins - target_bin;
    
    long nextIndexAbove;
    for(int i=target_bin; i<bins; ++i){
        /*out[i].m_phase = in[i].m_phase;
        out[i].m_mag = in[i].m_mag;
        continue;*/
        
        // norm_index will run from 0 - Range.
        int norm_index = i-target_bin;
        line = float(norm_index)/float(range);
        
        // scale
        index_above = (pow(line, ((1.f - width)*7.f) + 1.f)  * (float)range) ;
        // offset by the target frequency.
        if (!m_useLegacyHighFrequencyShift) {
            index_above += (float)target_bin;
        }

        index_above = utilities::clip(index_above, 1.f, out.size() - 1.f);

        temp[(int)index_above].m_mag += in[i].m_mag;
        out[i].m_phase = in[i].m_phase;

        nextIndexAbove = (long)index_above - 1l;
        out[(int)index_above].m_mag = utilities::interp_lin(temp[(int)index_above].m_mag, temp[nextIndexAbove].m_mag, index_above);
    }
}
