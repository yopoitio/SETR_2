#include "sensors.h"

int *temp_values = {-10, 23, -12, 5, -7, 60, 18, -35, 49, -22, 33, 0, 45, -27, 11, 58, -3, 40, -14, 7, -42, 21, -19, 2, -50, 34, -46, 56, -31, 13, -5, 60, -38, 17, -25, 9, -1, 41, -29, 50, -16, 55, -9, 48, -6, 37, -47, 29, -44, 27};
int *hum_values = {34, 89, 7, 41, 56, 18, 92, 25, 67, 11, 73, 48, 3, 99, 82, 31, 100, 45, 60, 22, 9, 53, 36, 77, 68, 0, 15, 97, 79, 27, 90, 13, 58, 47, 100, 71, 5, 85, 30, 61, 95, 12, 33, 20, 63, 54, 4, 87, 26, 50};
int *co2_values = {18457, 1203, 7654, 19876, 5234, 10987, 15789, 2345, 8765, 4321, 14987, 6789, 14235, 9876, 12004, 18765, 5432, 17000, 765, 13456, 1987, 16000, 987, 400, 15234, 5678, 13579, 18999, 4500, 7999, 19999, 17789, 10234, 6000, 8450, 14321, 20000, 15999, 11234, 12876, 17543, 14000, 7800, 18987, 9400, 15678, 4900, 16345, 19345, 8888};

int temp_history[20] = {0};
int hum_history[20] = {0};
int co2_history[20] = {0};

int i_temp = 0, i_hum = 0, i_co = 0;
int j_temp = 0, j_hum = 0, j_co = 0;

int simulate_temp_sensor(char action, int history[]){
    switch (action) {
        case 'P' || 'A':
            int value = temp_values[i_temp];
            i_temp = ++i_temp%49;

            temp_history[j_temp] = value;
            j_temp = ++j_temp%19;

            return value;
            break;

        case 'L':
            for(int i=j_temp, j=0;j<20;--i%19,j++){
                history[j] = temp_history[j_temp];
            }
            return 0;
            break;

        case 'H':
            for(int i=0;i<20;i++) {
                temp_history[i] = 0;
                j_temp = 0;
            }
            return 0;
            break;
    }
}

int simulate_hum_sensor(char action){
    switch (action) {
        case 'P' || 'A':
            int value = hum_values[i_hum];
            i_hum = ++i_hum%49;
            return value;
            break;

        case 'L':

            break;

        case 'H':
            for(int i=0;i<20;i++) {
                hum_history[i] = 0;
                j_hum = 0;
            }
            return 0;
            break;
    }
}

int simulate_co2_sensor(char action){
    switch (action) {
        case 'P' || 'A':
            int value = co2_values[i_co];
            i_co = ++i_co%49;
            return value;
            break;

        case 'L':

            break;

        case 'H':
            for(int i=0;i<20;i++) {
                co2_history[i] = 0;
                j_co = 0;
            }
            return 0;
            break;
    }
}