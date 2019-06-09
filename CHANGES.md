- Version 2.0.2
	* With AU plugins
	* Plugins load correctly in REAPER
	* Should now be able to support more than 2 channels (untested)
	* Frequency Shift now increments in 1hz instead of 10hz
	* About page has contact details
	* Plugins load correctly on Max MSP
	* Can load multiple plugins on FL Studio


- Version 2.0.1
	* Fixed bug in frequency magnet producing incorrect bass frequencies

	* Can now specify the random seed in the bin scrambler making this deterministic. 
	0 is a seed based on time effectively making this non-deterministic.
	Can select this using the settings button.

	* Spectral Gate cut off is now shown in dB

	* Frequency shifter can now select the max and minimum frequency range. Can select this using the settings button.

- Version 2.0.0
	* Rewrote plugins using the JUCE framework. https://juce.com/