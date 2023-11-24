# Szkolenie - Test-Driven Development w C++ #

## Ankieta

* https://forms.gle/q4V6e6GxTrmxtZuZ8

## Video

* https://infotrainingszkolenia-my.sharepoint.com/:v:/g/personal/krystian_piekos_infotraining_pl/ERo1o-MyTMVBk-k_mS2A8y4B_-Etf2lmfWrpKINeg2ku-Q?e=nRwKkI&nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJTdHJlYW1XZWJBcHAiLCJyZWZlcnJhbFZpZXciOiJTaGFyZURpYWxvZy1MaW5rIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXcifX0%3D

* https://infotrainingszkolenia-my.sharepoint.com/:v:/g/personal/krystian_piekos_infotraining_pl/EaCgXUnX-uVLmWYxzGOwNAcBfT64uwyrNqwJV0mVRV8tcQ?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0RpcmVjdCJ9fQ&e=C3dwth

* https://infotrainingszkolenia-my.sharepoint.com/:v:/g/personal/krystian_piekos_infotraining_pl/EdBpbbQHRRJImT8V-sYLc5IB9pVBPoyFvKiVcYvDyItBdw?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0RpcmVjdCJ9fQ&e=PYePL8

* https://infotrainingszkolenia-my.sharepoint.com/:v:/g/personal/krystian_piekos_infotraining_pl/EXz4J5D43G9AqGj62EPIedYBz-ZRPgYWpUb6zBQ2Lpw1iQ?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0RpcmVjdCJ9fQ&e=FbUI0C

## Dokumentacja

* https://infotraining.github.io/docs-cpp-tdd

### Linki

* https://martinfowler.com/articles/mocksArentStubs.html
* https://github.com/rollbear/trompeloeil

## Konfiguracja środowiska

Proszę wybrać jedną z poniższych opcji:

### Lokalna

Przed szkoleniem należy zainstalować:

#### Kompilator C++ wspierający C++20 - do wyboru:
  * Visual Studio 2022
    * przy instalacji należy zaznaczyć opcje:
      * Desktop development with C++
      * C++ CMake tools for Windows
      * vcpkg package manager

  * GCC - Linux lub WSL
    * gcc (wersja >= 12)
    * [CMake > 3.25](https://cmake.org/)
      * proszę sprawdzić wersję w linii poleceń        
  
        ```
        cmake --version
        ```
    * vcpkg
      * instalacja - https://vcpkg.io/en/getting-started
      * dodać zmienną środowiskową VCPKG_ROOT
        * w pliku `.bashrc` należy dodać wpis

          ```
          export VCPKG_ROOT=[path to vcpkg]
          ```
    * IDE: Visual Studio Code
      * [Visual Studio Code](https://code.visualstudio.com/)
      * zainstalować wtyczki
        * C/C++ Extension Pack
        * Live Share

### Docker + Visual Studio Code

Jeśli uczestnicy szkolenia korzystają w pracy z Docker'a, to należy zainstalować:

#### Docker Desktop (Windows)

* https://www.docker.com/products/docker-desktop/

#### Visual Studio Code

* [Visual Studio Code](https://code.visualstudio.com/)
* Zainstalować wtyczki
  * Live Share
  * Dev Containers ([wymagania](https://code.visualstudio.com/docs/devcontainers/containers#_system-requirements))
    * po instalacji wtyczki - należy otworzyć w VS Code folder zawierający sklonowane repozytorium i
      z palety poleceń (Ctrl+Shift+P) wybrać opcję **Dev Containers: Rebuild and Reopen in Container**
