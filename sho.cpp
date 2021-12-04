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
Facility Otocna_panelov("Z8");

//zasobniky na linke
Store Zasobnik_pred_Z2("Zasobnik pred Z2", 9);
Store Zasobnik_pred_Z3("Zasobnik pred Z3", 2);
Facility Zasobnik_pred_Z5("Zasobnik pred Z5");
Facility Zasobnik_pred_Z6("Zasobnik pred Z6");
Facility Zasobnik_pred_Z7("Zasobnik pred Z7");
Store Zasobnik_pred_Z8("Zasobnik pred Z8", 2);

Queue Q_Z2;
Queue Q_Z2_zas;
Queue Q_Z3;
Queue Q_Z3_zas;
Queue Q_Z5;
Queue Q_Z5_zas;
Queue Q_Z6;
Queue Q_Z6_zas;
Queue Q_Z7;
Queue Q_Z7_zas;
Queue Q_Z8;
Queue Q_Z8_zas;

Histogram celk("Celkova doba v systeme", 220, 20, 15);
bool is_day;

// trieda pre striedania dna a noci
class Den : public Process
{
    unsigned short den;
    void Behavior()
    {
        Priority = 1; // priorita pri zaberani pristrojov
        is_day = true;
        Wait(8 * 60); // modelovy cas je v minutach
        den = 1;
        while (1)
        {
            //nastala noc tak stroje pozastavi
            is_day = false;
            Enter(Montaz_panel, POC_STROJOV_MONTAZ_PANEL);
            Enter(Montaz_podstava, POC_STROJOV_MONTAZ_PODSTAVA);
            Seize(Zeriav);
            Enter(Montaz_kable, POC_STROJOV_MONTAZ_KABLE);
            Enter(Montaz_zadny_kryt, POC_STROJOV_MONTAZ_ZADNY_KRYT);
            Enter(Montaz_klapky, POC_STROJOV_MONTAZ_KLAPKY);
            Enter(Montaz_predne_dvere, POC_STROJOV_MONTAZ_PREDNE_DVERE);
            Seize(Otocna_panelov);

            if (den == 5)
            {
                //je víkend
                Wait((24 + 24 + 16) * 60);
                den = 1;
            }
            else
            {
                // noc
                Wait(16 * 60);
                den++;
            }
            is_day = true;
            //nastal den tak stroje pracuju
            Leave(Montaz_panel, POC_STROJOV_MONTAZ_PANEL);
            Leave(Montaz_podstava, POC_STROJOV_MONTAZ_PODSTAVA);
            Release(Zeriav);
            Leave(Montaz_kable, POC_STROJOV_MONTAZ_KABLE);
            Leave(Montaz_zadny_kryt, POC_STROJOV_MONTAZ_ZADNY_KRYT);
            Leave(Montaz_klapky, POC_STROJOV_MONTAZ_KLAPKY);
            Leave(Montaz_predne_dvere, POC_STROJOV_MONTAZ_PREDNE_DVERE);
            Release(Otocna_panelov);

            Wait(8 * 60);
        }
    }
};

