add_rules("mode.debug", "mode.release")

target("test")
	add_files("src/*.cpp")

