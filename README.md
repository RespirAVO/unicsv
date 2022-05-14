[contributors-shield]: https://img.shields.io/github/contributors/RespirAVO/unicsv.svg?style=flat-square
[contributors-url]: https://github.com/RespirAVO/unicsv/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/RespirAVO/unicsv.svg?style=flat-square
[forks-url]: https://github.com/RespirAVO/unicsv/network/members
[stars-shield]: https://img.shields.io/github/stars/RespirAVO/unicsv.svg?style=flat-square
[stars-url]: https://github.com/RespirAVO/unicsv/stargazers
[issues-shield]: https://img.shields.io/github/issues/RespirAVO/unicsv.svg?style=flat-square
[issues-url]: https://github.com/RespirAVO/unicsv/issues
[license-shield]: https://img.shields.io/github/license/RespirAVO/unicsv.svg?style=flat-square
[license-url]: https://github.com/RespirAVO/unicsv/blob/master/LICENSE

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
![shield](https://img.shields.io/static/v1?label=version&message=0.0.0&color=blue) 


# unicsv
unicsv è un programma work-in-progress scritto in C che permette di unire due file CSV.

<div align="center">
  <img src=".github/unicsv.PNG", width="800px" style="left: auto; right: auto">
</div>


## Compilazione
### Requisiti
* Un C Compiler (GCC/Clang)
* Make (mingw32-make/make)
### Procedura
```sh
make setup
make
```

## Esecuzione
Usare il comando
```sh
./bin/unicsv -fp ./posizioni.csv -fm ./misure.csv -fo ./out.csv -um 5
```
