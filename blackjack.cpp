#include <iostream>

using namespace std;

int numdeck = 6;

void dealer(int old_tot, int old_ace, int cards, double prob, int deck[], double dealer_prob[]);

int main()
{
    int i,deck[11];
    double dealer_prob[7];
    for (i=1; i<=9; i++)
        deck[i] = 4 * numdeck;
    deck[10] = 16 * numdeck;
    deck[0] = 52 * numdeck;
    dealer(5, 0, 1, 1.0, deck, dealer_prob);
    for (i = 0; i <= 6; i++)
        cerr << i << "\t" << dealer_prob[i] << "\n";
}

/*
 * Dealer Prob
 * 0 = 17
 * 1 = 18
 * 2 = 19
 * 3 = 20
 * 4 = 21
 * 5 = bj
 * 6 = bust */

void dealer(int old_tot, int old_ace, int cards, double prob, int deck[], double dealer_prob[])
{
    int i,next_card,new_tot,new_ace;
    if (cards == 1)
    {
        for (i = 0; i <= 6; i++)
            dealer_prob[i] = 0.0;
    }
    for (next_card = 1; next_card <= 10; next_card++)
    {
        prob *= (double)deck[next_card] / (double)deck[0];
        deck[next_card]--;
        deck[0]--;
        new_tot = old_tot + next_card;
        new_ace = old_ace + (next_card == 1 ? 1 : 0);
        if ((cards==1) && (new_tot==11) && (new_ace == 1)) // blackjack}}
            dealer_prob[5] += prob;
        else if (new_tot > 21)
            dealer_prob[6] += prob;
        else if (new_tot >= 17)
            dealer_prob[new_tot -17] += prob;
        else if ((new_tot >= 7) && (new_tot<=11) && (new_ace > 0))
            dealer_prob[new_tot - 7] += prob;
        else
            dealer(new_tot, new_ace, cards + 1, prob, deck, dealer_prob);
        deck[0]++;
        deck[next_card]++;
    }
}
