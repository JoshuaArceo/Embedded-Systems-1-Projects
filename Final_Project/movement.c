#include "open_interface.h"

char data[50];
volatile int short_count = 0;
char add[25];

double move_forward(oi_t *sensor_data, double distance_mm)
{
    sprintf(add, "");
    double sum = 0;


    oi_setWheels(150, 150);

    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance;
        if (sensor_data->bumpLeft)
        {
            sprintf(add, "Hit:Left");
            short_count++;
            oi_play_song(0);
            break;
        }
        if (sensor_data->bumpRight)
        {
            sprintf(add, "Hit:Right");
            short_count++;
            oi_play_song(0);
            break;
        }

        if (sensor_data->cliffFrontLeftSignal > 2600
                && sensor_data->cliffFrontRightSignal > 2600)
        {
            sprintf(add, "Border:Front");
            break;
        }
        if (sensor_data->cliffFrontRightSignal > 2600
                || sensor_data->cliffRightSignal > 2600)
        {
            sprintf(add, "Border:Right");
            break;
        }

        if (sensor_data->cliffFrontLeftSignal > 2600
                || sensor_data->cliffLeftSignal > 2600)
        {
            sprintf(add, "Border:Left");
            break;
        }



        if (sensor_data->cliffFrontLeft && sensor_data->cliffFrontRight)
        {
            sprintf(add, "Cliff:Front");
            break;
        }
        if (sensor_data->cliffFrontLeft || sensor_data->cliffLeft)
        {
            sprintf(add, "Cliff:Left");
            break;
        }
        if (sensor_data->cliffFrontRight || sensor_data->cliffRight)
        {
            sprintf(add, "Cliff:Right");
            break;
        }
    }
    stop_wheels();
    lcd_printf("Left:%d\nFrontLeft:%d\nRight:%d\nFrontRight:%d", sensor_data->cliffLeftSignal, sensor_data->cliffFrontLeftSignal, sensor_data->cliffRightSignal, sensor_data->cliffFrontRightSignal);
    sprintf(data, "%s Moved:Forward:%f", add, sum);
    uart_sendStr(data);
    return sum;
}

void stop_wheels(oi_t *sensor_data)
{
    oi_setWheels(0, 0);
}

//move the cybot backward
double move_backward(oi_t *sensor_data, double distance_mm)
{
    double sum = 0; // keep track of cybot distance

    oi_setWheels(-300, -300); //move backward

    while (sum > -1 * distance_mm)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance; // use -> notation since pointer
        lcd_printf("Distance moved: %.2fmm", sum);
    }

    oi_setWheels(0, 0); //stop
    sprintf(data, "Moved:Backward:%f", sum);
    uart_sendStr(data);
    return sum;
}

//turn right
double turn_right(oi_t *sensor, double degrees)
{
    char data[50];
    oi_setWheels(-100, 100);
    double sum = 0;
    while (sum > degrees * -1)
    {
        oi_update(sensor);
        sum += sensor->angle;
        lcd_printf("Angles(right): %.2f\n", sum);
    }
    sprintf(data, "Turned:Right:%f", sum);
    uart_sendStr(data);
    return sum;
}

//turn left
double turn_left(oi_t *sensor, double degrees)
{
    char data[50];
    oi_setWheels(100, -100);
    double sum = 0;
    while (sum < degrees)
    {
        oi_update(sensor);
        sum += sensor->angle;
        lcd_printf("Angles(left): %.2f\n", sum);
//        sprintf(data, "Angles(left): %.2f/%.2f\n", sum,degrees);
//        cyBot_sendString(data);
    }
    sprintf(data, "Turned:Left:%f", sum);
    uart_sendStr(data);
    return sum;
}

//move the cybot forward while accounting for obstacles
double move_forward_obj(oi_t *sensor_data, double distance_mm)
{
    double sum = 0; // distance member in oi_t struct is type double

    oi_setWheels(300, 300); //move forward

    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        //Check bumpers
        if (sensor_data->bumpLeft)
        {
            oi_setWheels(0, 0);
            //back up 15cm
            //90 degrees right then 25 cm
            //turn left 90, keep going
            move_backward(&sensor_data, 150);
            turn_right(&sensor_data, 90);
            move_forward(&sensor_data, 250);
            turn_left(&sensor_data, 90);
        }
        if (sensor_data->bumpRight)
        {
            oi_setWheels(0, 0);
            //back up 15cm
            //90 degrees left then 25 cm
            //turn right 90, keep going
            move_backward(&sensor_data, 150);
            turn_left(&sensor_data, 90);
            move_forward(&sensor_data, 250);
            turn_right(&sensor_data, 90);
        }
        oi_setWheels(300, 300);
        sum += sensor_data->distance; // use -> notation since pointer
        lcd_printf("Distance moved: %.2fmm", sum);
    }

    oi_setWheels(0, 0); //stop

    return sum;
}
