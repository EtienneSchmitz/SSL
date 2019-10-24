# SSL - AI

## Development Guidelines

The code follows the guideline describe in ROS. [ROS_Guideline](http://wiki.ros.org/CppStyleGuide)

### Format

We using a .clang-format file. Caution ! Use the clang-format version 6.0, if your OS doesn't install it automatically then install clang-format-6.0 and rename the command to use the format script.

## Installation

### System dependencies

You will need theses packets:

``` bash
sudo apt-get install -y g++ gnuplot python-pip clang-format-6.0 cmake ninja-build
```

### Setup conan package manager

Install [conan](https://docs.conan.io/en/latest/):

``` bash
# Install conan  
sudo pip3 install --user conan

# Install NAMEC configuration of conan  
conan config install http://gitlab.namec.fr/ssl/conan-configuration.git
```

## Build

By default all targets are build in **debug** mode.
You can specify a [profiles](https://docs.conan.io/en/latest/reference/profiles.html) when you install conan dependencies by adding `-pr=<profile_name>` after the command `conan install ..` .

Check: http://gitlab.namec.fr/ssl/conan-configuration.git to see all available profiles.

To build in debug, run:

``` bash
mkdir build_debug && cd build_debug
conan install .. # -pr=debug
cmake ..  # -GNinja to compile with Ninja instead of make
make -j 8 # or ninja
```

To build in release, run:

``` bash
mkdir build_release && cd build_release
conan install .. -pr=release
cmake .. # -GNinja to compile with Ninja instead of make
make -j 8 # or ninja
```

Then, you can use the following debugging binaries:

* `./build_release/bin/vision`, to display informations from the vision (see `client/vision.cpp`)
* `./build_release/bin/referee`, to display informations from the referee (see `client/referee.cpp`)
* `./build_release/bin/sim`, to send commands to the simulator (see `client/sim.cpp`)

### CMake options

| CMake arguments         | Behavior                                        |
| ----------------------- | ----------------------------------------------- |
| **-DBUILD_TESTS=on**    | build all tests in `build_<type>/bin/tests/`    |

### Documentation

From the root of the repository:

``` bash
cd doc
mkdir build
cd build
cmake ..
make doc
```

Documentation is inside html/ directory.

## Testing

To run the tests once built (see [CMake options](#cmake-options)), just run them under the `build/bin/tests/` folder.

## Packages

Here are the packages:

* `ai`: the package that can all the code relative to the ai.
* `client`: the package to communicate with the SSL official software (vision, referee and simulator)
  * Note that this provides test binaries in `bin/tests` directory
* `viewer`: the viewer to interact with the strategies
* TODO!

## QtCreator

To use QtCreator, you have to open the CMakeFiles:

``` bash
qtcreator CMakeLists.txt
```

**Uncheck** your usual desktop build kit and **use** the differents `imported_kits`.
Finally click on "Configure Project" button.

You should alwais build the project from the command line to avoid configurations errors.

If you want to reset your Qt configuration, just remove the file CMakeLists.txt.user. Also, please never add this file to the git repository.

## Help

If you have strange errors with cmake or something else, you must :
1. Remove all files in your build_directory 
2. Redo the build instructions.