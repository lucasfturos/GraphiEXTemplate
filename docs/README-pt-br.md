# Graphics Extreme Template PT-BR

Este repositório oferece um template para configuração rápida e eficiente de um ambiente de desenvolvimento e teste para modelos 2D e 3D, utilizando OpenGL, SDL2 e ImGui.

O projeto inclui exemplos e funções que permitem a implementação de diversos tipos de visualizações 3D, como renderização volumétrica, animação de modelos, modelos normais, um painel de controle interativo, entre outros recursos.

## Características

-   Renderização volumétrica
-   Animação de modelos 3D
-   Interface de usuário com ImGui
-   Painel de controle interativo
-   Sistema de testes unitário

## Instalação

Abaixo está a instalação do modelo, bem como o teste dos exemplos.

### Requisitos

-   GCC/G++
-   CMake
-   Make
-   SDL2
-   OpenGL
-   Assimp
-   STB
-   Doxygen
-   CTest

### Instruções para Instalar

1. Clone o repositório:

```
git clone https://github.com/lucasfturos/GraphiEXTemplate.git && cd GraphiEXTemplate
```

2. Atualize o git submodule

```
git submodule update --init --recursive
```

3. Crie o arquivo de build:

```
cmake -S . -B build
```

4. Entre no arquivo build e compile o programa:

```
cd build ; make -j4
```

5. Execute o programa:

```
./src/Demo
```

## Licença

Este projeto está sob a licença GPLv3. Consulte o arquivo LICENSE para mais informações.

## Referências

1. [Learn OpenGL - Skeletal Animation](https://learnopengl.com/Guest-Articles/2020/Skeletal-Animation)
2. [@MathNuts on Github - Skeletal animation using C++ and OpenGL.](https://github.com/MathNuts/SkeletalAnimation)
3. [Mixamo - Source of the Animated Model](https://www.mixamo.com/)
4. [Volume Rendering with WebGL](https://www.willusher.io/webgl/2019/01/13/volume-rendering-with-webgl/)
5. [Divin Ookken Athappan - Getting Started with Volume Rendering using OpenGL](https://www.codeproject.com/Articles/352270/Getting-Started-with-Volume-Rendering-using-OpenGL)
