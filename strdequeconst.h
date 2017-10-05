#ifndef STRDEQUECONST_H
#define STRDEQUECONST_H


    // Nie dekoruj ("name mangling") nazw w stylu kompilatorów c++.
    #ifdef __cplusplus
    extern "C" {
    #endif

        static const unsigned long KOLEJKA_PUSTA = 0;

        unsigned long emptystrdeque();

    #ifdef __cplusplus
    }
    #endif

#endif /* STRDEQUECONST_H */
