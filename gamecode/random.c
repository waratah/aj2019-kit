#include "random.h"

unsigned rand();
static unsigned _seed;

void initialise_random( unsigned seed ) {
    _seed = seed; 
}

int random_value( int limit ) {
    return rand( &_seed ) % limit;
}

unsigned rand()
{
    const unsigned N = 0x7fffffff;
    const unsigned G = 48271u;

    /*
        Indirectly compute state*G%N.

        Let:
          div = state/(N/G)
          rem = state%(N/G)

        Then:
          rem + div*(N/G) == state
          rem*G + div*(N/G)*G == state*G

        Now:
          div*(N/G)*G == div*(N - N%G) === -div*(N%G)  (mod N)

        Therefore:
          rem*G - div*(N%G) === state*G  (mod N)

        Add N if necessary so that the result is between 1 and N-1.
    */
    unsigned div = _seed / (N / G);  /* max : 2,147,483,646 / 44,488 = 48,271 */
    unsigned rem = _seed % (N / G);  /* max : 2,147,483,646 % 44,488 = 44,487 */

    unsigned a = rem * G;        /* max : 44,487 * 48,271 = 2,147,431,977 */
    unsigned b = div * (N % G);  /* max : 48,271 * 3,399 = 164,073,129 */

    return _seed = (a > b) ? (a - b) : (a + (N - b));
}

void destroy_random() {

}