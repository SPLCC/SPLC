#define Bsub b
#define SUMAB a + Bsub
#define SUMAAB a + \
    SUMAB
#define Csub b Csub

int main()
{
    int a, b, c;
    c = SUMAAB;
    Csub;
    return 0;
}