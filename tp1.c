#include <stdio.h>

// #include <stdlib.h>
// #include <assert.h>

#include <gmp.h>

// g mpz_t O maior divisor comum de a e b. x mpz_t Inteiro tal que ax + by = g. y mpz_t Inteiro tal que ax + by = g.
void mdc_estendido(mpz_t g, mpz_t x, mpz_t y, const mpz_t a, const mpz_t b){  
    if (mpz_cmp_ui(b, 0) == 0){                              // se b == 0  
        mpz_set(g, a);                                      // g será a na próxima chamada
        mpz_set_ui(x, 1);                                   // x será 1 na próxima chamada
        mpz_set_ui(y, 0);                                   // y será 0 na próxima chamada
        return ;                                            // como g, x, y são ponteiros é como se retornassemos (a, 1, 0)
    }   
    
    mpz_t q, r, old_x, old_y, aux1, aux2;
    mpz_inits(q, r, old_x, old_y, aux1, aux2, NULL);

    mpz_fdiv_qr(q, r, a, b);                               // sejam q,r com a = q*b + r e 0 <= e < b
    mdc_estendido(g, old_x, old_y, b, r);                  // (g, x', y') = euclides(b, r)

    mpz_mul(aux1, q, old_y);                                // aux1 = q * y'
    mpz_sub(aux2, old_x, aux1);                             // aux2 = x' - aux1
    mpz_set(x, old_y);                                      // x = y' na próxima chamada 
    mpz_set(y, aux2);                                       // y = x' - q * y' na próxima chamada

    mpz_clears(q, r, old_x, old_y, aux1, aux2, NULL);   
}

int inverso_modular(mpz_t r, const mpz_t a, const mpz_t n){
    mpz_t g, x, y;
    mpz_inits(g, x, y, NULL);
    mdc_estendido(g, x, y, a, n);                            // g = mdc(a, n)   

    if (mpz_cmp_ui(g,1) != 0){                               //se g for diferente de 1 retorna 0      
        mpz_clears(g,x,y,NULL);
        return 0;    
    }
    mpz_mod(r, x, n);                                        // caso contrario resposta = x e retorna 1
    mpz_clears(g,x,y,NULL);
    return 1;

}

void exp_binaria(mpz_t r, const mpz_t b, const mpz_t e, const mpz_t n) {
    mpz_t potenciaAtual, expoenteAtual, quociente, resto, aux;
    mpz_inits(potenciaAtual, expoenteAtual, quociente, resto, aux, NULL);

    mpz_mod(potenciaAtual, b, n);
    mpz_set(expoenteAtual, e);
    mpz_set_ui(r, 1);

    while(mpz_cmp_ui(expoenteAtual, 0) != 0){                   //enquanto expoente atual for maior do que 0
        mpz_fdiv_qr_ui(quociente, resto, expoenteAtual, 2);
        if(mpz_cmp_ui(resto, 1) == 0){                          //se expoente atual for impar
            mpz_mul(aux, r, potenciaAtual);                     // aux = resposta * potenciaAtual
            mpz_mod(r, aux, n);                                 // resposta = aux % modulo
        }
        mpz_mul(potenciaAtual, potenciaAtual, potenciaAtual);   // potenciaAtual = potenciaAtual ^ 2
        mpz_mod(potenciaAtual, potenciaAtual, n);               // potenciaAtual = potenciaAtual % n
        mpz_set(expoenteAtual, quociente);                      // quociente é o resultado da divisão inteira de e/2 logo, expoenteAtual = expoenteAtual / 2
    }
    
    mpz_clears(potenciaAtual, expoenteAtual, quociente, resto, aux, NULL);
}

// int main(){ // mdc
//     mpz_t my_ans, corr_ans, a, b, x, y, aux1, aux2, aux3;
//     mpz_inits(my_ans, corr_ans, a, b, x, y, aux1, aux2, aux3, NULL);
    
//     mpz_set_ui(a, 60123);
//     mpz_set_ui(b, 11123);

//     for(int i = 0; i < 100000; i++){
//         int r_a = rand() % 2000000000000000;
//         int r_b = rand() % 2000000000000000;
        
//         mpz_set_ui(a, r_a);
//         mpz_set_ui(b, r_b);
        
//         mdc_estendido(my_ans, x, y, a, b);
//         mpz_gcd(corr_ans, a, b);

//         mpz_mul(aux1, a, x);
//         mpz_mul(aux2, b, y);
//         mpz_add(aux3, aux2, aux1);

//         // gmp_printf("my ans: %Zd, correct ans: %Zd \n", my_ans, corr_ans);
//         assert(mpz_cmp(my_ans, corr_ans) == 0);            
//         assert(mpz_cmp(my_ans, aux3) == 0);            
//     }   
// }

// int main(){ // inverso modular
//     mpz_t my_ans, corr_ans, a, n;
//     mpz_inits(my_ans, corr_ans, a, n, NULL);
    
//     for(int i = 0; i < 10000; i++){
//         int r_a = rand() % 20000000;
//         int r_n = rand() % 20000;
        
//         mpz_set_ui(a, r_a);
//         mpz_set_ui(n, r_n);
        
//         int my_ret = inverso_modular(my_ans, a, n);
//         int corr_ret = mpz_invert(corr_ans, a, n);
               
//         assert(my_ret == 0 && corr_ret == 0 || my_ret != 0 && corr_ret != 0);

//         if(my_ret) 
//             assert(mpz_cmp(my_ans, corr_ans) == 0);                   
//     }   
// }

// int main(){ // exponenciacao binaria
//     mpz_t my_ans, corr_ans, r, b, e, n;
//     mpz_inits(my_ans, corr_ans, r, b, e, n, NULL);
    
//     for(int i = 0; i < 100000; i++){
//         int r_b = rand() % 2000000000000000;
//         int r_e = rand() % 2000000000000000;
//         int r_n = rand() % 2000000000000000;
        
//         mpz_set_ui(b, r_b);
//         mpz_set_ui(e, r_e);
//         mpz_set_ui(n, r_n);
        
//         exp_binaria(my_ans, b, e, n);

//         mpz_powm(corr_ans, b, e, n);

//         // gmp_printf("my ans: %Zd, correct ans: %Zd \n", my_ans, corr_ans);
//         assert(mpz_cmp(my_ans, corr_ans) == 0);               
//     }   
// }