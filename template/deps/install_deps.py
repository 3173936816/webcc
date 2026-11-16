import os
from optparse import OptionParser
import shutil
import platform


git_packages = [
	# git_url, folder_name, cmake_options[optional]
	["https://github.com/3173936816/lcy.git", "lcy", "-DLCY_BUILD_TESTS=OFF"],
	["https://github.com/nlohmann/json.git", "nlohmann", "-DJSON_BuildTests=OFF"],
	["https://github.com/leethomason/tinyxml2.git", "tinyxml2", "-Dtinyxml2_BUILD_TESTING=OFF"],
]


def package_already_in_sys(package_name) :
	sys_include_dirs = ["/usr/include", "/usr/local/include"]

	for include_dir in sys_include_dirs :
		if os.path.exists("{0}/{1}".format(include_dir, package_name)):
			return True
	return False


if __name__ == "__main__":
	os.makedirs("sources",exist_ok=True)
	os.makedirs("builds", exist_ok=True)
	os.makedirs("packages", exist_ok=True)

	for package in git_packages:
		if package_already_in_sys(package[1]):
			continue
			
		if not os.path.exists("./sources/{0}".format(package[1])):
			os.system("git clone --depth 1 {0} ./sources/{1}".format(package[0], package[1]))

		cmake_build_flags = ""
		if len(package) > 2:
			cmake_build_flags = package[2]

		os.makedirs("./builds/{0}".format(package[1]), exist_ok=True)
		os.system("cmake ./sources/{0} -B ./builds/{0}  -DCMAKE_PREFIX_PATH=./packages -DCMAKE_INSTALL_PREFIX=./packages {1}".format(package[1], cmake_build_flags))
		os.system("cmake --build ./builds/{0}  --parallel 8 --target install".format(package[1]))