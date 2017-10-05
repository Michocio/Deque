/*
 * Autor: Michał Jaroń
 * indeks: mj348711
 * Projekt wykonałem bez udziału wylosowanego partnera.
*/


// Dyrektywa pozwalająca uzyskać nazwę funkcji, w której jest użyta
#define FUN __FUNCTION__


#include <iostream>
#include <map>
#include <limits.h>
#include <deque>
#include <cassert>
#include <string>
#include "strdeque.h"
#include "strdequeconst.h"


typedef std::map<unsigned long, std::deque<std::string>> mapa;

namespace
{
    /*
     * Struktura przechowująca wszystkie stworzone kolejki dwustronne.
     * Obudowana w funkcję pozwala uniknąć static initialization order fiasco.
     */
    mapa& kolejki()
    {
        static mapa kolejki;
        return kolejki;
    }
}


/* Pomocnicza procedura wypisująca na standardowe wyjście
 * diagnostyczne dany komunikat.
 */
void wypisz_err(std::string komunikat)
{

    // Iniciuj strumień, żeby uniknąć static initialization order fiasco.
    static std::ios_base::Init inicjalizacja_strumienia;

    #ifndef NDEBUG
        std::cerr << komunikat << std::endl;
    #endif
}

/* Pomocnicza procedura wypisująca na standardowe wyjście
 * diagnostyczne info o nieistniejącej kolejce o danym id.
 */
void kolejka_nie_istnieje(unsigned long id)
{
    std::string komunikat ("    deque id:" + std::to_string(id) + " nie istnieje");
    #ifndef NDEBUG
        std::cerr << komunikat << std::endl;
    #endif
}

/* Pomocnicza procedura wypisująca na standardowe wyjście
 * diagnostyczne info o próbie odwołania się do emptystrdeque.
 */
bool czy_emptystrdeque(unsigned long id)
{
    if(id == emptystrdeque())
    {
        std::string komunikat ("    próba odwołania do emptystrdeque");
        #ifndef NDEBUG
            std::cerr << komunikat << std::endl;
        #endif
        return true;
    }
    return false;
}


/* Funckja zlicza na zmiennej licznik, ile kolejek zostało utworzonych,
 * zwraca kolejny wolny numer jako id nowej kolejki dwustronnej.
*/
unsigned long generuj_id()
{
    assert((emptystrdeque() + 1) < UINT_MAX);
    static unsigned long licznik = emptystrdeque() + 1;
    return licznik++;
}

/*
 * Tworzy nową, pustą kolejkę dwustronną ciągów znaków i zwraca jej
 * identyfikator.
*/
unsigned long strdeque_new()
{
    wypisz_err(FUN);

    unsigned long id = generuj_id();
    /* Upewnij się czy id różne od identyfikatora
     * pustej kolejki dwustronnej do której nic nie można
     * wstawiać i z której nic nie można usuwać. */
    assert(id != emptystrdeque());
    // Upewnij się, że zaraz nie przekręcimy licznika
    assert(id < UINT_MAX);

    std::deque<std::string> nowa;
    kolejki().insert(std::pair<unsigned long, std::deque<std::string>>(id,nowa));

    assert(nowa.size() == 0);

    wypisz_err("strdeque_new: deque id: " + std::to_string(id)+
            " została stworzona!");

    return id;
}

/*
 * Jeżeli istnieje kolejka dwustronna o identyfikatorze id, usuwa ją,
 * a w przeciwnym przypadku nic nie robi.
*/
void strdeque_delete(unsigned long id)
{
    wypisz_err(FUN);

    if(!czy_emptystrdeque(id))
    {
        mapa::iterator it = kolejki().find(id);
        if(it != kolejki().end())
        {
            assert(kolejki().count(id) == 1);
            kolejki().erase(it);
            assert(kolejki().count(id) == 0);// Upewnij się czy usunięto kolejke

            wypisz_err("strdeque_delete: deque id:" + std::to_string(id) +
                            " usunięta");
        }
        else
            kolejka_nie_istnieje(id);
    }
}

/*
 * Jeżeli istnieje kolejka dwustronna o identyfikatorze id,
 * zwraca liczbę jej elementów, a w przeciwnym przypadku zwraca 0.
*/
size_t strdeque_size(unsigned long id)
{
    wypisz_err(FUN);

    if(!czy_emptystrdeque(id))
    {
        mapa::iterator it = kolejki().find(id);
        if(it != kolejki().end())
        {
            assert(kolejki().count(id) == 1);

            wypisz_err("strdeque_size: deque id:" + std::to_string(id) +" rozmiar: " +
                                std::to_string(it->second.size()));
            return it->second.size();
        }
        else
            kolejka_nie_istnieje(id);
    }
    return 0;
}

