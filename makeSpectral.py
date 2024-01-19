import os
from dataclasses import dataclass
from typing import TextIO

"""
NOTE:
	Although a little more cumbersome, I feel that providing the our
	*replace substring* function extra verbosity is important.
	This is because the User's chosen Name and Description
	may occur in parts of the c++ code we do not want to edit.
	The inclusion of the context around the needle we want to substitute
	will help prevent accidental substitions.
Happy Coding. Pix ❤️
""" 

@dataclass
class Jucer:
	name: str
	desc: str
	file: TextIO

	def rep(self, haystack:str, needle:str, spore:str) -> str :
		if haystack == "" or haystack is None:
			return None
		if not needle in haystack:
			raise ValueError("needle not found in haystack")
		return repr(haystack).replace(repr(needle), repr(spore), 1)

	def manip_data(dt : str) -> str:
		return dt

	def crank() -> TextIO:
		dt = self.file.read()
		self.file.write(manip_data(dt))
		self.file.rename(self.name)
		return self.file

	def __exit__(self, *args):
		self.file.close()

@dataclass
class Base(Jucer):

	def manip_data(dt : str) -> str:
		dt = rep(dt, 'name="BaseAudioPlugin.cpp"', f'name="{self.name}AudioPlugin.cpp"')
		dt = rep(dt, 'file="BaseAudioPlugin.cpp"', f'file="{self.name}AudioPlugin.cpp"')
		dt = rep(dt, 'name="BaseAudioPlugin.h"', f'name="{self.name}AudioPlugin.h"')
		dt = rep(dt, 'file="BaseAudioPlugin.h"', f'file="{self.name}AudioPlugin.h"')
		dt = rep(dt, 'name="BaseParameters.cpp"', f'name="{self.name}Parameters.cpp"')
		dt = rep(dt, 'file="BaseParameters.cpp"', f'file="{self.name}Parameters.cpp"')
		dt = rep(dt, 'name="BaseParameters.h"', f'name="{self.name}Parameters.h"')
		dt = rep(dt, 'file="BaseParameters.h"', f'file="{self.name}Parameters.h"')
		dt = rep(dt, 'name="BaseProcessor.cpp"', f'name="{self.name}Processor.cpp"')
		dt = rep(dt, 'file="BaseProcessor.cpp"', f'file="{self.name}Processor.cpp"')
		dt = rep(dt, 'name="BaseProcessor.h"', f'name="{self.name}Processor.h"')
		dt = rep(dt, 'file="BaseProcessor.h"', f'file="{self.name}Processor.h"')
		return dt

@dataclass
class AudioPluggin_Cpp(Jucer):

	def manip_data(dt : str) -> str:
		dt = rep(dt, '#include "BaseAudioPlugin.h"', f'#include "{self.name}AudioPlugin.h"')
		dt = rep(dt, 'std::make_shared<BaseParameters>', f'std::make_shared<{self.name}Parameters>')
		dt = rep(dt, 'std::make_unique<BaseProcessor>', f'std::make_unique<{self.name}Processor>')
		dt = rep(dt, 'std::shared_ptr<BaseParameters>', f'std::shared_ptr<{self.name}Parameters>')
		return dt

@dataclass
class AudioPluggin_H(Jucer):

	def manip_data(dt : str) -> str:
		dt = rep(dt, '#include "BaseProcessor.h"', f'#include "{self.name}Processor.h"')
		dt = rep(dt, '#include "BaseParameters.h"', f'#include "{self.name}Parameters.h"')
		return dt

@dataclass
class Parrameters_Cpp(Jucer):

	def manip_data(dt : str) -> str:
		dt = rep(dt, '#include "BaseParameters.h"', f'#include "{self.name}Parameters.h"')
		dt = rep(dt, 
			'BaseParameters::BaseParameters(AudioProcessor * processor)', 
			f'{self.name}Parameters::{self.name}Parameters(AudioProcessor * processor)')
		return dt

@dataclass
class Parrameters_H(Jucer):

	def manip_data(dt : str) -> str:
		dt = rep(dt, 
			'BaseParameters(AudioProcessor* processor);', 
			f'{self.name}Parameters(AudioProcessor* processor);')
		dt = rep(dt, 
			'class BaseParameters : public PluginParameters', 
			f'class {self.name}Parameters : public PluginParameters')
		return dt

@dataclass
class Processor_Cpp(Jucer):

	def manip_data(dt : str) -> str:
		dt = rep(dt, '#include "BaseProcessor.h"', f'#include "{self.name}Processor.h"')
		dt = rep(dt, 
			'std::unique_ptr<StandardFFTProcessor> BaseProcessor', 
			f'std::unique_ptr<StandardFFTProcessor>{self.name}Processor')
		dt = rep(dt, 
			'void BaseProcessor::prepareProcess(StandardFFTProcessor * spectralProcessor)', 
			f'void {self.name}Processor::prepareProcess(StandardFFTProcessor * spectralProcessor)')
		return dt

@dataclass
class Processor_H(Jucer):

	def manip_data(dt : str) -> str:
		dt = rep(dt, '#include "BaseParameters.h"', f'#include "{self.name}Parameters.h"')
		dt = rep(dt, 
			'class BaseProcessor : public SpectralAudioProcessorInteractor',
			f'class {self.name}Processor : public SpectralAudioProcessorInteractor')
		dt = rep(dt, 
			'BaseProcessor(int numOverlaps, std::shared_ptr<BaseParameters> params)', 
			f'{self.name}Processor(int numOverlaps, std::shared_ptr<BaseParameters> params)')
		dt = rep(dt, 'std::shared_ptr<BaseParameters> m_params', f'std::shared_ptr<{NewThing}Parameters> m_params')
		return dt

@dataclass
class UiContainer_Cpp(Jucer):

	def manip_data(dt : str) -> str:
		dt = rep(dt, 
			'std::shared_ptr<BaseParameters> valueTreeState', 
			f'std::shared_ptr<{self.name}Parameters> valueTreeState')
		return dt

@dataclass
class UiContainer_H(Jucer):

	def manip_data(dt : str) -> str:
		dt = rep(dt, '#include "BaseParameters.h"', f'#include "{self.name}Parameters.h"')
		dt = rep(dt, 
			'std::shared_ptr<BaseParameters> valueTreeState', 
			f'std::shared_ptr<{self.name}Parameters> valueTreeState')
		dt = rep(dt, 
			'std::shared_ptr<BaseParameters> pluginParameters', 
			f'std::shared_ptr<{self.name}Parameters> pluginParameters')
		return dt

def main():
	pass


if __name__ == "__main__":
	main()