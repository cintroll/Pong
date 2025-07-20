# 🎮 Pong em SDL

**Projeto acadêmico desenvolvido para a disciplina Introdução a Ciencias da Computação do curso de Engenharia da Computação**

Este projeto representa meu primeiro contato com desenvolvimento de jogos utilizando a biblioteca SDL (Simple DirectMedia Layer) em linguagem C. Foi desenvolvido como parte do currículo de Engenharia da Computação, explorando conceitos fundamentais de programação gráfica, manipulação de eventos e estruturas de dados em tempo real.

## 📋 Sobre o Projeto

### Tecnologias Utilizadas
- **Linguagem**: C
- **Biblioteca Gráfica**: SDL2
- **Extensões**: SDL2_image, SDL2_ttf
- **Build System**: CMake
- **Compilador**: GCC/Clang

## 🛠️ Compilação e Execução

### Pré-requisitos

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

#### Fedora/CentOS
```bash
sudo dnf install gcc cmake SDL2-devel SDL2_image-devel SDL2_ttf-devel
```

#### Arch Linux
```bash
sudo pacman -S gcc cmake sdl2 sdl2_image sdl2_ttf
```

### Compilação

```bash
# Clone o repositório
git clone https://github.com/cintroll/Pong
cd Pong 

# Crie o diretório de build
mkdir build && cd build

# Configure com CMake
cmake ..

# Compile o projeto
make

# Execute o jogo
cd bin
./Pong
```

### Compilação Manual (sem CMake)
```bash
gcc -o Pong src/*.cpp `pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf`
```

## 🎯 Estrutura do Projeto

```
projeto/
├── src/                    # Código fonte
│   ├── main.cpp            # Função principal e loop do jogo
├── include/               # Headers
├── assets/                # Recursos do jogo
├── CMakeLists.txt        # Configuração de build
└── README.md            # Este arquivo
```

## 🎮 Como Jogar

### Controles
- **Setas para cima e para baixo**: Movimento do personagem 1
- **Teclas W e S**: Movimento do personagem 2
- **Tecla P**: Pausar o jogo
- **Tecla ESC**: Volta para o menu

