# Build avec CMake

## Dépendances

* Un compilateur C++20
* CMake 3.14 ou plus récent

## Build avec CMake

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

### Build avec MSVC (Windows)

Par défaut, MSVC n'est pas conforme aux standards et vous devez passer des
flags pour le faire fonctionner correctement. Voir le preset `flags-windows`
dans le fichier [CMakePresets.json](CMakePresets.json) pour les flags et avec
quelle variable les fournir à CMake pendant la configuration.

### Build avec Apple Silicon (macOS)

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

[1]: https://cmake.org/download/
[2]: https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project
[3]: https://cmake.org/cmake/help/latest/module/FetchContent.html