/*
 * Jeżeli istnieje kolejka dwustronna o identyfikatorze id oraz value != NULL,
 * wstawia element value przed pozycją pos lub na koniec kolejki, jeżeli pos
 * wykracza poza kolejkę.
 * Gwarancje odnośnie kosztów wstawienia na początku, na końcu i w środku
 * mają być takie same jak w przypadku kontenera deque (plus koszt
 * odnalezienia kolejki o danym identyfikatorze).
 * Jeżeli kolejka dwustronna nie istnieje lub value == NULL, to nic nie robi.
 * Pozycje w kolejce numerowane są od zera.
*/
void strdeque_insert_at(unsigned long id, size_t pos, const char *value)
{
    wypisz_err(FUN);

    if(!czy_emptystrdeque(id))
    {
        mapa::iterator it = kolejki().find(id);
        if(it != kolejki().end() && value!= NULL)
        {
            const int rozmiar_poczatkowy = it->second.size();
            // Unikamy warninga:
            if(rozmiar_poczatkowy) {}
            assert(kolejki().count(id) == 1);

            std::string val(value);

            if(pos > it->second.size())
                it->second.push_back(val);
            else
                it->second.insert(it->second.begin() + pos, val);
            wypisz_err("strdeque_insert_at: dodano nowy element");

            // Upewnij się, że dodano element
            assert((unsigned long)it->second.size() ==
                            (unsigned long)rozmiar_poczatkowy+1);
        }
        else if(value == NULL)
            wypisz_err("strdeque_insert_at: przekazano val==NULL");
        else
            kolejka_nie_istnieje(id);
    }
}

/*
 * Jeżeli istnieje kolejka dwustronna o identyfikatorze id i pos nie wykracza
 * poza nią, usuwa element na pozycji pos, a w przeciwnym przypadku nic nie
 * robi.
*/
void strdeque_remove_at(unsigned long id, size_t pos)
{
    wypisz_err(FUN);

    if(!czy_emptystrdeque(id))
    {
        mapa::iterator it = kolejki().find(id);
        if(it != kolejki().end() && pos < it->second.size())
        {
            assert(kolejki().count(id) == 1);

            const int rozmiar_poczatkowy = it->second.size();
            // Unikamy warninga:
            if(rozmiar_poczatkowy) {}

            it->second.erase(it->second.begin() + pos);

            assert((unsigned long)it->second.size() ==
                            (unsigned long)rozmiar_poczatkowy - 1);
        }
        else if(pos >= it->second.size())
            wypisz_err("strdeque_insert_at: pos wykracza poza kolejke");
        else
            kolejka_nie_istnieje(id);
    }
}

/*
 * Jeżeli istnieje kolejka dwustronna o identyfikatorze id i pos nie wykracza
 * poza kolejkę, zwraca wskaźnik do elementu na pozycji pos, a w przeciwnym
 * przypadku zwraca NULL.
*/
const char* strdeque_get_at(unsigned long id, size_t pos)
{
    wypisz_err(FUN);

    if(!czy_emptystrdeque(id))
    {
        mapa::iterator it = kolejki().find(id);
        if(it != kolejki().end() && pos < it->second.size())
            return it->second[pos].c_str();
        else if(pos >= it->second.size())
            wypisz_err("strdeque_get_at: pos wykracza poza kolejke");
        else
            kolejka_nie_istnieje(id);
    }
    return NULL;
}

/*
 * Jeżeli istnieje kolejka dwustronna o identyfikatorze id, usuwa wszystkie
 * jej elementy, a w przeciwnym przypadki nic nie robi.
*/
void strdeque_clear(unsigned long id)
{
    wypisz_err(FUN);

    if(!czy_emptystrdeque(id))
    {
        mapa::iterator it = kolejki().find(id);
        if(it != kolejki().end())
        {
            it->second.clear();
            assert(it->second.empty());
        }
        else
            kolejka_nie_istnieje(id);
    }
}

/*
 * Porównuje leksykograficznie kolejki dwustronne o identyfikatorach
 * id1 i id2, zwracając
     -1, gdy kolejka(id1) < kolejka(id2),
      0, gdy kolejka(id1) = kolejka(id2),
      1, gdy kolejka(id1) > kolejka(id2).
 * Jeżeli kolejka dwustronna o którymś z identyfikatorów nie istnieje, to jest
 * traktowana jako leksykograficznie równa liście pustej.
*/
int strdeque_comp(unsigned long id1, unsigned long id2)
{
    wypisz_err(FUN);

    std::deque<std::string> comp1;
    std::deque<std::string> comp2;

    mapa::iterator it1 = kolejki().find(id1);
    mapa::iterator it2 = kolejki().find(id2);

    if(it1 != kolejki().end())
        comp1 = it1->second;

    if(it2 != kolejki().end())
        comp2 = it2->second;

    if(comp1 < comp2)
    {
        wypisz_err("strdeque_comp: id1 < id2");
        return -1;
    }
    if(comp1 == comp2)
    {
        wypisz_err("strdeque_comp: id1 = id2");
        return 0;
    }
    wypisz_err("strdeque_comp: id1 > id2");
    return 1;
}
