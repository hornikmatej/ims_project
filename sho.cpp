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
//const int POC_ZERIAV = 1;
const int POC_STROJOV_MONTAZ_KABLE = 3;
const int POC_STROJOV_MONTAZ_ZADNY_KRYT = 2;
const int POC_STROJOV_MONTAZ_KLAPKY = 2;
const int POC_STROJOV_MONTAZ_PREDNE_DVERE = 4;

//stroje v zavode

Store Montaz_panel("Z1", POC_STROJOV_MONTAZ_PANEL);
Store Montaz_podstava("Z2", POC_STROJOV_MONTAZ_PODSTAVA);

Facility Zeriav("Z3");

Store Montaz_kable("Z4", POC_STROJOV_MONTAZ_KABLE);
Store Montaz_zadny_kryt("Z5", POC_STROJOV_MONTAZ_ZADNY_KRYT);
Store Montaz_klapky("Z6", POC_STROJOV_MONTAZ_KLAPKY);
Store Montaz_predne_dvere("Z7", POC_STROJOV_MONTAZ_PREDNE_DVERE);

Histogram celk("Celkova doba v systeme", 0, 5, 20);

class Vyrobok : public Process
{
    void Behavior()
    {
        double prichod = Time; // cas prichodu
    }                          // Behavior
};                             // Vyrobok

class Prichody : public Event
{
    void Behavior()
    {
        //(new Vyrobok)->Activate();
        //Activate(Time + Exponential(8));
    }
};

int main() // popis experimentu
{
    SetOutput("data.dat");
    Init(0, 1000);
    //(new Prichody)->Activate(); // start generatora
    Run();
    // tisk statistik:

    celk.Output();
    Montaz_panel.Output();
    Montaz_podstava.Output();
    Zeriav.Output();
    Montaz_kable.Output();
    Montaz_zadny_kryt.Output();
    Montaz_klapky.Output();
    Montaz_predne_dvere.Output();
}
