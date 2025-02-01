// original BASIC code at https://www.retrobrewcomputers.org/forum/index.php?t=msg&th=201&goto=4704&#msg_4704
// translated C code, CC0

#ifdef DEBUG
#include <stdio.h>
#endif
#define DEC_DIGIT    6
#define DEC_DIGIT_2    DEC_DIGIT*2
#define POSITIVE     0
#define NEGATIVE     1

#define GREATER        1
#define EQUAL        0
#define LESS        -1

typedef struct {
    signed char sign;        // POSTIVE or NEGATIVE
    signed char integer[DEC_DIGIT];
    signed char decimal[DEC_DIGIT];
} real_char_t;

void real_char_set(real_char_t *a, int integer, int decimal, int sign);
int real_char_compare(real_char_t a, real_char_t b);
void real_char_add_abs(real_char_t a, real_char_t b, real_char_t *c);
void real_char_diff_abs(real_char_t a, real_char_t b, real_char_t *c);
void real_char_add(real_char_t a, real_char_t b, real_char_t *c);
void real_char_diff(real_char_t a, real_char_t b, real_char_t *c);
void real_char_mul(real_char_t a, real_char_t b, real_char_t *c);

void real_char_set(real_char_t *a, int integer, int decimal, int sign) {
    for (int i = 0; i < DEC_DIGIT; i++) {
        a->integer[i] = integer % 10;
        integer /= 10;
    }
    for (int i = 0; i < DEC_DIGIT; i++) {
         a->decimal[i] = decimal % 10;
        decimal /= 10;
    }
    a->sign = sign;
}

void real_char_diff_abs(real_char_t a, real_char_t b, real_char_t *c) {
    // decimal
    for (int i = 0; i < DEC_DIGIT; i++) {
        if (a.decimal[i] < b.decimal[i]) {
            a.decimal[i] += 10;
            if (i < DEC_DIGIT-1) {
                a.decimal[i+1]--;
            } else {
                a.integer[0]--;
            }
        }
        c->decimal[i] = a.decimal[i] - b.decimal[i];
    }
    // carry over
    if (a.integer[0] < 0) {
        for (int  i= 0; i < DEC_DIGIT; i++) {
            if (i < DEC_DIGIT-1  && a.integer[i] < 0) {
                a.integer[i] += 10;
                a.integer[i+1]--;
            }
        }
    }
    // integer
    for (int i = 0; i < DEC_DIGIT; i++) {
        if (a.integer[i] < b.integer[i]) {
            if (i < DEC_DIGIT-1) {
                a.integer[i] += 10;
                a.integer[i+1]--;
            }
        }
        c->integer[i] = a.integer[i] - b.integer[i];
    }
    //sign
    c->sign = POSITIVE;
}

void real_char_add_abs(real_char_t a, real_char_t b, real_char_t *c) {
    //decimal
    for (int i = 0; i < DEC_DIGIT; i++) {
        c->decimal[i] = a.decimal[i] + b.decimal[i];
        if (c->decimal[i] >= 10) {
            c->decimal[i] = c->decimal[i] - 10;
            if (i < DEC_DIGIT-1) {
                a.decimal[i+1]++;
            } else {
                a.integer[0]++;
            }
        }
    }
    // carry over
    if (a.integer[0] > 9) {
        for (int i = 0; i < DEC_DIGIT; i++) {
            if (i < DEC_DIGIT-1 && a.integer[i] > 9) {
                a.integer[i] -= 10;
                a.integer[i+1]++;
            }
        }
    }
    //integer
    for (int i = 0; i < DEC_DIGIT; i++) {
        c->integer[i] = a.integer[i] + b.integer[i];
        if (c->integer[i] >= 10) {
            c->integer[i] -= 10;
            if (i < DEC_DIGIT-1) {
                a.integer[i+1]++;
            }
        }
    }
    c->sign = a.sign;
}

int real_char_compare(real_char_t a, real_char_t b) {
    for (int i = 0; i < DEC_DIGIT; i++) {
        if (a.integer[DEC_DIGIT-1-i] > b.integer[DEC_DIGIT-1-i]) return GREATER;
        if (a.integer[DEC_DIGIT-1-i] < b.integer[DEC_DIGIT-1-i]) return LESS;
    }
    for (int i = 0; i < DEC_DIGIT; i++) {
        if (a.decimal[DEC_DIGIT-1-i] > b.decimal[DEC_DIGIT-1-i]) return GREATER;
        if (a.decimal[DEC_DIGIT-1-i] < b.decimal[DEC_DIGIT-1-i]) return LESS;
    }
    return EQUAL;
}

void real_char_add(real_char_t a, real_char_t b, real_char_t *c) {
    if (a.sign == b.sign) {
        real_char_add_abs(a, b, c);
        c->sign = a.sign;
    } else {
        if (real_char_compare(a, b) >= EQUAL) { //GREATER or EQUAL
            real_char_diff_abs(a, b, c);
            c->sign = (a.sign == NEGATIVE) ? NEGATIVE : POSITIVE;
        } else {
            real_char_diff_abs(b, a, c);
            c->sign = (b.sign == NEGATIVE) ? NEGATIVE : POSITIVE;
        }
    }
}

