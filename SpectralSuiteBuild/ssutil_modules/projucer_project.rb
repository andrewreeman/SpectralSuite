require 'nokogiri'

module ProjucerProject    
    def ProjucerProject.update_projects
        base_jucer = ProjucerProject.get_current_dir_project()
        version = base_jucer.get_version()
        version_code = base_jucer.get_version_code()
        shared_group = base_jucer.get_shared_group()

        other_projects =  ProjucerProject.get_other_projects(base_jucer)
        if other_projects.empty?
            SSUtil.print_and_quit("No jucer projects found")    
        end

        other_projects.each { |p|
            p.set_version(version)
            p.set_version_code(version_code)
            #p.set_shared_group(shared_group)
        }
    end

    def ProjucerProject.get_current_dir_project
        project_path = Dir.glob('*.jucer')[0]
        return Project.new(project_path)
    end    

    def ProjucerProject.get_other_projects(project_to_file)
        Dir.glob("../**/*.jucer")
            .select{ |p| !project_to_file.same_project_name(p) } 
            .map{ |p| Project.new(p)}
    end    

    class Project
        def initialize(path)
            @path = path
        end

        def get_version()        
            xml = Nokogiri::XML(open(@path))
            return xml.root["version"]            
        end
        
        def set_version(new_version)
            xml = Nokogiri::XML(open(@path))
            xml.root['version'] = new_version

            File.write(@path, xml.to_xml)
            # xml.write_to(open(@path, 'w'))
        end

        def get_version_code()
            xml = Nokogiri::XML(open(@path))
            return xml.root["defines"]                                    
        end

        def set_version_code(new_version_code)
            xml = Nokogiri::XML(open(@path))
            xml.root['defines'] = new_version_code
            File.write(@path, xml.to_xml)
            # xml.write_to(open(@path, 'w'))
        end

        def get_shared_group()
            xml = Nokogiri::XML(open(@path))            
            return xml.xpath("//GROUP[@name='Shared']")[0].inner_html
        end

        def set_shared_group(new_shared_group)
            xml = Nokogiri::XML(open(@path))
            node = xml.xpath("//GROUP[@name='Shared']")[0]
            node.inner_html = new_shared_group            
            File.write(@path, xml.to_xml)
            # xml.write_to(open(@path, 'w'))                        
        end

        def same_project_name(s)
            s.include? @path
        end  
        
        def to_s
            return "Project path: #@path"
        end
    end
end
