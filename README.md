# moj-shell

Mini Unix shell napisan u C++-u, kao projekat za učenje sistemskog programiranja.

## Šta radi

- Čita komande u petlji i razbija ih na riječi
- Pokreće bilo koji program (`ls`, `pwd`, `g++`...) pomoću `fork`, `execvp` i `waitpid`
- Ugrađena komanda `cd` (preko `chdir`)
- Ispisuje razlog greške kad komanda ne postoji (`perror`)
- Izlazak sa `exit` ili `Ctrl+D`

## Kako radi

Shell je petlja: **čitaj liniju, razbij na riječi, pokreni, čekaj, ponovi.**

1. `fork()` pravi kopiju shella (dijete)
2. dijete pomoću `execvp()` postaje traženi program
3. roditelj (shell) čeka pomoću `waitpid()` da dijete završi

`cd` je ugrađena komanda jer mora promijeniti folder samog shella, a ne djeteta.

## Pokretanje

Potreban je Linux (ili WSL na Windowsu) i g++.

```
g++ -std=c++17 -Wall -o shell main.cpp
./shell
```

## Primjer

```
moj-shell> pwd
/home/nirvana/moj-shell
moj-shell> cd ..
moj-shell> ls -l
moj-shell> nepostoji
moj-shell: No such file or directory
moj-shell> exit
```

## Plan

- [x] Petlja za unos i parsiranje
- [x] Pokretanje programa (`fork`/`exec`/`wait`)
- [x] Ugrađena komanda `cd`
- [x] Preusmjeravanje (`>`)
- [x] Pipe (`|`)
- [x] Pozadinski procesi (`&`)
