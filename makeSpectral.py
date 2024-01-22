from os import listdir, rename
from os.path import isfile, join, abspath, basename, splitext, dirname
from typing import override
from typing import TextIO, List
from shutil import copy, SameFileError
from argparse import ArgumentParser

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

class Cmd:
	def __init__(self, p : str, n : str, d : str, v : bool):
		self.path = p.pop() if p else None
		self.name = n.pop() if n else None
		self.description = d.pop() if d else None
		self.verbose = v.pop() if v else False

class Abstract_JucerFile:

	def __init__(self, name:str, desc:str, path:str):
		self.name = name
		self.desc = desc
		self.path = path
		self.diry = dirname(path)


	def swit(self, haystack:str, needle:str, spore:str) -> str :
		if haystack == "" or haystack is None:
			return None
		if not needle in haystack:
			raise ValueError("needle not found in haystack")
		return haystack.replace(needle, spore, 1)

	def manip(self, dt : str) -> str:
		return dt

	def rename_path(self, old_path : str) -> str:	
		new_file_name = self.swit(basename(old_path), "Base", self.name)
		new_file_path = join(self.diry, new_file_name)
		rename(old_path, new_file_path)
		return new_file_path

	def crank(self) -> str:
		print(self.path)
		with open(self.path, 'r+') as f:
			text = f.read()
			manip_text = self.manip(text)
			f.seek(0)
			f.write(manip_text)
			f.truncate()
		self.path = self.rename_path(self.path)
		return text

class Base(Abstract_JucerFile):

	def manip_description(self, dt:str) -> str:
		dt = self.swit(dt, 'pluginDesc="BaseDescription"', f'pluginDesc="{self.desc}"')
		dt = self.swit(dt, 'name="BaseTitle"', f'name="{self.name}"')
		return dt

	@override
	def manip(self, dt : str) -> str:
		if self.desc:
			dt = self.manip_description(dt)
		dt = self.swit(dt, 'name="BaseAudioPlugin.cpp"', f'name="{self.name}AudioPlugin.cpp"')
		dt = self.swit(dt, 'file="BaseAudioPlugin.cpp"', f'file="{self.name}AudioPlugin.cpp"')
		dt = self.swit(dt, 'name="BaseAudioPlugin.h"', f'name="{self.name}AudioPlugin.h"')
		dt = self.swit(dt, 'file="BaseAudioPlugin.h"', f'file="{self.name}AudioPlugin.h"')
		dt = self.swit(dt, 'name="BaseParameters.cpp"', f'name="{self.name}Parameters.cpp"')
		dt = self.swit(dt, 'file="BaseParameters.cpp"', f'file="{self.name}Parameters.cpp"')
		dt = self.swit(dt, 'name="BaseParameters.h"', f'name="{self.name}Parameters.h"')
		dt = self.swit(dt, 'file="BaseParameters.h"', f'file="{self.name}Parameters.h"')
		dt = self.swit(dt, 'name="BaseProcessor.cpp"', f'name="{self.name}Processor.cpp"')
		dt = self.swit(dt, 'file="BaseProcessor.cpp"', f'file="{self.name}Processor.cpp"')
		dt = self.swit(dt, 'name="BaseProcessor.h"', f'name="{self.name}Processor.h"')
		dt = self.swit(dt, 'file="BaseProcessor.h"', f'file="{self.name}Processor.h"')
		return dt

class AudioPluggin_Cpp(Abstract_JucerFile):
	@override
	def manip(self, dt : str) -> str:
		dt = self.swit(dt, '#include "BaseAudioPlugin.h"', f'#include "{self.name}AudioPlugin.h"')
		dt = self.swit(dt, 'std::make_shared<BaseParameters>', f'std::make_shared<{self.name}Parameters>')
		dt = self.swit(dt, 'std::make_unique<BaseProcessor>', f'std::make_unique<{self.name}Processor>')
		dt = self.swit(dt, 'std::shared_ptr<BaseParameters>', f'std::shared_ptr<{self.name}Parameters>')
		return dt

