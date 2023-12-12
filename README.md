# Développement avancé R5.06: Maillages triangulaires

Ce projet propose une bibliothèque C++ pour manipuler des maillages triangulaires (très basique).

- [Développement avancé R5.06: Maillages triangulaires](#développement-avancé-r506-maillages-triangulaires)
  - [Comment compiler et installer](#comment-compiler-et-installer)
  - [Comment utiliser la bibliothèque](#comment-utiliser-la-bibliothèque)
    - [Charger un maillage](#charger-un-maillage)
    - [Sauvegarder un maillage](#sauvegarder-un-maillage)
    - [Calculer la surface d'un maillage](#calculer-la-surface-dun-maillage)
    - [Inverser les normales d'un maillage](#inverser-les-normales-dun-maillage)
    - [Homothétie du maillage](#homothétie-du-maillage)
    - [Bruiter le maillage](#bruiter-le-maillage)
    - [Vérifier les arêtes](#vérifier-les-arêtes)
  - [Subdivision de Loop](#subdivision-de-loop)
  - [Exemple concret](#exemple-concret)
  - [Licence](#licence)

## Comment compiler et installer

Pour des instructions détaillées sur la façon de construire et d'installer ce projet, veuillez vous référer au document [BUILDING](BUILDING.md).

## Comment utiliser la bibliothèque

### Charger un maillage

**Ne pas oublier d'inclure le header contenant la classe mesh**

```cpp
#include <tml/mesh.hpp>
```

Pour construire un maillage depuis un fichier, il suffit d'appeler le constructeur de la classe ``tml::mesh`` avec le nom du fichier en paramètre ou alors d'appeler la fonction ``read`` avec le nom du fichier en paramètre. Dans les deux cas, le fichier doit utiliser l'un des formats suivants:

- PLY (.ply) ``Format ASCII seulement``
- STL (.stl)
- COLLADA (.dae)

Le format sera automatiquement détecté en fonction de l'extension du fichier. Une extension différente résultera d'une ``tml::parse_error`` avec comme code ``tml::error_code::unsupported_format`` si appelé depuis la fonction ``read`` ou d'une exception ``std::runtime_error`` si appelé depuis le constructeur.

**Directement depuis le fichier:**

```cpp
try
{
    /*
     * À gérer dans un try/catch car c'est le seul moyen de reporter une
     * erreur dans le constructeur.
     */
    tml::mesh mesh{"input.ply"};
}
catch (std::exception const& exc)
{
    std::cerr << exc.what() << '\n';
}
```

Ou depuis la fonction ``read``:

```cpp
// Construction par défaut, maillage vide
tml::mesh mesh;
mesh.read("input.dae");
tml::parse_error const err = mesh.read("input.stl");

// Pas obligatoire mais fortemment recommandé, on vérifie si une erreur est survenue
if (err) [[unlikely]]
{
    std::cerr << err.message() << '\n';
}
Il est également possible de gérer les erreurs en les comparant à l'énumération tml::error_code:

switch (ec.code) // .code ici pour récupérer l'erreur sous forme d'un entier
{
    case tml::error_code::none:
        break;
    case tml::error_code::file_not_found:
        std::cerr << std::format("Le fichier {} n'existe pas.\n", filename);
        break;
    ...
}
```

ou

```cpp
if (ec == tml::error_code::file_not_found)
{
    std::cerr << std::format("Le fichier {} n'existe pas.\n", filename);
}
```

### Sauvegarder un maillage

Pour sauvegarder un maillage, il suffit d'appeler la fonction ``write`` avec le nom du fichier en paramètre et un booléen optionnel pour indiquer si on veut potentiellement écrire par-dessus un fichier existant. Si le fichier existe déjà et que le booléen est à false, une erreur ``tml::write_error`` avec le code ``tml::error_code::file_already_exists`` sera retournée. Si le fichier existe déjà et que le booléen est à true, le fichier sera écrasé. Par défaut, le booléen est à false.

```cpp
// On imagine un objet mesh déjà construit et rempli de quelque façon que ce soit...
mesh.write("output.ply", true); // Écrase le fichier s'il existe déjà
```

De la même manière que pour la fonction ``read``, on peut gérer l'erreur si besoin:

```cpp
// Pas de booléen, par défaut à false. Erreur si le fichier existe déjà
tml::write_error const err = mesh.write("output.stl"); // Écrase le fichier s'il existe déjà
```

```cpp
// Tout comme pour la fonction read, on peut gérer l'erreur si besoin
if (err) [[unlikely]]
{
    std::cerr << err.message() << '\n';
}
```

### Calculer la surface d'un maillage

Pour calculer la surface d'un maillage, on calcule la somme des aires de tous les triangles du maillage. Pour cela, on peut utiliser la fonction ``area``

```cpp
// On imagine un objet mesh déjà présent
float const area = mesh.area();
```

### Inverser les normales d'un maillage

Pour inverser les normales d'un maillage, on passe par toutes les faces et on inverse deux indices de sommets sur les trois. Pour cela, on peut utiliser la fonction ``invert``.

```cpp
// On imagine un objet mesh déjà présent
mesh.invert();
```

Centrer le maillage
Pour centrer le maillage, on passe par tous les sommets pour trouver les minimums et maximums suivant les coordonnées X, Y et Z. On translate ensuite tous les sommets pour que le minimum soit égal à l'opposé du maximum. Pour cela, on peut utiliser la fonction ``center``.

```cpp
// On imagine un objet mesh déjà présent
mesh.center();
```

### Homothétie du maillage

Pour effectuer une homothétie du maillage, on passe par tous les sommets pour les multiplier par un facteur. Pour cela, on peut utiliser la fonction ``scale``.

```cpp
// On imagine un objet mesh déjà présent
mesh.scale(2.0F);
```

### Bruiter le maillage

Pour bruiter le maillage, on passe par tous les sommets pour les déplacer aléatoirement. Pour cela, on peut utiliser la fonction ``noise``.

```cpp
// On imagine un objet mesh déjà présent
mesh.noise(0.1F);
```

### Vérifier les arêtes

Pour vérifier les arêtes d'un maillage, on passe par tous les sommets pour trouver les arêtes qui sont partagées par deux faces. Pour cela, on peut utiliser la fonction ``is_closed``.

```cpp
// On imagine un objet mesh déjà présent
if (mesh.is_closed())
{
    // Le maillage est fermé, encore du code...
}
```

## Subdivision de Loop

On peut subdiviser un maillage en utilisant la fonction ``subdivide``.

```cpp
// On imagine un objet mesh déjà présent
mesh.subdivide();
```

## Exemple concret

Petit exemple d'utilisation qui a pour objectifs:

- Charger un maillage depuis un fichier COLLADA (.dae) passé en paramètre
- Inverser les normales
- Centrer le maillage
- Effectuer une homothétie du maillage
- Bruiter le maillage
- Sauvegarder le maillage dans un fichier PLY (.ply) passé en paramètre

```cpp
#include <tml/mesh.hpp>
#include <iostream>

auto main(int argc, char* argv[]) -> int
{
    if (argc != 3) [[unlikely]]
    {
        std::cerr << "Usage: " << argv[0] << " <input> <output>\n";
        return -1;
    }

    try
    {
        tml::mesh mesh{argv[1]};

        mesh.invert();
        mesh.center();
        mesh.scale(2.0F);
        mesh.noise(0.1F);
        mesh.write(argv[2], true);
    }
    catch (std::exception const& exc)
    {
        std::cerr << exc.what() << '\n';
        return -1;
    }

    return 0;
}
```

Ou mieux, on peut même chaîner les appels:

```cpp
#include <tml/mesh.hpp>
#include <iostream>

auto main(int argc, char* argv[]) -> int
{
    if (argc != 3) [[unlikely]]
    {
        std::cerr << "Usage: " << argv[0] << " <input> <output>\n";
        return -1;
    }

    try
    {
        tml::mesh mesh{argv[1]};

        // Chaque fonction retourne le résultat de l'appel, on peut donc les chaîner
        auto const ec = mesh.invert()
                            .center()
                            .scale(2.0F)
                            .noise(0.1F)
                            .write(argv[2], true);

        if (ec) [[unlikely]]
        {
            std::cerr << ec.message() << '\n';
            return -1;
        }
    }
    catch (std::exception const& exc)
    {
        std::cerr << exc.what() << '\n';
        return -1;
    }

    return 0;
}
```

## Licence

Ce projet est sous licence MIT. Pour plus d'informations sur la licence MIT, voir le fichier [LICENSE](LICENSE) et visiter [ici](https://choosealicense.com/licenses/mit/).