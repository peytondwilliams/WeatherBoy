#include "sl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {

#define SPEED (200.0)
#define BOT_COORD (40.0)
#define GRAVITY (1200.0)

#define WIDTH (1200)
#define HEIGHT (800)

#define W1_HEIGHT (60)
#define W1_WIDTH (60)

    time_t t;

    srand((unsigned) time(&t));

    slWindow(WIDTH, HEIGHT, "testing", 0);
    slSetFont(slLoadFont("tff/coolvetica rg.ttf"), 24);
    slSetTextAlign(SL_ALIGN_LEFT);

    char curr_time[10];

    int character = slLoadTexture("sprite/guy.png");
    int weather_1 = slLoadTexture("sprite/raindrop.jpg");
    int weather_2 = slLoadTexture("sprite/cloud.png");
    int weather_2_1 = slLoadTexture("sprite/lightning.jpg");

    double x_coord = 200.0;
    double y_coord = BOT_COORD;
    double y_speed = 0;

    typedef struct w1_phys {
        double x_coord;
        double y_coord;
        double y_speed;

    } w_1;

    typedef struct w2_phys {
        double x_coord;
        double timer;
    } w_2;

    w_1 rain[30];
    w_2 cloud[10];

    int rain_size = 0;
    int cloud_size = 0;

    int location = 0;

    int max_rain = 0;

    double timer_rain = 0.0;
    double timer_cloud = 0.0;

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

//        if (d_time >= 10 && d_time < 30) {
//            difficulty = 1;
//        } else if (d_time >= 30 && d_time < 60) {
//            difficulty = 2;
//        } else if (d_time >= 60 && d_time < 100) {
//            difficulty = 3;
//        } else if (d_time >= 100) {
//            difficulty = 4;
//        }

        timer_rain += slGetDeltaTime();

        max_rain = (int) 2 * (log(d_time) + 1);

            if (rain_size < max_rain && timer_rain > 0.5) {
                timer_rain = 0;
                location = (rand() % (WIDTH));
                w_1 temp = {location, HEIGHT - 60, 40.0};
                rain[rain_size] = temp;
                rain_size++;
            }
            //moves rain and checks if too close to player
            for (int i = 0; i < rain_size; i++) {
                rain[i].y_coord -= SPEED * slGetDeltaTime();
                slSprite(weather_1, rain[i].x_coord, rain[i].y_coord, W1_WIDTH, W1_HEIGHT);

                double x_dif = fabs(x_coord - rain[i].x_coord);
                double y_dif = fabs(y_coord - rain[i].y_coord);
                if (40 >= sqrt(x_dif * x_dif + y_dif * y_dif)){
                    over = 1;
                }
            }
            //deletes rain at bottom
            for (int i = 0; i < rain_size; i++) {
                if (rain[i].y_coord <= 10.0) {
                    for (int j = i; j < rain_size; j++) {
                        rain[j] = rain[j+1];
                    }
                    rain_size--;
                }
            }

        timer_cloud += slGetDeltaTime();

        if (timer_cloud >= 1.0 && cloud_size < 10) {
            timer_cloud = 0.0;
            location = (rand() % (WIDTH));
            w_2 temp = {location, 0.0};
            cloud[cloud_size] = temp;
            cloud_size++;

        }


        for (int i = 0; i < cloud_size; i++) {
            cloud[i].timer += slGetDeltaTime();
            slSprite(weather_2, cloud[i].x_coord, HEIGHT - 60, 100, 40);
                if (cloud[i].timer >= 1.5) {
                    slSprite(weather_2_1, cloud[i].x_coord, HEIGHT / 2 - 40, 40, HEIGHT);
                    double x_dif = fabs(x_coord - cloud[i].x_coord);
                    if (x_dif <= 35) {
                        over = 1;
                        break;
                    }
                }

        }
        for (int i = 0; i < cloud_size; i++) {
            if (cloud[i].timer >= 2.3) {
                for (int j = i; j < cloud_size; j++) {
                    cloud[j] = cloud[j+1];
                }
                cloud_size--;
            }
        }


        slRender();

        if(over != 0) {
            break;
        }
    }
    while (!slShouldClose() &&!slGetKey(SL_KEY_ESCAPE)) {
        slSetForeColor(1.0, 1.0, 1.0, 1.0);
        slSetTextAlign(SL_ALIGN_CENTER);
        slText(WIDTH / 2, HEIGHT / 2, "Game Over! Press esc to leave");

        slRender();
    }

return 0;
}
