require 'nokogiri'

f = Dir.glob('*.jucer')[0]
xml = Nokogiri::Slop(open(f))
version = xml.html.body.jucerproject['version']

`mv ../release ../release.bak`
`mkdir ../release`

Dir.glob('../*/Builds/MacOSX/*.xcodeproj').each do |f|    
    if f.include?('SpectralGateBuild') || f.include?('Base.xcodeproj')        
        next
    end        
    project_name = File.basename(f, '.xcodeproj')    
    `xcodebuild clean -project "#{f}" -scheme "#{project_name} - AU" -configuration Release`    
    `xcodebuild build -project "#{f}" -scheme "#{project_name} - AU" -configuration Release`    
    `mv "../release/#{project_name}.component" "../release/#{project_name}_#{version}.component"`
end
