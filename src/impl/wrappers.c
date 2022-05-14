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


#include "wrappers.h"
#include <csvlib.h>
#include <stdio.h>


/***************************************************************************
DESCRIZIONE:
Legge i valori timestamp, latitudine e longitudine dal file delle posizioni.

PARAMETRI:
    - Puntatore a FILE CSV
    - Puntatore a `pos_t` "__pos"

RETURN:
    - FILE_OK (0) se l'operazione è stata conclusa con successo
    - Un valore positivo se è stato raggiunto EOF
***************************************************************************/
int leggi_pos(FILE *__file, pos_t *__pos)
{
    return csvGetEntries(
        __file,                        // Leggi dal file specificato
        &(__pos->timestamp), "%lu",    // Leggi timestamp
        IGNORE, NULL,                  // Ignora colonna
        &(__pos->lat), "%f",           // Leggi latitudine
        &(__pos->lon), "%f"            // Leggi longitudine
    );
}

/***************************************************************************
DESCRIZIONE:
Legge i valori timestamp, NO2, VOC, PM10 e PM2.5 dal file delle misure.

PARAMETRI:
    - puntatore a FILE CSV
    - Puntatore a `mis_t` "__mis"

RETURN:
    - FILE_OK (0) se l'operazione è stata conclusa con successo
    - Un valore positivo se è stato raggiunto EOF
***************************************************************************/
int leggi_mis(FILE *__file, mis_t *__mis)
{
    return csvGetEntries(
        __file,                        // Leggi dal file delle misure
        &(__mis->timestamp), "%lu",    // Leggi timestamp
        IGNORE, NULL,                  // Ignora colonna
        &(__mis->no2), "%f",           // Leggi valore di NO2
        &(__mis->voc), "%f",           // Leggi valore di VOC
        &(__mis->pm10), "%f",          // Leggi valore di PM10
        &(__mis->pm25), "%f",          // Leggi valore di PM2.5
        IGNORE, NULL,                  // Ignora colonna
        IGNORE, NULL,                  // Ignora colonna
        IGNORE, NULL,                  // Ignora colonna
        IGNORE, NULL                   // Ignora colonna
    );
}

/******************************************************************************
DESCRIZIONE:
Scrive i valori timestamp, latitudine, longitudine e misura sul file di output.

PARAMETRI:
    - Puntatore a FILE CSV
    - `pos_t` "__pos"
    - `mis_t` "__mis"
    - int "__opt"

RETURN:
    - Un valore positivo se l'operazione è stata conclusa con successo
    - Altrimenti 0
******************************************************************************/
int scrivi_out(FILE *__file, pos_t __pos, mis_t __mis, int __opt)
{
    bool _success = false;
    fprintf(__file, "%lu,%f,%f", __pos.timestamp, __pos.lat, __pos.lon);

    switch (__opt)
    {
        case NO2  : _success = fprintf(__file, ",%f", __mis.no2); break;
        case VOC  : _success = fprintf(__file, ",%f", __mis.voc); break;
        case PM10 : _success = fprintf(__file, ",%f", __mis.pm10); break;
        case PM25 : _success = fprintf(__file, ",%f", __mis.pm25); break;
        case ALL  : _success = fprintf(__file, ",%f,%f,%f,%f", __mis.no2, __mis.voc, __mis.pm10, __mis.pm25); break;
    }

    fprintf(__file, "\n");
    return _success;
}
