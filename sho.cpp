/*
Projekt: SHO vo vyrobe
Autori: Filip Brna (xbrnaf00), Matej Hornik (xhorni20)
Projekt simuluje výrobu rovadzaca vysokeho napatia UniGear spoločnosti ABB.
Dalsie informacie a popis najdete v dokumentacii.
jednotky času v systeme su minuty
*/

#include "simlib.h"
#include <stdio.h>

const int POC_STROJOV_MONTAZ_PANEL = 4;
const int POC_STROJOV_MONTAZ_PODSTAVA = 5;
//const int POC_ZERIAV = 1;
const int POC_STROJOV_MONTAZ_KABLE = 2;
const int POC_STROJOV_MONTAZ_ZADNY_KRYT = 1;
const int POC_STROJOV_MONTAZ_KLAPKY = 1;
const int POC_STROJOV_MONTAZ_PREDNE_DVERE = 4;

//stroje v zavode

Store Montaz_panel("Z1", POC_STROJOV_MONTAZ_PANEL);
Store Montaz_podstava("Z2", POC_STROJOV_MONTAZ_PODSTAVA);

Facility Zeriav("Z3");

Store Montaz_kable("Z4", POC_STROJOV_MONTAZ_KABLE);
Store Montaz_zadny_kryt("Z5", POC_STROJOV_MONTAZ_ZADNY_KRYT);
Store Montaz_klapky("Z6", POC_STROJOV_MONTAZ_KLAPKY);
Store Montaz_predne_dvere("Z7", POC_STROJOV_MONTAZ_PREDNE_DVERE);

Histogram celk("Celkova doba v systeme", 0, 25, 12);

class Vyrobok : public Process
{
    void Behavior()
    {
        double prichod = Time; // cas prichodu
        // FACILITY
        // Seize(drtic_kuz);
        // Release(drtic_kuz);
        // STORE
        // Enter(predehrivac, 1);
        // Leave(predehrivac, 1);
        // WAIT
        // Wait(Exponential(20));
        // Wait(1);

        // Z1
        Enter(Montaz_panel, 1);
        Wait(Exponential(51.8));
        Leave(Montaz_panel, 1);
        // Z2
        Enter(Montaz_podstava, 1);
        Wait(Exponential(75.5));
        Leave(Montaz_podstava, 1);
        // Z3
        Seize(Zeriav);
        Wait(Uniform(8, 10));
        Release(Zeriav);
        // Z4
        Enter(Montaz_kable, 1);
        Wait(Exponential(9));
        Leave(Montaz_kable, 1);
        // Z5
        Enter(Montaz_zadny_kryt, 1);
        Wait(Exponential(9));
        Leave(Montaz_zadny_kryt, 1);
        // Z6
        Enter(Montaz_klapky, 1);
        Wait(Exponential(11.1));
        Leave(Montaz_klapky, 1);
        // Z7
        Enter(Montaz_predne_dvere, 1);
        Wait(Exponential(61));
        Leave(Montaz_predne_dvere, 1);

        celk(Time - prichod);
    } // Behavior
};    // Vyrobok

class Prichody : public Event
{
    void Behavior()
    {
        for (int i = 0; i < 3; i++)
        {
            (new Vyrobok)->Activate();
        }
        Activate(Time + Exponential(51.8));
    }
};

int main() // popis experimentu
{
    SetOutput("data2.dat");
    Init(0, 1000);
    (new Prichody)->Activate(); // start generatora
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
