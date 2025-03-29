#include "sensors.h"

int temp_values[SAMPLES] = {-10, 23, -12, 5, -7, 60, 18, -35, 49, -22, 33, 0, 45, -27, 11, 58, -2, 40, -14, 7, -42, 21, -19, 2, -50, 34, -46, 56, -31, 13, -5, 60, -38, 17, -25, 9, -1, 41, -29, 50, -16, 55, -9, 48, -6, 37, -47, 29, -44, 27};
int hum_values[SAMPLES] = {34, 89, 7, 41, 56, 18, 92, 25, 67, 11, 73, 48, 3, 99, 82, 31, 100, 45, 60, 22, 9, 53, 36, 77, 68, 0, 15, 97, 79, 27, 90, 13, 58, 47, 100, 71, 5, 85, 30, 61, 95, 12, 33, 20, 63, 54, 4, 87, 26, 50};
int co2_values[SAMPLES] = {18457, 1203, 7654, 19876, 5234, 10987, 15789, 2345, 8765, 4321, 14987, 6789, 14235, 9876, 12004, 18765, 5432, 17000, 765, 13456, 1987, 16000, 987, 400, 15234, 5678, 13579, 18999, 4500, 7999, 19999, 17789, 10234, 6000, 8450, 14321, 20000, 15999, 11234, 12876, 17543, 14000, 7800, 18987, 9400, 15678, 4900, 16345, 19345, 8888};

int temp_history[HISTORY] = {__INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, };
int hum_history[HISTORY] = {__INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, };
int co2_history[HISTORY] = {__INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, __INT_MAX__, };

int v_temp = 0, v_hum = 0, v_co2 = 0;
int h_temp = 0, h_hum = 0, h_co2 = 0;

int simulate_temp_sensor(char action, int history[]){
    switch (action) {
        case 'P':
        case 'A':
            int value = temp_values[v_temp];
            v_temp = (v_temp+1)%(SAMPLES);


            temp_history[h_temp] = value;
            h_temp = (h_temp+1)%(HISTORY);
            return value;

        case 'L':
            for(int i=h_temp, j=0;j<HISTORY;j++){
                if(i==0)
                    i = HISTORY-1;
                else
                    i--;

                history[j] = temp_history[i];  
            }
            return 0;

        case 'R':
            for(int i=0;i<HISTORY;i++) {
                temp_history[i] = __INT_MAX__;
            }
            h_temp = 0;
            return 0;
        
        default:
            return -1;
    }
    return -1;
}

int simulate_hum_sensor(char action, int history[]){
    switch (action) {
        case 'P':
        case 'A':
            int value = hum_values[v_hum];
            v_hum = (v_hum+1)%(SAMPLES);


            hum_history[h_hum] = value;
            h_hum = (h_hum+1)%(HISTORY);
            return value;

        case 'L':
            for(int i=h_hum, j=0;j<HISTORY;j++){
                if(i==0)
                    i = HISTORY-1;
                else
                    i--;

                history[j] = hum_history[i];  
            }
            return 0;

        case 'R':
            for(int i=0;i<HISTORY;i++) {
                hum_history[i] = __INT_MAX__;
            }
            h_hum = 0;
            return 0;
        
        default:
            return -1;
    }
    return -1;
}

int simulate_co2_sensor(char action, int history[]){
    switch (action) {
        case 'P':
        case 'A':
            int value = co2_values[v_co2];
            v_co2 = (v_co2+1)%(SAMPLES);


            co2_history[h_co2] = value;
            h_co2 = (h_co2+1)%(HISTORY);
            return value;

        case 'L':
            for(int i=h_co2, j=0;j<HISTORY;j++){
                if(i==0)
                    i = HISTORY-1;
                else
                    i--;

                history[j] = co2_history[i];  
            }
            return 0;

        case 'R':
            for(int i=0;i<HISTORY;i++) {
                co2_history[i] = __INT_MAX__;
            }
            h_co2 = 0;
            return 0;
        
        default:
            return -1;
    }
    return -1;
}