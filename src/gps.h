#ifndef _GPS_H_
#define _GPS_H_

#ifdef __cplusplus
extern "C" {
#endif

struct location {
    int hour;
    int minute;
    int second;
    double latitude;
    double longitude;
    double speed;
    double altitude;
    double course;
};
typedef struct location loc_t;

// Initialize device
extern void gps_init(void);
// Activate device
extern void gps_on(void);
// Get the actual location
// returns 0 if using old data
// returns 1 if using new data
extern int gps_location(loc_t *);


// Turn off device (low-power consumption)
extern void gps_off(void);

// -------------------------------------------------------------------------
// Internal functions
// -------------------------------------------------------------------------

// convert deg to decimal deg latitude, (N/S), longitude, (W/E)
void gps_convert_deg_to_dec(double *, char, double *, char);
double gps_deg_dec(double);

#ifdef __cplusplus
}
#endif

#endif
