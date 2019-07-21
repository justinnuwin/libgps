#include <stdio.h>
#include <stdlib.h>
#include <gps.h>

int main(void) {
    // Open
    gps_init();

    loc_t data;

    while (1) {
        int new = gps_location(&data);

        printf("%c %02d:%02d:%02d %lf %lf\n",
                new ? '!' : '-', data.hour, data.minute, data.second, data.latitude, data.longitude);
    }

    return EXIT_SUCCESS;
}