void real_char_diff(real_char_t a, real_char_t b, real_char_t *c) {
    real_char_t b_nega;
    b_nega = b;
    b_nega.sign = (b_nega.sign == NEGATIVE) ? POSITIVE : NEGATIVE;
    real_char_add(a, b_nega, c);
}

void real_char_mul(real_char_t a, real_char_t b, real_char_t *c) {
    int quot = 0;
    signed char a12[DEC_DIGIT_2], b12[DEC_DIGIT_2], c12[DEC_DIGIT_2], c24[DEC_DIGIT_2*2]; 

    // clear return value
    for (int i = 0; i < DEC_DIGIT_2; i++) {
        a12[i] = b12[i] = c12[i] = c24[i] = c24[i+DEC_DIGIT_2] = 0;
    }

    // convert real_char_t to char[]
    for (int i = 0; i < DEC_DIGIT; i++) {
        a12[i]=a.decimal[i];
        a12[i+DEC_DIGIT]=a.integer[i];
        b12[i]=b.decimal[i];
        b12[i+DEC_DIGIT]=b.integer[i];
    }

    // calculate
    for (int j = 0; j < DEC_DIGIT_2; j++) {
        for (int i = 0; i < DEC_DIGIT_2; i++) {
            c12[i] = a12[i] * b12[j] + quot;
            quot = 0;
            if (i < DEC_DIGIT_2 && c12[i] > 9) {
                quot = c12[i] / 10;
                c12[i] = c12[i] % 10;
            }
        }
        quot = 0;
        for (int i=0; i<DEC_DIGIT_2; i++) {
            c24[i+j] = c24[i+j] + c12[i];
        }
    }

    // carry over 
    quot=0;
    for (int i=0; i<DEC_DIGIT_2*2; i++) {
        c24[i] += quot;
        quot = 0;
        if (c24[i] > 9) {
            quot = c24[i] / 10;
            c24[i] = c24[i] % 10;
        }
    }

    // set return value
    for (int i=0; i<DEC_DIGIT; i++) {
        c->integer[i] = c24[DEC_DIGIT_2+i];
        c->decimal[i] = c24[DEC_DIGIT+i];
    }
    c->sign = (a.sign == b.sign) ? POSITIVE : NEGATIVE; 
}

#ifdef DEBUG
int main(int argc, char *argv[]) {
#else
int asciiart(void) {
#endif
    int x, y, i;
    //float a, b, ca, cb, t;
    real_char_t ra, rb, rca, rcb, rt, realx, realy, constx, consty, const2, wa, wb, wa2, wb2, w2ab, const4;
    const static unsigned char c[] = "0123456789ABCDEF ";

    //constx.integer = 0; constx.decimal = 45800; constx.sign = 0;
    //consty.integer = 0; consty.decimal = 83330; consty.sign = 0;
    //const2.integer = 2; const2.decimal = 0; const2.sign = 0;
    real_char_set(&constx, 0, 45800, POSITIVE);    // +0.045800
    real_char_set(&consty, 0, 83330, POSITIVE);    // +0.083330
    real_char_set(&const2, 2, 0, POSITIVE);    // +2.0
    real_char_set(&const4, 4, 0, POSITIVE);    // +4.0

    for (y = -12; y <= 12; y++) {
        real_char_set(&realy, (y >= 0) ? y : -1*y , 0, (y >= 0) ? POSITIVE : NEGATIVE);

        for (x = -39; x <= 39; x++) {
            real_char_set(&realx, (x >= 0) ? x : -1*x , 0, (x >= 0) ? POSITIVE : NEGATIVE);

            //a = ca = x * 0.0458;
            //b = cb = y * 0.08333;
            real_char_mul(realx, constx, &rca);
            real_char_mul(realy, consty, &rcb);
            ra = rca;
            rb = rcb;
    
            for (i = 0; i <= 15; i++) {
                //t = (a * a) - (b * b) + ca;
                real_char_mul(ra, ra, &wa);
                real_char_mul(rb, rb, &wb);
                real_char_diff(wa, wb, &w2ab);
                real_char_add(w2ab, rca, &rt);
        
                //b = (2 * a * b) + cb;
                real_char_mul(const2, ra, &wa2);
                real_char_mul(wa2, rb, &wb2);
                real_char_add(wb2, rcb, &rb);
        
                //a = t;
                ra = rt;
        
                // if (a*a + b*b >=4) {
                real_char_mul(ra, ra, &wa);
                real_char_mul(rb, rb, &wb);
                real_char_add(wa, wb, &w2ab);
                if (real_char_compare(w2ab, const4) >= EQUAL) {
                    break;
                }
            }
            putchar(c[i]);
        }
        putchar('\n');
    }
    return 0;
}
