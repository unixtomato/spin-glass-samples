#include <cstdio>
#include <cstdlib>
#include <random>
#include <cmath>
#include <ctime>
#include <cassert>

using namespace std;

//#define L 32
//#define K 0.44068679350977147

//#define K0 // integral part
//#define K1 // decimal part
//#define K // combined, to be specified in driver

//#define B 0.001

#define N (L*L)

int sr1[L][L];
int sr2[L][L];

int s[L][L]; // effective lattice

int jd[L][L];
int jr[L][L];

// random generator
random_device rd;
mt19937_64 gen(rd());
uniform_real_distribution<> dis(0.0, 1.0);

void initialize(int sr[][L])
{
    // initialize lattice of spins
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            if (dis(gen) < 0.5)
                sr[i][j] = 1;
            else
                sr[i][j] = -1;
        }
    }

}

void randomize_coupling(int jj[][L])
{
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            if (dis(gen) < 0.5)
                jj[i][j] = 1;
            else
                jj[i][j] = -1;
        }
    }
}

void sweep(int sr[][L]) // sweep replica lattice
{
    
    int i, j, k;
    int in, jn, sum;
    double delta;

    for (k = 0; k < N; ++k) {

        i = L * dis(gen);
        j = L * dis(gen);

        if ((in = i + 1) >= L) in -= L;
        sum = jd[i][j] * sr[in][j];
        if ((in = i - 1) <  0) in += L;
        sum += jd[in][j] * sr[in][j];
        if ((jn = j + 1) >= L) jn -= L;
        sum += jr[i][j] * sr[i][jn];
        if ((jn = j - 1) <  0) jn += L;
        sum += jr[i][jn] * sr[i][jn];

        delta = (K * sum) * sr[i][j];

        if (delta <= 0)
            sr[i][j] = -sr[i][j];
        else if (dis(gen) < exp(-2.0 * delta))
            sr[i][j] = -sr[i][j];
    }
}

void effective()
{
    for (int i = 0; i < L; ++i)
        for (int j = 0; j < L; ++j)
            s[i][j] = sr1[i][j] * sr2[i][j];
}

int main(int argc, char* argv[])
{

//#define EQUIL 10000
//#define QUNCH 10 //realizations
//#define NCONF 10000

    const clock_t begin_time = clock();

    assert(argc == 3);

    int cpu = stoi(argv[2]);
    
    
    
    char filename [200];
    sprintf(filename, "%s/glass_l%03d_k%03d%02d_%02d.dat", argv[1], L, K0, K1, cpu);
    FILE* pf = fopen(filename, "wb");

    //fprintf(stderr, "%s\n", filename);
    if (pf == NULL)
        fprintf(stderr, "fail open\n");


    for (int q = 0; q < QUNCH; ++q) {

        initialize(sr1);
        initialize(sr2);
        randomize_coupling(jd);
        randomize_coupling(jr);

        for (int k = 0; k < EQUIL; ++k) {
            sweep(sr1);
            sweep(sr2);
        }

        for (int kk = 0; kk < NCONF; ++kk) {
//#define DECOR 10
            for (int k = 0; k < DECOR; ++k) {
                sweep(sr1);
                sweep(sr2);
            }

            effective();

            for (int i = 0; i < L; ++i)
                fwrite(s[i], sizeof(int), L, pf);

            int cnt = kk + q * NCONF;
            if (cnt % (NCONF*QUNCH/5) == 0)
                fprintf(stderr, "%d %d-%d-%d %.2f\n", cpu, cnt, NCONF, QUNCH, float(clock()-begin_time)/CLOCKS_PER_SEC);
        }

    }
    fclose(pf);
}



