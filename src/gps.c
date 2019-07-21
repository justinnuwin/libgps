#include "gps.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "nmea.h"
#include "serial.h"

gpgga_t gpgga;
gprmc_t gprmc;

extern void gps_init(void) {
    serial_init();
    serial_config();

    //Write commands
}

extern void gps_on(void) {
    //Write on
}

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// Compute the GPS location using decimal scale
// returns 0 if using old data
// returns 1 if using new data
extern int gps_location(loc_t *coord) {
    char buffer[256];

    int data_ready = serial_readln(buffer);
    if (!data_ready) {
        coord->hour = MAX(gpgga.hour, gprmc.hour);
        coord->minute = MAX(gpgga.minute, gprmc.minute);
        coord->second = MAX(gpgga.second, gprmc.second);
        coord->latitude = gpgga.latitude;
        coord->longitude = gpgga.longitude;
        coord->altitude = gpgga.altitude;
        coord->speed = gprmc.speed;
        coord->course = gprmc.course;
    } else {
        switch (nmea_get_message_type(buffer)) {
            case NMEA_GPGGA:
                nmea_parse_gpgga(buffer, &gpgga);

                gps_convert_deg_to_dec(&(gpgga.latitude), gpgga.lat, &(gpgga.longitude), gpgga.lon);

                coord->hour = gpgga.hour;
                coord->minute = gpgga.minute;
                coord->second = gpgga.second;

                coord->latitude = gpgga.latitude;
                coord->longitude = gpgga.longitude;
                coord->altitude = gpgga.altitude;

                break;
            case NMEA_GPRMC:
                nmea_parse_gprmc(buffer, &gprmc);

                coord->hour = gprmc.hour;
                coord->minute = gprmc.minute;
                coord->second = gprmc.second;

                coord->speed = gprmc.speed;
                coord->course = gprmc.course;

                break;
        }
    }
    return data_ready;
}

extern void gps_off(void) {
    //Write off
    serial_close();
}

// Convert lat e lon to decimals (from deg)
void gps_convert_deg_to_dec(double *latitude, char ns,  double *longitude, char we)
{
    double lat = (ns == 'N') ? *latitude : -1 * (*latitude);
    double lon = (we == 'E') ? *longitude : -1 * (*longitude);

    *latitude = gps_deg_dec(lat);
    *longitude = gps_deg_dec(lon);
}

double gps_deg_dec(double deg_point)
{
    double ddeg;
    double sec = modf(deg_point, &ddeg)*60;
    int deg = (int)(ddeg/100);
    int min = (int)(deg_point-(deg*100));

    double absdlat = round(deg * 1000000.);
    double absmlat = round(min * 1000000.);
    double absslat = round(sec * 1000000.);

    return round(absdlat + (absmlat/60) + (absslat/3600)) /1000000;
}

