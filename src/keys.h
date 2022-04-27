#ifndef KEYS_H
#define KEYS_H

#define PORT 8888

/*HEADER COMMANDS*/
#define HEAD_HANDSHAKE    0xA0
#define HEAD_REQUEST      0xA1
#define HEAD_WRITE        0xA2

/*HANDSHAKE COMMANDS*/
#define HS_HELLO        0xB0
#define HS_WELCOME      0xB1
#define HS_REGISTER     0xB2
#define HS_GRANT        0xB3
#define HS_REVOKE       0xB4

/*HANDSHAKE STATUS*/
#define HS_PENDING      0xB5
#define HS_AUTH         0xB6

/*SENSOR ADDRESS*/
#define SA_POWER            0xC0
#define SA_WIFI             0xC1
#define SA_BLUETOOTH        0xC2
#define SA_CAMERA           0xC3
#define SA_DATE             0xC4
#define SA_TEMPERATURE      0xC5
#define SA_GPS              0xC6
#define SA_ACCEL            0xC7
#define SA_GYRO             0xC8
#define SA_ALL              0xC9





#endif

