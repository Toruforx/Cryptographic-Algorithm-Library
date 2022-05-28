#include <gmpxx.h>
#include <iostream>
#include <cstdio>
#include <time.h>
using namespace std;

mpz_t p, q, n, phi, pub_key, pri_key;
string pub_keystr, pri_keystr, nstr, m, c;

void init()
{
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(phi);
    mpz_init(pub_key);
    mpz_init(pri_key);
}

void generate_pq()
{
    gmp_randstate_t seed;
    gmp_randinit_default(seed);
    gmp_randseed_ui(seed, time(NULL));
    mpz_urandomb(p, seed, 1024);
    mpz_urandomb(q, seed, 1024);
    mpz_nextprime(p, p);
    mpz_nextprime(q, q);
}

void calculate()
{
    mpz_mul(n, q, p);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);
}

void generate_key()
{
    mpz_set_ui(pub_key, 65537);
    mpz_invert(pri_key, pub_key, phi);
    mpz_class temp_n(n);
    mpz_class temp_pub(pub_key);
    mpz_class temp_pri(pri_key);
    nstr = temp_n.get_str();
    pub_keystr = temp_pub.get_str();
    pri_keystr = temp_pri.get_str();
}

string rsa_encode(string strm)
{
    mpz_t m, temp_pub, temp_n;
    string ans = "";
    mpz_init(m);
    mpz_init(temp_pub);
    mpz_init(temp_n);
    mpz_set_str(temp_pub, pub_keystr.c_str(), 10);
    mpz_set_str(temp_n, nstr.c_str(), 10);
    for (int i = 0; i < strm.length(); i++)
    {
        mpz_set_ui(m, (unsigned long)strm[i]);
        mpz_powm(m, m, temp_pub, temp_n);
        mpz_class t(m);
        ans += t.get_str();
        ans += '\n';
    }
    mpz_clear(m);
    mpz_clear(temp_pub);
    mpz_clear(temp_n);
    return ans;
}

string rsa_decode(string strc)
{
    mpz_t c, temp_pri, temp_n;
    string ans = "", temp = "";
    mpz_init(c);
    mpz_init(temp_pri);
    mpz_init(temp_n);
    mpz_set_str(temp_pri, pri_keystr.c_str(), 10);
    mpz_set_str(temp_n, nstr.c_str(), 10);
    for (int i = 0; i < strc.length(); i++)
    {
        if (strc[i] == '\n')
        {
            mpz_set_str(c, temp.c_str(), 10);
            mpz_powm(c, c, temp_pri, temp_n);
            mpz_class t(c);
            unsigned long x = t.get_ui();
            ans += (char)x;
            temp = "";
        }
        else
        {
            temp += strc[i];
        }
    }
    return ans;
}

int main()
{
    init();
    generate_pq();
    calculate();
    generate_key();
    cout << "Plain Text: ";
    getline(cin, m);
    string c = rsa_encode(m);
    cout << "Cipher: " << c << endl;
    cout << "Decode: " << rsa_decode(c) << endl;
}
/*
#if 1
int main(int argc, void *argv[])
{
    //char *option, *text, *out;
    string option, text, out;
    if (argc != 3)
    {
        printf("Usage: rsa <--encode | --decode> <text>\n");
        return 0;
    }
    else
    {
        option = argv[1];
        text = argv[2];
        if (option == "--encode")
        {
            out = rsa_encode(text);
            cout << out << endl;
        }
        else if (option == "--decode")
        {
            out = rsa_decode(text);
            cout << out << endl;
        }
        else
        {
            printf("Usage: rsa <--encode | --decode> <text>\n");
            return 0;
        }
    }

    return 0;
}
#endif
*/