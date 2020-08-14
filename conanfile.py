from conans import ConanFile, tools


class UnitConfConan(ConanFile):
    name = "unit_conf"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"
    description = "<Description of UnitConf here>"
    requires = "qt/5.14.2@bincrafters/stable"
    url = "None"
    license = "None"
    author = "None"
    topics = None
    generators = "cmake"

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)

    def package_id(self):
        pass
        # self.requires.unrelated_mode()
