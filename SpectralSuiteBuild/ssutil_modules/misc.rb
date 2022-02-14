require 'Open3'
require 'optparse'

module SSUtil 
    def SSUtil.perform_and_print(command)       
        Open3.popen3(command) do |stdin, stdout, stderr, wait_thr|
            while line = stdout.gets
                puts line
            end
        end
    end

    def SSUtil.print_and_quit(message)
        p message
        abort()
    end

    def SSUtil.parse_args
        options = {}
        OptionParser.new do |opts| 
            opts.banner = "SSUtil will update, build and package all the projects. Usage: ssutil.rb [options]"

            opts.on("-u", "--update", "Update projects with contents of the base projects") do |v|
                options[:update] = v
            end

        end.parse!

        return Args.new options
    end

    class Args
        def initialize(args)
            @args = args
            @should_quit = false
        end

        def should_do_update
            @args.empty? || @args[:update]
        end 

        def done_update
            if @args[:update]
                @args.delete :update

                if @args.empty?
                    @should_quit = true
                end
            end
        end

        def should_quit
            @should_quit
        end
    end
end