import os
import re

from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.env import VirtualBuildEnv, VirtualRunEnv
from conan.tools.files import load, update_conandata
from conan.tools.scm import Git

from inspect import getsourcefile

class libreflectConan(ConanFile):
    # package reference
    author = "Aurélien Questel / questela@gmail.com"
    url = "https://github.com/KobNael/nael_utils"
    description = "C++ Toolbox"

    # metadata
    # binary model
    package_type = "library"
    settings = "os", "compiler", "build_type", "arch"
    options = {
            "shared": [True, False],
            "fPIC": [True, False],
            "coverage": ["ON","OFF"],
            "cppcheck": ["ON","OFF"],
            "valgrind": ["ON","OFF"]
        }
    default_options = {
            "shared": False,
            "fPIC": True,
            "coverage": "OFF",
            "cppcheck": "OFF",
            "valgrind": "OFF"
        }

    # folders and layout
    no_copy_source = True

    def _get_infos(self):
        try:
            # TODO how should we do better than this ?
            recipe_root = os.path.dirname(getsourcefile(lambda:0))
            content = load(self, os.path.join(recipe_root, "CMakeLists.txt"))
            infos = re.search(r"project\s*\(\s*(\w+)\s*\bVERSION\s*(\d+(\.\d+)*)", content, re.DOTALL)
            name = infos.group(1).strip()
            version = infos.group(2).strip()
            return name, version
        except Exception:
            return None,None

    #def init(self):

    def set_name(self):
        self.name = self._get_infos()[0]

    def set_version(self):
        self.version = self._get_infos()[1]

    def export(self):
        git = Git(self, self.recipe_folder)
        # save the url and commit in conandata.yml
        git.coordinates_to_conandata()

    #def export_sources(self):

    # init is called again when doing some conan create.

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    #def configure(self):

    def _get_build_dir(self):
        system = str(self.settings.os)
        compiler = "".join(str(self.settings.compiler).split())
        compiler_version = str(self.settings.compiler.version)
        arch = str(self.settings.arch)
        build_type = str(self.settings.build_type)
        return os.path.join("builds", system, compiler, compiler_version, arch, build_type)

    def layout(self):
        build_dir = self._get_build_dir()
        cmake_config_path = os.path.join("lib", "cmake", f"{self.name}-{self.version}")

        # set folders
        self.folders.build = build_dir
        self.folders.generators = os.path.join(self.folders.build, "generators")

        # set information for package consumption
        self.cpp.package.builddirs = [cmake_config_path]

        # set information for editable consumption
        self.cpp.build.builddirs = [os.path.join("install", f"{self.name}-{self.version}", cmake_config_path)]

    def requirements(self):
        self.requires("boost/1.91.0", transitive_headers=True)
        self.requires("gtest/1.17.0", transitive_headers=True)

    def package_id(self):
        self.info.clear()

    def validate(self):
        check_min_cppstd(self, "20")

    #def validate_build(self):

    #def build_requirements(self):
        #self.build_requires("cmakemodules/2.3.5@eurodecision/stable")

    #def build_id(self):

    def source(self):
        # we recover the saved url and commit from conandata.yml and use them to get sources
        git = Git(self)
        git.checkout_from_conandata_coordinates()

    def generate(self):
        be = VirtualBuildEnv(self)
        be.generate()

        re = VirtualRunEnv(self)
        re.generate()

        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.variables[self.name+"_WITH_COVERAGE"] = self.options.coverage
        tc.variables[self.name+"_WITH_CPPCHECK"] = self.options.cppcheck
        tc.variables[self.name+"_WITH_VALGRIND"] = self.options.valgrind
        tc.variables[self.name+"_AS_SUBMODULE"] = "OFF"
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        #cmake.test(target="run_unit_tests")

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "none")

    # undefined order

    #def system_requirements(self):

    #def compatibility(self):

    #def deploy(self):

