# ICP Projekt: 2024/2025
Implementace projektu do předmetu ICP, 2024/2025 na FIT VUT.

# Autoři
* Antonín Červinka (xcervia00)
* Tereza Kadlecová (xkadlet00)
* Jana Elisabet Zejdová (xzejdoj00)

## Obsah

- [ICP Projekt: 2024/2025](#icp-projekt-20242025)
- [Autoři](#autoři)
- [Obsah](#obsah)
- [Shrnutí](#shrnutí)
- [Struktura Programu](#struktura-programu)
- [Sestavení](#sestavení)
- [Spuštění](#spuštení)
- [Implementovaná funkcionalita](#implementovaná-funkcionalita)

## Shrnutí
Projekt si dává za cíl vytvořit vizuální editor pro tvorbu a úpravu konečných automatů Mooreova typu, a poté jejich
interpretaci pomocí téhož uživatelského rozhraní.

Pro implementaci se spoléhá na framework Qt (minimálně verze 5.5.1).

Editor umožňuje specifikovat stavy automatu, přechody mezi nimi a seznam akcí, které se mají při vstupu do stavu vykonat.
Přechody se mohou provést pouze po zaznamenání vstupní události (může být i prázdná), přičemž se v takovém případě nejprve vyhodnotí podmínka přechodu. 
Pokud je pravdivá, přechod může mít nadále nastaveno zpoždění, po jehož skončení se přejde do nového stavu – může tak v jednu chvíli existovat více čekajících přechodů.

V editoru je dále možné specifikovat vstupní, výstupní a interní proměnné.
Interní mají konkrétní datový typ a jsou využívány v rámci akcí stavů či podmínkách přechodů jakožto pomocné proměnné.
Vstupní a výstupní jsou pouze řetězce, které slouží pro úschovu hodnot spjatých se vstupními a výstupními událostmi.

Akce stavů či podmínky přechodů jsou zapsány a interpretovány v inskripčním jazyce JavaScript (vestavěný ve využitém frameworku Qt).

## Struktura programu
Program je rozdělen do několika složek podle významu. Ty jsou:
* `Makefile` - Makefile pro překlad programu; bliže popsáno v [sekci o sestavení](#sestavení)
* `examples` - příklady specifikovaných automatů
* `doc` - složka pro generovanou programovou dokumentaci
* `doc/konceptualni_navrh.pdf` - popis konceptuálního návrhu
* `src` - zdrojové soubory
    - `model` - vnitřní reprezentace automatu; odděleno od zobrazování
    - `interpreter` - pomocné struktury pro interpretaci automatu z vnitřní reprezentace
    - `network` - modul pro komunikaci po síti
    - `view` - implementace zobrazování automatu a uživatelského vstupu; řízené stavem vnitřní reprezentace
    - `mvc_interface.h` - sdílená knihovna pro komunikaci mezi model-view-controller entitami
    - `img` - ikony využívané view
    - `exceptions` - výjimky specifické pro interpret/model

## Sestavení
Pro sestavení stačí pouze spustit Makefile příkazem `make` v kořenovém adresáři projektu.
Předpokládá se nainstalovaná knihovna Qt, včetně Qml rozšíření, alespoň ve verzi 5.5.1 (ideálně ale vyšší).
Při kompilaci se také využívá qmake v dané verzi.
Projekt byl primárně vyvíjen na platformě Linux.

Makefile podporuje následující příkazy:
* `make` - Zkompiluje projekt do binárního souboru icp_fsm_interpreter do složky `build`
* `make run` - Program zkompiluje a poté spustí
* `make doxygen` - Vygeneruje projektovou dokumentaci pomocí doxygen do složky `doc`
* `make clean` - Smaže veškeré produkty kompilace (včetně obsahu složky `doc`) 
* `make pack` - Sbalí a zkomprimuje zdrojové soubory projektu .zip souboru
* `make debug` - Zkompiluje program v režimu pro ladění 
* `make run_debug` - Zkompiluje a spustí program v režimu pro ladění

## Spuštění
Pro spuštění stačí pouze spustit příkaz:
```
./build/icp_fsm_interpreter
```
v kořenové složce programu, či případně využít `make run`.

## Implementovaná funkcionalita
* Vizuální editor konečných automatů 
* Specifikovaný automat načíst z/uložit do souboru (ve snadno čitelném formátu)
* Uživatele informovat o chybách při tvorbě/interpretaci automatu
* Načtený automat interpretovat
* Asynchronně přijímat vstup z klávesnice či ze sítě (pomocí UDP socketů)
* Logování průběhu interpretace