class AudioPluggin_H(Abstract_JucerFile):
	@override
	def manip(self, dt : str) -> str:
		dt = self.swit(dt, '#include "BaseProcessor.h"', f'#include "{self.name}Processor.h"')
		dt = self.swit(dt, '#include "BaseParameters.h"', f'#include "{self.name}Parameters.h"')
		return dt

class Parameters_Cpp(Abstract_JucerFile):
	@override
	def manip(self, dt : str) -> str:
		dt = self.swit(dt, '#include "BaseParameters.h"', f'#include "{self.name}Parameters.h"')
		dt = self.swit(dt, 
			'BaseParameters::BaseParameters(AudioProcessor * processor)', 
			f'{self.name}Parameters::{self.name}Parameters(AudioProcessor * processor)')
		return dt

class Parameters_H(Abstract_JucerFile):
	@override
	def manip(self, dt : str) -> str:
		dt = self.swit(dt, 
			'BaseParameters(AudioProcessor* processor);', 
			f'{self.name}Parameters(AudioProcessor* processor);')
		dt = self.swit(dt, 
			'class BaseParameters : public PluginParameters', 
			f'class {self.name}Parameters : public PluginParameters')
		return dt

class Processor_Cpp(Abstract_JucerFile):
	@override
	def manip(self, dt : str) -> str:
		dt = self.swit(dt, '#include "BaseProcessor.h"', f'#include "{self.name}Processor.h"')
		dt = self.swit(dt, 
			'std::unique_ptr<StandardFFTProcessor> BaseProcessor', 
			f'std::unique_ptr<StandardFFTProcessor>{self.name}Processor')
		dt = self.swit(dt, 
			'void BaseProcessor::prepareProcess(StandardFFTProcessor * spectralProcessor)', 
			f'void {self.name}Processor::prepareProcess(StandardFFTProcessor * spectralProcessor)')
		return dt

class Processor_H(Abstract_JucerFile):
	@override
	def manip(self, dt : str) -> str:
		dt = self.swit(dt, '#include "BaseParameters.h"', f'#include "{self.name}Parameters.h"')
		dt = self.swit(dt, 
			'class BaseProcessor : public SpectralAudioProcessorInteractor',
			f'class {self.name}Processor : public SpectralAudioProcessorInteractor')
		dt = self.swit(dt, 
			'BaseProcessor(int numOverlaps, std::shared_ptr<BaseParameters> params)', 
			f'{self.name}Processor(int numOverlaps, std::shared_ptr<BaseParameters> params)')
		dt = self.swit(dt, 'std::shared_ptr<BaseParameters> m_params', f'std::shared_ptr<{self.name}Parameters> m_params')
		return dt

class UiContainer_Cpp(Abstract_JucerFile):
	@override
	def manip(self, dt : str) -> str:
		dt = self.swit(dt, 
			'std::shared_ptr<BaseParameters> valueTreeState', 
			f'std::shared_ptr<{self.name}Parameters> valueTreeState')
		return dt

	@override
	def rename_path(self, old_path : str) ->  str:
		return old_path

class UiContainer_H(Abstract_JucerFile):
	@override
	def manip(self, dt : str) -> str:
		dt = self.swit(dt, '#include "BaseParameters.h"', f'#include "{self.name}Parameters.h"')
		dt = self.swit(dt, 
			'std::shared_ptr<BaseParameters> valueTreeState', 
			f'std::shared_ptr<{self.name}Parameters> valueTreeState')
		dt = self.swit(dt, 
			'std::shared_ptr<BaseParameters> pluginParameters', 
			f'std::shared_ptr<{self.name}Parameters> pluginParameters')
		return dt

	@override
	def rename_path(self, old_path : str) ->  str:
		return old_path

