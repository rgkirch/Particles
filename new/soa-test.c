#include "soa.h"

int main(void) {
    Position* position = init(10, 800, 600);
    printf("%f\n", position->x[0]);
    return 0;
}
