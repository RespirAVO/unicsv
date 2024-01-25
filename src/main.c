/***************************************************************************
RespirAVO/unicsv
Copyright 2022 3C A. Avogadro & RespirAVO Contributors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***************************************************************************/

/******************************************************************************
@data: Marzo 2022
@auhor: Classe 3CI as 2021/22

@scopo:
legge i file misure.csv e posizioni.csv
e produce in output il file out.csv mettendo insieme i dati dei due file,
controllando le vicinanze del timestamp e prendendo le misure
che sono distanti temporalmente  meno di 1 minuto  tra i due file

1) formato del file misure.csv
   - timestamp
   - data
   - NO2 (ppb)
   - VOC (ppb)
   - pm 10 (ug/m3)
   - pm 2.5 (ug/m3)
   + altro che non interessa

2) formato del file posizioni.csv
   - timestamp
   - data
   - latitude
   - longitude

3) formato del file out.csv
   - timestamp
   - latitude
   - longitude
   - NO2 (ppb)
   - VOC (ppb)
   - pm 10 (ug/m3)
   - pm 2.5 (ug/m3)

@compilazione:
WINDOWS: mingw32-make
POSIX: make

@esecuzione:
WINDOWS: .\bin\unicsv.exe -fp .\posizioni.csv -fm .\misure.csv -fo .\out.csv -um 5
POSIX: ./bin/unicsv -fp ./posizioni.csv -fm ./misure.csv -fo ./out.csv -um 5
******************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <filelib.h>
#include <csvlib.h>
#include <assertlib.h>
#include <siglib.h>
#include <iolib.h>
#include "wrappers.h"
#include "cliargs.h"

// Crea costante con comando di clear
#ifdef _WIN32
    #define CLEARSTR "cls"
#else
    #define CLEARSTR "clear"
#endif

// Macro per screen clear
#define CLEAR() (void)system(CLEARSTR)

#define MAX_SPREAD 60


void disegna_logo()
{
    CLEAR();

    println("              _                ");
    println("             (_)               ");
    println("  _   _ _ __  _  ___ _____   __");
    println(" | | | | '_ \\| |/ __/ __\\ \\ / /");
    println(" | |_| | | | | | (__\\__ \\\\ V / ");
    println("  \\__,_|_| |_|_|\\___|___/ \\_/  \n\n");
}



void ignore_lines(FILE *__file, int __nlines)
{
    for (int i = 0; i < __nlines; i++)
        csvIgnoreLine(__file);
}

bool numero_a_misura(int __scelta, char __nome_mis[])
{
    // Valutazione inpu
    switch (__scelta)
    {
        case NO2  : strcpy(__nome_mis, "NO2 (ppb)");                                     return true;
        case VOC  : strcpy(__nome_mis, "VOC (ppb)");                                     return true;
        case PM10 : strcpy(__nome_mis, "PM10 (ug/m3)");                                  return true;
        case PM25 : strcpy(__nome_mis, "PM25 (ug/m3)");                                  return true;
        case ALL  : strcpy(__nome_mis, "NO2 (ppb),VOC (ppb),PM10 (ug/m3),PM25 (ug/m3)"); return true;
    }

    return false;
}

/**************************************************
DESCRIZIONE:
Visualizza un menu' che permette di sceglere la
misura che si vuole unire.

PARAMETRI:
    - Puntatore ad `int` "__scelta"
    - Stringa "__nome_mis"
**************************************************/
void menu(int *__scelta, char __nome_mis[])
{
    while (true)
    {
        disegna_logo();

        // Stampa opzioni
        println("Selezionare misura");
        println("1. NO2");
        println("2. VOC");
        println("3. PM10");
        println("4. PM2.5");
        println("5. Tutte");
        
        // Input utente
        printf("\nscelta: ");
        (void) scanf("%d", __scelta);  // Cast a void utilizzato per annullare valore ritornato
        
        if (numero_a_misura(*__scelta, __nome_mis)) return;
    }
}

int main(int __argc_, char **__argv_)
{
    // Inizializza gestore segnali
    sigSetup();
    
    pos_t _pos = { 0 };
    mis_t _mis = { 0 };

    // Nomi file
    char _fp_name[MAX_STR_LEN],   // Nome file posizioni
         _fm_name[MAX_STR_LEN],   // Nome file misure
         _fo_name[MAX_STR_LEN];   // Nome file di output

    // Scelta
    char  _mis_name[MAX_STR_LEN];

    // Controllo argomenti commandline - Codice da pulire
    disegna_logo();
    arginfo_t _info = parse_args(__argc_, __argv_);

    if (_info.fp == NULL) { printf("Nome file posizioni: "); scanf("%s", _fp_name); _info.fp = _fp_name; }
    if (_info.fm == NULL) { printf("Nome file misure: ");    scanf("%s", _fm_name); _info.fm = _fm_name; }
    if (_info.fo == NULL) { printf("Nome file di output: "); scanf("%s", _fo_name); _info.fo = _fo_name; }

    // Apre file stream verso i file richiesti
    disegna_logo();
    println("Apertura file...");
    FILE *_fp = fileOpenRead(_info.fp);  // pointer al file delle posizioni
    FILE *_fm = fileOpenRead(_info.fm);  // pointer al file delle misure
    FILE *_fo = fileOpenWrite(_info.fo); // pointer al file di output

    // Chiede misura da mettere nel file di output
    if (_info.mis == 0) { menu(&_info.mis, _mis_name); disegna_logo(); } else { massert(numero_a_misura(_info.mis, _mis_name), -7, "ID Misura %d non valido.", _info.mis); }

    // Ignora prima riga dei file di input
    println("Ignorando %d righe dei CSV...", _info.inogra_fino);
    ignore_lines(_fm, _info.inogra_fino);
    ignore_lines(_fp, _info.inogra_fino);

    // Stampa intestazione tabella CSV
    println("Inserendo intestazione in file di output...");
    csvPutHeader(_fo, 4, "timestamp", "latitudine", "longitudine", _mis_name);

    printf("\nProgresso:\n[");

    // Determina se è possibile continaure o meno
    volatile bool _continuare = true;
             int  _count      = 0;

    // Fino a quando non si raggiunge la fine di uno dei due file di input
    while (_continuare)
    {
        // Se p_time è minore di m_time, si legge dal file posizioni, altrimenti dal file misure
        _continuare = (_pos.timestamp < _mis.timestamp) ? FILE_OK == leggi_pos(_fp, &_pos) :
                                                          FILE_OK == leggi_mis(_fm, &_mis);
        
        // Differenza tra i timestamp di misure e posizioni
        int _diff = abs((int)_pos.timestamp - (int)_mis.timestamp);

        // Se la differenza tra i due timestamp è nella forbice accettabile
        if (_diff < MAX_SPREAD)
            massert(
                scrivi_out(_fo, _pos, _mis, _info.mis),           // Condizione
                -4, "Scrittura su '%s' fallita.", _fo_name        // Se la condizione non è verificata
            );

        if (_count++ % 100 == 0)
            printf("#");
    }

    // Chiusura delle stream su tutti i file
    fclose(_fp);
    fclose(_fm);
    fclose(_fo);

    // Stampa due righe vuote alla fine
    println("\t100%%\n");

    #ifdef _WIN32
        system("pause");
    #endif
}
