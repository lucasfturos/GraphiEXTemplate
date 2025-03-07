# GraphiEXTemplate

This repository provides a template for quickly and efficiently setting up a development and testing environment for 2D and 3D models using OpenGL, SDL2, and ImGui.

The project includes examples and functions that enable the implementation of various types of 3D visualizations, such as volumetric rendering, model animation, standard models, and an interactive control panel, among other features.

## Features

-   Volumetric rendering
-   3D model animation
-   User interface with ImGui
-   Interactive control panel
-   Unit testing system

## Install

Below is the installation of the template, as well as testing of the examples.

### Requirements

-   GCC/G++
-   CMake
-   Make
-   SDL2
-   OpenGL
-   Assimp
-   STB
-   Doxygen
-   CTest

### Instruction

1. Clone the repository:

```
git clone https://github.com/lucasfturos/GraphiEXTemplate.git && cd GraphiEXTemplate
```

2. Update the git submodule

```
git submodule update --init --recursive
```

3. Create the build folder:

```
cmake -S . -B build
```

4. Enter the folder and compile the program:

```
cd build ; make -j4
```

5. Run the program:

```
./src/Demo
```

## License

This project is licensed under the GPLv3 License. See the LICENSE file for more information.

## Reference

1. [Learn OpenGL - Skeletal Animation](https://learnopengl.com/Guest-Articles/2020/Skeletal-Animation)
2. [@MathNuts on Github - Skeletal animation using C++ and OpenGL.](https://github.com/MathNuts/SkeletalAnimation)
3. [Mixamo - Source of the Animated Model](https://www.mixamo.com/)
4. [Volume Rendering with WebGL](https://www.willusher.io/webgl/2019/01/13/volume-rendering-with-webgl/)
5. [Divin Ookken Athappan - Getting Started with Volume Rendering using OpenGL](https://www.codeproject.com/Articles/352270/Getting-Started-with-Volume-Rendering-using-OpenGL)

## Other Languages

-   [Read this in Portuguese (Leia isto em português)](docs/README-pt-br.md)
