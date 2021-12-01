/*
Projekt: SHO vo vyrobe
Autori: Filip Brna (xbrnaf00), Matej Hornik (xhorni20)
Projekt simuluje výrobu rovadzaca vysokeho napatia UniGear spoločnosti ABB.
Dalsie informacie a popis najdete v dokumentacii.
jednotky času v systeme su minuty
*/

#include "simlib.h"
#include <stdio.h>

const int POC_STROJOV_MONTAZ_PANEL = 3;
const int POC_STROJOV_MONTAZ_PODSTAVA = 3;
const int POC_ZERIAV = 1;
const int POC_STROJOV_MONTAZ_KABLE = 3;
const int POC_STROJOV_MONTAZ_ZADNY_KRYT = 2;
const int POC_STROJOV_MONTAZ_KLAPKY = 2;
const int POC_STROJOV_MONTAZ_PREDNE_DVERE = 4;

///////////////////////////// NEVIEM CI TAKTO /////////////////////////////

//stroje v zavode
Facility Montaz_panel[POC_STROJOV_MONTAZ_PANEL];
Facility Montaz_podstava[POC_STROJOV_MONTAZ_PODSTAVA];
Facility Zeriav[POC_ZERIAV];
Facility Montaz_kable[POC_STROJOV_MONTAZ_KABLE];
Facility Montaz_zadny_kryt[POC_STROJOV_MONTAZ_ZADNY_KRYT];
Facility Montaz_klapky[POC_STROJOV_MONTAZ_KLAPKY];
Facility Montaz_predne_dvere[POC_STROJOV_MONTAZ_PREDNE_DVERE];
///////////////////////  ALEBO TAKTO  //////////////////////////////////////

Store Montaz_panel("Z1", POC_STROJOV_MONTAZ_PANEL);
Store Montaz_podstava("Z2", POC_STROJOV_MONTAZ_PODSTAVA);
Store Zeriav("Z3", POC_ZERIAV);
Store Montaz_kable("Z4", POC_STROJOV_MONTAZ_KABLE);
Store Montaz_zadny_kryt("Z5", POC_STROJOV_MONTAZ_ZADNY_KRYT);
Store Montaz_klapky("Z6", POC_STROJOV_MONTAZ_KLAPKY);
Store Montaz_predne_dvere("Z7", POC_STROJOV_MONTAZ_PREDNE_DVERE);

int main(){
    printf("Hello world\n");
}
