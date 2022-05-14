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


#include <string.h>
#include <assertlib.h>
#include <stdlib.h>
#include <iolib.h>
#include <stdio.h>
#include "cliargs.h"


// Argomenti
#define ARG_IGNORA_FINO        "-if"
#define ARG_IGNORA_FINO_INTERO "--ignora-fino"
#define ARG_POSFILE            "-fp"
#define ARG_POSFILE_INTERO     "--file-posizioni"
#define ARG_MISFILE            "-fm"
#define ARG_MISFILE_INTERO     "--file-misure"
#define ARG_OUTFILE            "-fo"
#define ARG_OUTFILE_INTERO     "--file-output"
#define ARG_HELP               "-h"
#define ARG_HELP_INTERO        "--help"
#define ARG_MISNAME            "-um"
#define ARG_MISNAME_INTERO     "--unisci-misura"

#define TOO_FEW_ARGS -5
#define INVALID_OPT  -6


/******************************************************************************
PARAMETRI:
    - Stringa "__long" che rappresenta la versione lunga del parametro
    - Stringa "__short" che rappresenta la versione corta del parametro
    - Stringa "__arg" Il parametro effettivament passato

RETURN:
    - true se il parametro richiesto corriponde a quello fornito
    - Altrimenti false
******************************************************************************/
static bool __MatchOption__(const char *__long, const char *__short, const char *__arg)
{
    return strcmp(__arg, __long) == 0 || strcmp(__arg, __short) == 0;
}

/******************************************************************************
DESCRIZIONE:
Termina il programma con un messaggio di errore se non sono stati forniti
abbastanza argomenti per il parametro.

PARAMETRI:
    - Numero di argomenti richiesti dal parametro
    - Numero di argomenti forniti
    - Indice nell'array di argomenti
    - Opzione selezionata
******************************************************************************/
static void __AssertNumberOfArguments__(int __argn, int __argc__, int *__i, const char *__opt)
{
    int _si = *__i;
    *__i   += __argn;
    massert(*__i < __argc__, TOO_FEW_ARGS, "L'Opzione '%s' richiede %d argomenti aggiuntivi. %d forniti.", __opt, __argn, __argc__ - _si - 1);
}

/******************************************************************************
DESCRIZIONE:
Scrive i valori timestamp, latitudine, longitudine e misura sul file di output.

PARAMETRI:
    - Numero di argomenti
    - Array degli argomenti

RETURN:
    - Struct di informazione
******************************************************************************/
arginfo_t parse_args(int __argc_, char **__argv_)
{
    arginfo_t _info = { 1, 0, NULL, NULL, NULL };

    // Per ogni argomento fornito
    for (int i = 1; i < __argc_; i++)
    {
        // Ignora fino (-if --ignora-fino)
        if (__MatchOption__(ARG_IGNORA_FINO_INTERO, ARG_IGNORA_FINO, __argv_[i]))
        {
            __AssertNumberOfArguments__(1, __argc_, &i, __argv_[i]);
            _info.inogra_fino = atoi(__argv_[i]);
        }

        // Nome file posizioni (-fp --file-posizioni)
        else if (__MatchOption__(ARG_POSFILE_INTERO, ARG_POSFILE, __argv_[i]))
        {
            __AssertNumberOfArguments__(1, __argc_, &i, __argv_[i]);
            _info.fp = __argv_[i];
        }

        // Nome file misure (-fm --file-misure)
        else if (__MatchOption__(ARG_MISFILE_INTERO, ARG_MISFILE, __argv_[i]))
        {
            __AssertNumberOfArguments__(1, __argc_, &i, __argv_[i]);
            _info.fm = __argv_[i];
        }

        // Nome file di output (-fo --file-output)
        else if (__MatchOption__(ARG_OUTFILE_INTERO, ARG_OUTFILE, __argv_[i]))
        {
            __AssertNumberOfArguments__(1, __argc_, &i, __argv_[i]);
            _info.fo = __argv_[i];
        }

        // Nome misura (-um --unisci-misura)
        else if (__MatchOption__(ARG_MISNAME_INTERO, ARG_MISNAME, __argv_[i]))
        {
            __AssertNumberOfArguments__(1, __argc_, &i, __argv_[i]);
            _info.mis = atoi(__argv_[i]);
        }

        // Help (-h --help)
        else if (__MatchOption__(ARG_HELP_INTERO, ARG_HELP, __argv_[i]))
        {
            println("unicsv help menu");
            println("USAGE: unicsv [opzioni]\n");
            println("OPZIONI:");
            println("\t-fp | --file-posizioni\t| Permette di specificare un file di input per le posizioni");
            println("\t-fm | --file-misure\t| Permette di specificare un file di input per le misure");
            println("\t-fo | --file-output\t| Permette di specificare un file di output");
            println("\t-um | --unisci-misura\t| Permette di sceglere la misura da unire");
            println("\t-if | --ignora-fino\t| Permette di ignorare le prime n righe");

            exit(0);
        }

        // Se l'opzione selezionata non esiste
        else mexit(INVALID_OPT, "Opzione '%s' non riconosciuta.", __argv_[i]);
    }

    return _info;
}