def get_jucebox(name : str, desc : str, directory : str) -> List[Abstract_JucerFile]:
	jucebox = []
	files = [f for f in listdir(directory)]
	for file_path in files:
		if file_path == "Base.jucer":
			jucebox.append(Base(name, desc, abspath(f'{directory}/{file_path}')))
		elif file_path == "BaseAudioPlugin.cpp":
			jucebox.append(AudioPluggin_Cpp(name, desc, abspath(f'{directory}/{file_path}')))
		elif file_path == "BaseAudioPlugin.h":
			jucebox.append(AudioPluggin_H(name, desc, abspath(f'{directory}/{file_path}')))
		elif file_path == "BaseParameters.cpp":
			jucebox.append(Parameters_Cpp(name, desc, abspath(f'{directory}/{file_path}')))
		elif file_path == "BaseParameters.h":
			jucebox.append(Parameters_H(name, desc, abspath(f'{directory}/{file_path}')))
		elif file_path == "BaseProcessor.cpp":
			jucebox.append(Processor_Cpp(name, desc, abspath(f'{directory}/{file_path}')))
		elif file_path == "BaseProcessor.h":
			jucebox.append(Processor_H(name, desc, abspath(f'{directory}/{file_path}')))
		elif file_path == "UiContainer.cpp":
			jucebox.append(UiContainer_Cpp(name, desc, abspath(f'{directory}/{file_path}')))
		elif file_path == "UiContainer.h":
			jucebox.append(UiContainer_H(name, desc, abspath(f'{directory}/{file_path}')))
		else:
			pass
	return jucebox

def get_args() -> ArgumentParser:
	parser = ArgumentParser(description='Automate SpectralSuite boilerplate for new plugin.')
	parser.add_argument('-p', '--path', type=str, nargs=1, required=True,
               			help='The relative or absolute path to the directory containing new plugin')
	parser.add_argument('-n', '--name', type=str, nargs=1, required=True,
               			help='The name of the new plugin')
	parser.add_argument('-d', '--description', type=str, nargs=1, required=False, 
               			default=None, help='The description of the new plugin')
	parser.add_argument('-v', '--verbose', nargs=1, required=False, default=True, 
						type=bool, choices=[True,False])
	return parser.parse_args()

def copy_boilerplate_files_to_new_plugin(new_plugin_directory : str) -> List[str]:
	errors = []
	boilerplate_files = ["Base.jucer",
					"BaseAudioPlugin.cpp",
					"BaseAudioPlugin.h",
					"BaseParameters.cpp",
					"BaseParameters.h",
					"BaseProcessor.cpp",
					"BaseProcessor.h",
					"UiContainer.cpp",
					"UiContainer.h"]
	build_directory = abspath('./SpectralSuiteBuild')
	build_files = [f for f in listdir(build_directory)]
	existing_files = [f for f in listdir(new_plugin_directory)]
	for f in build_files:
		if f in boilerplate_files  and f not in existing_files:
			try:
			    copy(join(build_directory, f), join(new_plugin_directory, f))
			except SameFileError:
			    errors.append(f"@{f} - Source and destination represents the same file.")
			except PermissionError:
			    errors.append(f"@{f} - Bad permission to write to the destination file.")
	return errors

def main():
	args = get_args()
	cmds = Cmd(args.path, args.name, args.description, args.verbose)

	copy_errors = copy_boilerplate_files_to_new_plugin(cmds.path)
	if cmds.verbose:
		print(f"Populated {cmds.path} with boilerplate files")
		if copy_errors:
			print("- the following files were unsuccessfully moved to f{cmds.path}:")
			for err in copy_errors:
				print(f" -- {err}")

	jucebox = get_jucebox(cmds.name, cmds.description, cmds.path)
	for j in jucebox:
		if cmds.verbose:
			print(f"Pushing provided values into {j} and renaming file")
		j.crank()

if __name__ == "__main__":
	main()