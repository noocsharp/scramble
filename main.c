#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/random.h>

#define DEFAULT_LENGTH 25

static const char* pmoves[3][6] = { {"U", "D", "L", "R", "F", "B"}, {"U'", "D'", "L'", "R'", "F'", "B'"}, {"U2", "D2", "L2", "R2", "F2", "B2"} };

const char help[] = "usage: scramble [LENGTH]";

static char const** typo_scramble(uint8_t length) {
    char const** moves = calloc(length, sizeof(char*));

    if (moves == NULL) return NULL;

    uint8_t last_side = 0;

    uint8_t counter = 0;
    // TODO deal with RAND_MAX
    while (counter < length) {
        uint8_t rval = (uint8_t) rand();
        // 3 possible values for set, so 2 bits
        uint8_t set = rval & 3;
        // 6 possible values for side, so 3 bits
        uint8_t side = (rval >> 2) & 7;

        // prevent biasing of results that would occur if
        // we just took the modulus of the values
        if (set == 3 || side > 5) continue;

        // if the side is the same as before, choose another side
        if (counter != 0 && side == last_side) continue;

        moves[counter++] = pmoves[set][side];
        last_side = side;
    }

    return moves;
}

int main(int argc, char** argv) {
    uint8_t length = DEFAULT_LENGTH;

    if (argc > 2) {
        printf("%s: Too many arguments\n", argv[0]);
        return -1;
    } else if (argc == 2) {
        if (argv[1] == "-h" || argv[1] == "--help") {
            printf("%s\n", help);
            return 0;
        }

        char* endptr = NULL;
        long int newlength = strtol(argv[1], &endptr, 10);

        if (*endptr != '\0') {
            printf("%s: invalid length\n", argv[0]);
            return -2;
        }

        if (0 > newlength || newlength > 255) {
            printf("%s: invalid length, max of 255\n", argv[0]);
            return -3;
        }

        length = newlength;
    }

    uint32_t seed = 0;
    getrandom((void*) &seed, 4, GRND_NONBLOCK);
    srand(seed);

    const char** moves;

    
    if ((moves = typo_scramble(length)) != NULL) {
        for (uint8_t i = 0; i < length; ++i) {
            printf("%s ", moves[i]);
        }
        printf("\n");
        free(moves);
    }

    return 0;

}
