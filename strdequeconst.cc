/* W zadaniu jest wzmianka o tym, że ten moduł powinien używać asercji,
 * w przyjętym przeze mnie rozwiązaniu nie ma właściwie czego sprawdzać. */
#include "strdequeconst.h"
#include <iostream>


unsigned long emptystrdeque() {
    /* Nie wiem co autor miał na myśli wypisywanie błędów w tym pliku, np(?):
     *  #ifndef NDEBUG
        static std::ios_base::Init inicjalizacja_strumienia;
        std::cerr << __FUNCTION__ << std::endl;
        #endif
    */
    return KOLEJKA_PUSTA;
}
