#include "sl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {

#define SPEED (200.0)
#define BOT_COORD (40.0)
#define GRAVITY (1200.0)

    slWindow(920, 680, "testing", 0);
    slSetFont(slLoadFont("tff/coolvetica rg.ttf"), 24);
    slSetTextAlign(SL_ALIGN_LEFT);

    char curr_time[10];

    int character = slLoadTexture("sprite/guy.png");
    int weather_1 = slLoadTexture("sprite/raindrop.jpg");
    //int weather_2 = slLoadTexture();
    //int weather_3 = slLoadTexture();

    double x_coord = 200.0;
    double y_coord = BOT_COORD;
    double y_speed = 0;

    typedef struct w1_phys {
        double x_coord;
        double y_coord;
        double y_speed;
    } w_1;

    w_1 rain[20];
    int rain_size = 0;

    int location = 0;

    int difficulty = 0;

    double timer_rain = 0.0;

    int over = 0;

    while (!slShouldClose()) {

        if (slGetKey('D')) {
            x_coord += SPEED * slGetDeltaTime();
        }
        if (slGetKey('A')) {
            x_coord -= SPEED * slGetDeltaTime();
        }
        if ((slGetKey('W') != 0) && (y_coord == 40.0)) {
            y_speed = 600;
        }

        if (y_speed != 0.0) {
            y_speed -= GRAVITY * slGetDeltaTime();
            y_coord += y_speed * slGetDeltaTime();
            if (y_coord <= 40.0) {
                y_coord = 40.0;
                y_speed = 0.0;
            }
        }

        slSetForeColor(1.0, 1.0, 1.0, 1.0);
        slSprite(character, x_coord, y_coord, 40.0, 40.0);

        double d_time = slGetTime();
        sprintf(curr_time, "Time: %.1f", d_time);
        slSetForeColor(1.0, 1.0, 1.0, 1.0);
        slText(10, 300, curr_time);

        if (d_time >= 10 && d_time < 30) {
            difficulty = 1;
        } else if (d_time >= 30 && d_time < 60) {
            difficulty = 2;
        } else if (d_time >= 60 && d_time < 100) {
            difficulty = 3;
        } else if (d_time >= 100) {
            difficulty = 4;
        }

        timer_rain += slGetDeltaTime();

        switch (difficulty) {

            case 0:
                if (rain_size < 3 && timer_rain > 0.5) {
                    timer_rain = 0;
                    location = (rand() % (600));
                    w_1 temp = {location, 400, 40.0, 40.0};
                    rain[rain_size] = temp;
                    rain_size++;
                }
                for (int i = 0; i < rain_size; i++) {
                    rain[i].y_coord -= SPEED * slGetDeltaTime();
                    slSprite(weather_1, rain[i].x_coord, rain[i].y_coord, 40.0, 40.0);
                    if (rain[i].y_coord <= 10.0) {
                        for (int j = 0; j < rain_size; j++) {
                            rain[i] = rain[i+1];
                        }
                        rain_size--;
                    }
                    double x_dif = fabs(x_coord - rain[i].x_coord);
                    double y_dif = fabs(y_coord - rain[i].y_coord);
                    if (40 >= sqrt(x_dif * x_dif + y_dif * y_dif)){
                        over = 1;
                    }
                }

                break;

            case 1:

                break;

            case 2:

                break;

            case 3:

                break;

            case 4:

                break;

        }

        slRender();

        if(over != 0) {
            break;
        }
    }
    while (!slShouldClose() &&!slGetKey(SL_KEY_ESCAPE)) {
        slSetForeColor(1.0, 1.0, 1.0, 1.0);
        slText(300, 300, "Game Over! Press esc to leave.");

        slRender();
    }

return 0;
}
