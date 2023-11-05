#include <iostream>

using namespace std;

int numdeck = 6;
int upcard;

double player_stand(int ptot, int pace, int cards, int deck[]);
void dealer(int old_tot, int old_ace, int cards, double prob, int deck[], double dealer_prob[]);

int main()
{
    int ch, i,j,deck[11], pc1, pc2;
    double dealer_prob[7], ev_stand, ev_hit, ev_double, ev_split;
    for (i=1; i<=9; i++)
        deck[i] = 4 * numdeck;
    deck[10] = 16 * numdeck;
    deck[0] = 52 * numdeck;
    cerr << "1. Dealer probabilities\n";
    cerr << "2. Player hand\n";
    cin >> ch;


    if (ch==1)
    {
        for (i=1; i<=10; i++)
        {
            cout << "up card = " << i << "\n";
            deck[i]--;
            deck[0]--;
            dealer(i, (i==1?1:0), 1, 1.0, deck, dealer_prob);
            for (j = 0; j <= 6; j++)
                cout << j << "\t" << dealer_prob[j] << "\n";
            deck[i]++;
            deck[0]++;
        }
    }
    else if (ch == 2)
    {
        cerr << "Player card 1: ";
        cin >> pc1;
        cerr << "Player card 2: ";
        cin >> pc2;
        cerr << "Dealer up card: ";
        cin >> upcard;
        deck[pc1]--;
        deck[pc2]--;
        deck[upcard]--;
        deck[0] -= 3;
        ev_stand=player_stand(pc1+pc2, (pc1==1?1:0) + (pc2==1?1:0), 2, deck);
        cerr << "EV stand =\t" << ev_stand << "\n";
    }
}

double player_stand(int ptot, int pace, int cards, int deck[])
{
    double ev_stand, dealer_prob[7];
    if ((ptot == 11) && (pace == 1) && (cards == 2))
    {
        if ((upcard > 1) && (upcard < 10)) // winning BJ
            return 1.5;
        else
        {
            dealer(upcard, (upcard == 1 ? 1 : 0), 1, 1.0, deck, dealer_prob);
            return 1.5 * (dealer_prob[0] + dealer_prob[1] + dealer_prob[2] + dealer_prob[3] + dealer_prob[4] + dealer_prob[6]);
        }
    }
    else if (ptot > 21) // player busts
        return -1.0;
    else
    {
        if ((ptot <= 11) && (pace > 0)) // soft
            ptot += 10;
        dealer(upcard, (upcard == 1 ? 1 : 0), 1, 1.0, deck, dealer_prob);
        ev_stand = dealer_prob[6];
        ev_stand -= dealer_prob[5];
        for (int i=0; i<=4; i++)
        {
            if (ptot > i + 17)
                ev_stand += dealer_prob[i];
            else if (ptot < i +17)
                ev_stand -= dealer_prob[i];
        }
    }
    return ev_stand;
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
        prob /= (double)deck[next_card] / (double)deck[0];
    }
}
