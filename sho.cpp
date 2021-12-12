/*
Projekt: SHO vo vyrobe
Autori: Filip Brna (xbrnaf00), Matej Hornik (xhorni20)
Projekt simuluje výrobu rovadzaca vysokeho napatia UniGear spoločnosti ABB.
Dalsie informacie a popis najdete v dokumentacii.
jednotky času v systeme su minuty
*/

#include "simlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>



int POC_STROJOV_MONTAZ_PANEL = 3;
int POC_STROJOV_MONTAZ_PODSTAVA = 5; 
int POC_STROJOV_MONTAZ_KABLE = 5;
int POC_STROJOV_MONTAZ_ZADNY_KRYT = 2;
int POC_STROJOV_MONTAZ_KLAPKY = 2;
int POC_STROJOV_MONTAZ_PREDNE_DVERE = 4;



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

Histogram celk("Celkova doba v systeme", 200, 20, 20);

unsigned int pocet_vyrobkov_vo_vyrobe = 0;
unsigned int pocet_hotovych_vyrobkov = 0;


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

        if (Q_Z2.Length() > 0 && Montaz_podstava.Capacity() != Montaz_podstava.Used())
        {
            (Q_Z2.GetFirst())->Activate();
        }
        else if (Montaz_podstava.Capacity() == Montaz_podstava.Used() && !Zasobnik_pred_Z2.Full())
        {
            Enter(Zasobnik_pred_Z2, 1);
            Into(Q_Z2);
            Passivate();
            Leave(Zasobnik_pred_Z2, 1);
        }
        // Z2
        Enter(Montaz_podstava, 1);

        if (Q_Z2_zas.Length() > 0)
        {
            (Q_Z2_zas.GetFirst())->Activate();
        }

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

        if (Q_Z3.Length() > 0 && !Zeriav.Busy())
        {
            (Q_Z3.GetFirst())->Activate();
        }
        else if (Zeriav.Busy() && !Zasobnik_pred_Z3.Full())
        {
            Enter(Zasobnik_pred_Z3, 1);
            Into(Q_Z3);
            Passivate();
            Leave(Zasobnik_pred_Z3, 1);
        }

        // Z3
        Seize(Zeriav);
        if (Q_Z3_zas.Length() > 0 && !Zeriav.Busy())
        {
            (Q_Z3_zas.GetFirst())->Activate();
        }
        Wait(Uniform(3, 5));
        Release(Zeriav);

        if (Q_Z3.Length() > 0)
        {
            (Q_Z3.GetFirst())->Activate();
        }
        // Z4
        Enter(Montaz_kable, 1);
        Wait(Normal(30.9, 5));
        if (Zasobnik_pred_Z5.Busy())
        {
            Into(Q_Z5_zas);
            Passivate();
        }
        Leave(Montaz_kable, 1);

        if (Q_Z5.Length() > 0 && Montaz_zadny_kryt.Capacity() != Montaz_zadny_kryt.Used())
        {
            (Q_Z5.GetFirst())->Activate();
        }

        else if (Montaz_zadny_kryt.Capacity() == Montaz_zadny_kryt.Used() && !Zasobnik_pred_Z5.Busy())
        {
            Seize(Zasobnik_pred_Z5);
            Into(Q_Z5);
            Passivate();
            Release(Zasobnik_pred_Z5);
        }

        // Z5
        Enter(Montaz_zadny_kryt, 1);

        if (Q_Z5_zas.Length() > 0)
        {
            (Q_Z5_zas.GetFirst())->Activate();
        }

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

        else if (Montaz_klapky.Capacity() == Montaz_klapky.Used() && !Zasobnik_pred_Z6.Busy())
        {
            Seize(Zasobnik_pred_Z6);
            Into(Q_Z6);
            Passivate();
            Release(Zasobnik_pred_Z6);
        }
        // Z6
        Enter(Montaz_klapky, 1);

        if (Q_Z6_zas.Length() > 0 && Montaz_klapky.Capacity() != Montaz_klapky.Used())
        {
            (Q_Z6_zas.GetFirst())->Activate();
        }

        Wait(Normal(11.1, 2));
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
        else if (Montaz_predne_dvere.Capacity() == Montaz_predne_dvere.Used() && !Zasobnik_pred_Z6.Busy())
        {
            Seize(Zasobnik_pred_Z7);
            Into(Q_Z7);
            Passivate();
            Release(Zasobnik_pred_Z7);
        }
        // Z7
        Enter(Montaz_predne_dvere, 1);

        if (Q_Z7_zas.Length() > 0 && Montaz_predne_dvere.Capacity() != Montaz_predne_dvere.Used())
        {
            (Q_Z7_zas.GetFirst())->Activate();
        }

        Wait(Normal(61, 5));
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

        if (Q_Z8.Length() > 0 && !Otocna_panelov.Busy())
        {
            (Q_Z8.GetFirst())->Activate();
        }

        else if (Otocna_panelov.Busy() && !Zasobnik_pred_Z8.Full())
        {
            Enter(Zasobnik_pred_Z8, 1);
            Into(Q_Z8);
            Passivate();
            Leave(Zasobnik_pred_Z8, 1);
        }

        // Z8
        Seize(Otocna_panelov);
        if (Q_Z8_zas.Length() > 0 && !Otocna_panelov.Busy())
        {
            (Q_Z8_zas.GetFirst())->Activate();
        }
        Wait(1);
        Release(Otocna_panelov);

        if (Q_Z8.Length() > 0)
        {
            (Q_Z8.GetFirst())->Activate();
        }

        celk(Time - prichod);
        pocet_hotovych_vyrobkov++;
    } // Behavior
};    // Vyrobok

class Prichody : public Event
{
    void Behavior()
    {
        int pocet = Montaz_panel.Free();
        if (pocet != 0)
        {
            for (int i = 1; i < pocet + 1; i++)
            {
                (new Vyrobok)->Activate();
                pocet_vyrobkov_vo_vyrobe ++;
            }
        }
        Activate(Time + 1);
    } //Behavior
};    //Prichody

void print_stats(){
    printf("Pocet hotovych vyrobkov = %u\n", pocet_hotovych_vyrobkov);
    printf("Pocet vyrobkov vo vyrobe(nehotove kusy) = %u\n", pocet_vyrobkov_vo_vyrobe - pocet_hotovych_vyrobkov);
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

void run_sim(int cislo_exp){
    SetOutput(("experiment" + std::to_string(cislo_exp) + ".dat").c_str());
    Init(0, 1440 * 30); // simulacia 1 mesiac
    (new Prichody)->Activate(); // start generatora
    Run();
    print_stats();
}

int main(int argc, char **argv) // popis experimentu
{
    if (argc != 2){
        fprintf(stderr, "ERROR: Zly vstupny argument\n");
        exit(1);
    }
    int cislo_exp = atoi(argv[1]);
    if (cislo_exp == 0){
        fprintf(stderr, "ERROR: Zla vstupna hodnota\n");
        exit(1);
    }
    
    switch (cislo_exp){
        case 1:
            // experiment 1
            run_sim(cislo_exp);
            break;
        case 2:
            // experiment 2
            POC_STROJOV_MONTAZ_PODSTAVA = 4;
            run_sim(cislo_exp);
            break;
        case 3:
            // experiment 3
            run_sim(cislo_exp);
            break;
        
        default:
            fprintf(stderr, "ERROR: Zle cislo experimentu\n");
            exit(1);
            break;
    }

    return 0;
}