class Vyrobok : public Process
{
    void Behavior()
    {
        double prichod = Time; // cas prichodu
        // FACILITY
        // Seize(stroj);
        // Release(stroj);
        // STORE
        // Enter(stroj, 1);
        // Leave(stroj, 1);
        // WAIT
        // Wait(Normal(20));
        // Wait(1);

        // Z1
        Enter(Montaz_panel, 1);
        Wait(Normal(51.8, 5));
        if (Zasobnik_pred_Z2.Full())
        {
            Into(Q_Z2_zas);
            Passivate();
        }
        Leave(Montaz_panel, 1);

        if (Q_Z2_zas.Length() > 0 && Montaz_podstava.Capacity() != Montaz_podstava.Used())
        {
            (Q_Z2_zas.GetFirst())->Activate();
        }

        if (Montaz_podstava.Capacity() == Montaz_podstava.Used())
        {
            Enter(Zasobnik_pred_Z2, 1);
            Into(Q_Z2);
            Passivate();
            Leave(Zasobnik_pred_Z2, 1);
        }
        // Z2
        Enter(Montaz_podstava, 1);
        Wait(Normal(75.5, 5));
        if (Zasobnik_pred_Z3.Full())
        {
            Into(Q_Z3_zas);
            Passivate();
        }
        Leave(Montaz_podstava, 1);

        if (Q_Z2.Length() > 0)
        {
            (Q_Z2.GetFirst())->Activate();
        }

        if (Q_Z3_zas.Length() > 0 && !Zeriav.Busy())
        {
            (Q_Z3_zas.GetFirst())->Activate();
        }

        if (Zeriav.Busy())
        {
            Enter(Zasobnik_pred_Z3, 1);
            Into(Q_Z3);
            Passivate();
            Leave(Zasobnik_pred_Z3, 1);
        }

        // Z3
        Seize(Zeriav);
        Wait(Uniform(3, 5));
        Release(Zeriav);

        if (Q_Z3.Length() > 0)
        {
            (Q_Z3.GetFirst())->Activate();
        }
        // Z4
        Enter(Montaz_kable, 1);
        Wait(Normal(30.9, 7));
        if (Zasobnik_pred_Z5.Busy())
        {
            Into(Q_Z5_zas);
            Passivate();
        }
        Leave(Montaz_kable, 1);

        if (Q_Z5_zas.Length() > 0 && Montaz_zadny_kryt.Capacity() != Montaz_zadny_kryt.Used())
        {
            (Q_Z5_zas.GetFirst())->Activate();
        }

        if (Montaz_zadny_kryt.Capacity() == Montaz_zadny_kryt.Used())
        {
            Seize(Zasobnik_pred_Z5);
            Into(Q_Z5);
            Passivate();
            Release(Zasobnik_pred_Z5);
        }

        // Z5
        Enter(Montaz_zadny_kryt, 1);
        Wait(Normal(9, 2));
        if (Zasobnik_pred_Z6.Busy())
        {
            Into(Q_Z6_zas);
            Passivate();
        }
        Leave(Montaz_zadny_kryt, 1);

        if (Q_Z5.Length() > 0)
        {
            (Q_Z5.GetFirst())->Activate();
        }

        if (Q_Z6.Length() > 0 && Montaz_klapky.Capacity() != Montaz_klapky.Used())
        {
            (Q_Z6.GetFirst())->Activate();
        }

        if (Montaz_klapky.Capacity() == Montaz_klapky.Used())
        {
            Seize(Zasobnik_pred_Z6);
            Into(Q_Z6);
            Passivate();
            Release(Zasobnik_pred_Z6);
        }
        // Z6
        Enter(Montaz_klapky, 1);
        Wait(Normal(11.1, 3));
        if (Zasobnik_pred_Z7.Busy())
        {
            Into(Q_Z7_zas);
            Passivate();
        }
        Leave(Montaz_klapky, 1);

        if (Q_Z6.Length() > 0)
        {
            (Q_Z6.GetFirst())->Activate();
        }

        if (Q_Z7.Length() > 0 && Montaz_predne_dvere.Capacity() != Montaz_predne_dvere.Used())
        {
            (Q_Z7.GetFirst())->Activate();
        }

        if (Montaz_predne_dvere.Capacity() == Montaz_predne_dvere.Used())
        {
            Seize(Zasobnik_pred_Z7);
            Into(Q_Z7);
            Passivate();
            Release(Zasobnik_pred_Z7);
        }
        // Z7
        Enter(Montaz_predne_dvere, 1);
        Wait(Normal(61, 6));
        if (Zasobnik_pred_Z8.Full())
        {
            Into(Q_Z8_zas);
            Passivate();
        }
        Leave(Montaz_predne_dvere, 1);

        if (Q_Z7.Length() > 0)
        {
            (Q_Z7.GetFirst())->Activate();
        }

        if (Q_Z8_zas.Length() > 0 && !Otocna_panelov.Busy())
        {
            (Q_Z8_zas.GetFirst())->Activate();
        }

        if (Otocna_panelov.Busy())
        {
            Enter(Zasobnik_pred_Z8, 1);
            Into(Q_Z8);
            Passivate();
            Leave(Zasobnik_pred_Z8, 1);
        }

        // Z8
        Seize(Otocna_panelov);
        Wait(1);
        Release(Otocna_panelov);

        if (Q_Z8.Length() > 0)
        {
            (Q_Z8.GetFirst())->Activate();
        }

        celk(Time - prichod);
    } // Behavior
};    // Vyrobok

class Prichody : public Event
{
    void Behavior()
    {
        int pocet = Montaz_panel.Free();
        if (pocet != 0 && is_day)
        {
            printf("\n%0.0f minuta-", Time);
            for (int i = 1; i < pocet + 1; i++)
            {
                printf("%d.,  ", i);
                (new Vyrobok)->Activate();
            }
        }
        Activate(Time + 1);
    } //Behavior
};    //Prichody

int main() // popis experimentu
{
    SetOutput("data.dat");
    Init(0, 10000);
    (new Prichody)->Activate(); // start generatora
    (new Den)->Activate();
    Run();
    // tisk statistik:
    celk.Output();
    Montaz_panel.Output();
    Zasobnik_pred_Z2.Output();
    Montaz_podstava.Output();
    Zasobnik_pred_Z3.Output();
    Zeriav.Output();
    Montaz_kable.Output();
    Zasobnik_pred_Z5.Output();
    Montaz_zadny_kryt.Output();
    Zasobnik_pred_Z6.Output();
    Montaz_klapky.Output();
    Zasobnik_pred_Z7.Output();
    Montaz_predne_dvere.Output();
    Zasobnik_pred_Z8.Output();
    Otocna_panelov.Output();
}
