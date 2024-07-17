#!/usr/bin/ruby

require './ssutil_modules/misc'
require './ssutil_modules/projucer_project'

args = SSUtil.parse_args()

if args.should_do_update()
    ProjucerProject.update_projects()    
    p "Projects updated"
    args.done_update()    
end

if args.should_quit 
    abort()
end

base_jucer = ProjucerProject.get_current_dir_project()
version = base_jucer.get_version()

`rm -r ../release`
`mkdir -p ../release/MacOSX/AU`
`mkdir -p ../release/MacOSX/VST3`

# assuming projucer alias is set to point the projucer executable
jucer_projects = Dir.glob('../*/*.jucer')
jucer_projects.each do |p|
    puts "Resaving #{p}"
    `zsh -ic 'projucer --resave #{p}'`
end

projects = Dir.glob('../*/Builds/MacOSX/*.xcodeproj')

if projects.empty?
    puts "There are no xcode projects. Please run projucer on each project once to generate the xcode projects"
    return 
end

projects.each do |f|    
    if f.include?('SpectralGateBuild') || f.include?('Base') || f.include?('Playground') 
        next
    end            
    project_name = File.basename(f, '.xcodeproj')                
    build_dir = File.dirname(f) + "/build/Release"        
    
    puts "Cleaning project #{project_name}..."
    SSUtil::perform_and_print "xcodebuild clean -project '#{f}\' -scheme '#{project_name} - All' -configuration Release"
    
    puts "Building project #{project_name}..."
    SSUtil::perform_and_print "xcodebuild build -project '#{f}' -scheme '#{project_name} - All' -configuration Release"
        
    `cp -r "#{build_dir}/#{project_name}.component" "../release/MacOSX/AU/#{project_name}.component"`    
    `cp -r "#{build_dir}/#{project_name}.vst3" "../release/MacOSX/VST3/#{project_name}.vst3"`
end

`pandoc ../CHANGES.md -f markdown -t html -s -o PACKAGE_README.html`
`cp SpectralSuitePackager.pkgproj.template SpectralSuitePackager.pkgproj`
`sed -i '' 's/{VERSION}/#{version}/g' SpectralSuitePackager.pkgproj`
`packagesbuild SpectralSuitePackager.pkgproj`
