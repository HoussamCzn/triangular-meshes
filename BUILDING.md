# Compilation

- [Compilation](#compilation)
  - [Dépendances](#dépendances)
    - [Debian/Ubuntu](#debianubuntu)
    - [Arch Linux](#arch-linux)
    - [Windows](#windows)
    - [macOS](#macos)
  - [Compilation avec CMake](#compilation-avec-cmake)
    - [Compilation avec MSVC (Windows)](#compilation-avec-msvc-windows)
    - [Compilation avec Apple Silicon (macOS)](#compilation-avec-apple-silicon-macos)
  - [Installation](#installation)
    - [Package CMake](#package-cmake)


## Dépendances

* Un compilateur C++20
* CMake 3.14 ou plus récent
* [fmt](https://github.com/fmtlib/fmt/), une bibliothèque de formatage de texte.
* [pugixml](https://github.com/zeux/pugixml), une bibliothèque de lecture de XML.

Pour les dépendances sous Linux, vous pouvez les installer avec votre gestionnaire de paquets. Sous Windows, vous pouvez utiliser [vcpkg](https://vcpkg.io) pour les installer. SUr macOS, vous pouvez utiliser [Homebrew](https://brew.sh).

### Debian/Ubuntu

On suppose un compilateur C++20 installé.

```sh
sudo apt install cmake libfmt-dev libpugixml-dev
```

### Arch Linux

```sh
sudo pacman -S cmake fmt pugixml
```

### Windows

```ps1
vcpkg install fmt pugixml
```

### macOS

```sh
brew install cmake fmt pugixml
```

## Compilation avec CMake

Ce projet ne nécessite aucun flag spécial pour garder les choses simples.

**Ici sont les étapes pour construire en mode Release avec un générateur, comme
Unix Makefiles:**

```sh
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build
```

**Ici sont les étapes pour construire en mode Release avec un générateur, comme
Visual Studio:**

```sh
cmake -S . -B build
cmake --build build --config Release
```

### Compilation avec MSVC (Windows)

Par défaut, MSVC n'est pas conforme aux standards et vous devez passer des
flags pour le faire fonctionner correctement. Voir le preset `flags-windows`
dans le fichier [CMakePresets.json](CMakePresets.json) pour les flags et avec
quelle variable les fournir à CMake pendant la configuration.

### Compilation avec Apple Silicon (macOS)

CMake supporte la compilation sur Apple Silicon depuis la version 3.20.1.
Assurez-vous d'avoir la [dernière version][1] installée.

## Installation

Le prérequis est que le projet ait déjà été construit avec les commandes
ci-dessus.

Les commandes ci-dessous nécessitent au moins CMake 3.15 pour fonctionner,
car c'est la version dans laquelle [Install a Project][2] a été ajouté.

Voici la commande pour installer les artefacts en mode release avec un
générateur à configuration unique, comme Unix Makefiles:

```sh
cmake --install build
```

Ici est la commande pour installer les artefacts en mode release avec un
générateur à configuration multiple, comme Visual Studio:

```sh
cmake --install build --config Release
```

Pour utiliser la bibliothèque installée en ligne de commandes, vous devez manuellement lier les
dépendances en plus de la bibliothèque elle-même.

```sh
g++ -std=c++20 -o main main.cpp -ltml -lpugixml -lfmt
```

### Package CMake

**Ce projet exporte un package CMake pour être utilisé avec la commande
[`FetchContent`][3]:**

* Nom du package: `tml`
* Nom de la cible: `tml::tml`

**Exemple d'utilisation:**

```cmake
include(FetchContent)

FetchContent_Declare(
  tml
  GIT_REPOSITORY https://github.com/HoussamCzn/triangular-meshes
  GIT_TAG main
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(tml)

target_link_libraries(${PROJECT_NAME} PRIVATE tml::tml)
```

**Pas de dépendances à lier soi-même ici.**

[1]: https://cmake.org/download/
[2]: https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project
[3]: https://cmake.org/cmake/help/latest/module/FetchContent.html
