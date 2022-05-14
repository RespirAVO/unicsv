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


#ifndef CLIARGS_H
#define CLIARGS_H

    typedef struct arg_info
    {
        unsigned long int   inogra_fino;
                      int   mis;
                const char *fp;
                const char *fm;
                const char *fo;
    } arginfo_t;


    arginfo_t   parse_args    (int __argc_, char **__argv_);

#endif // CLIARGS_H
