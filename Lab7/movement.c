#include "open_interface.h"
#include "uart-interrupt.h"
#include "Timer.h"

char data[50];
//move the cybot forward 1 meter
double move_forward(oi_t *sensor_data, double distance_mm)
{
    double sum = 0; // distance member in oi_t struct is type double

    oi_setWheels(300, 300); //drive the cybot forward

    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance; // use -> notation since pointer
        lcd_printf("Distance moved: %.2fmm", sum);
    }

    oi_setWheels(0, 0); //stop

    return sum;
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
//        sprintf(data, "Angles(right): %.2f/%.2f\n", sum, degrees);
//        cyBot_sendString(data);
    }
    oi_setWheels(0,0);
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
    oi_setWheels(0,0);
    return sum;
}

//move the cybot forward while accounting for obstacles
int move_forward_obj(oi_t *sensor_data, double distance_mm)
{

    double sum = 0; // distance member in oi_t struct is type double

    oi_setWheels(150, 150); //move forward

    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        //Check bumpers
        if (sensor_data->bumpLeft)
        {
            oi_setWheels(0, 0);
            sprintf(data, "bumped with sum: %f",sum);
            cyBot_sendString(data);
            //back up 15cm
            //90 degrees right then 25 cm
            //turn left 90, keep going
          return -1;
        }
        if (sensor_data->bumpRight)
        {
            oi_setWheels(0, 0);
            sprintf(data, "bumped with sum: %f",sum);
             cyBot_sendString(data);
            //back up 15cm
            //90 degrees left then 25 cm
            //turn right 90, keep going
           return 1;
        }
        sum += sensor_data->distance; // use -> notation since pointer
        lcd_printf("Distance moved: %.2fmm", sum);
    }

    oi_setWheels(0, 0); //stop
    sprintf(data, "returning sum: %f", sum);
    cyBot_sendString(data);
    return 0;
}

void bump_left(oi_t *sensor_data){
    move_backward(&sensor_data, 150);
              turn_right(&sensor_data, 90);
              move_forward(&sensor_data, 250);
              turn_left(&sensor_data, 100);
              move_forward(&sensor_data, 150);
}

void bump_right(oi_t *sensor_data){
                move_backward(&sensor_data, 150);
                turn_left(&sensor_data, 90);
                move_forward(&sensor_data, 250);
                turn_right(&sensor_data, 100);
                move_forward(&sensor_data, 150);
}

void move_forward_manual()
{

    oi_setWheels(300, 300); //drive the cybot forward

}


void move_backward_manual()
{

    oi_setWheels(-300, -300); //drive the cybot forward

}


//turn right
void turn_right_manual()
{
    oi_setWheels(-100, 100);
}


void turn_left_manual()
{
    oi_setWheels(100, -100);
}

void stop_manual(){
    oi_setWheels(0,0);
}
