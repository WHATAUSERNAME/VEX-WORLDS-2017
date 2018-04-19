#pragma config(Sensor, dgtl1,  LE,             sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  RE,             sensorQuadEncoder)
#pragma config(Motor,  port1,           BR,            tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           FR,            tmotorVex393_MC29, openLoop, encoderPort, dgtl1)
#pragma config(Motor,  port3,           LL,            tmotorVex393_MC29, openLoop, encoderPort, dgtl1)
#pragma config(Motor,  port4,           TL,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           MG,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           I,             tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           RL,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           MB,            tmotorVex393_MC29, openLoop, encoderPort, dgtl3)
#pragma config(Motor,  port9,           FL,            tmotorVex393_MC29, openLoop, encoderPort, dgtl3)
#pragma config(Motor,  port10,          BL,            tmotorVex393_HBridge, openLoop)

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

// Include Jason's Gyro Filtering/ Motion Planning Libraries
//#include "NERD_Gyro.c"
//#include "motionplanner.c"

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void pre_auton()
{

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/

task autonomous()
{

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
	// define variables used to extract joystick values
	long drive_l_motor;
	long drive_r_motor;

	while (true)
	{
		/*---------------------------------------------------------------------------------*/
		/*		                          Drive                                              */
		/*		                          Code                                               */
		/*---------------------------------------------------------------------------------*/

		// get js values
		drive_l_motor = vexRT[Ch4] + vexRT[Ch3];
		drive_r_motor = vexRT[Ch4] - vexRT[Ch3];

		// deadband
		if(abs(drive_l_motor) < 15 )
			drive_l_motor = 0;
		if(abs(drive_r_motor) < 15 )
			drive_r_motor = 0;

		// voltage cap to alleviate stalling outof motors
		if(drive_l_motor > 85)
			drive_l_motor = 85;
		if(drive_l_motor < -85)
			drive_l_motor = -85;
		if(drive_r_motor > 85)
			drive_r_motor = 85;
		if(drive_r_motor < -85)
			drive_r_motor = -85;

		// send to motors
		motor[FL] = drive_l_motor;
		motor[BL] = drive_l_motor;
		motor[FR] = drive_r_motor;
		motor[BR] = drive_r_motor;

		wait1Msec(20);

		/*---------------------------------------------------------------------------------*/
		/*		                          Mogo                                               */
		/*  	                          Code                                               */
		/*---------------------------------------------------------------------------------*/

		if(vexRT [Btn7U] == 1)
		{
			motor(MG) = -127;

		}
		else if (vexRT [Btn7L] == 1)
		{
			motor(MG) = 127;

		}
		else if (vexRT [Btn7D] == 1)
		{
			motor(MG) = -50;

		}
		else
		{
			motor(MG) = 0;
		}


		/*---------------------------------------------------------------------------------*/
		/*		                          Lift                                               */
		/*		                          Code                                               */
		/*---------------------------------------------------------------------------------*/

		if(vexRT[Btn5U] == 1)
		{
			motor(LL) = 127;
			motor(RL) = 127;
			motor(TL) = 127;
		}
		else if(vexRT[Btn5D] == 1)
		{
			motor(LL) = -127;
			motor(RL) = -127;
			motor(TL) = -127;
		}
		else
		{
			motor(LL) = 0;
			motor(RL) = 0;
			motor(TL) = 0;

		}

		/*---------------------------------------------------------------------------------*/
		/*		                          Intake                                             */
		/*		                          Code                                               */
		/*---------------------------------------------------------------------------------*/

		/*Constantly Run Intake
		if(vexRT[Btn8L] == 1)
		{
		motor(I) = -127;
		}
		else if(vexRT[Btn8U] == 1)
		{
		motor(I) = 127;

		}
		else
		{
		motor(I) = -30;
		}

		/*---------------------------------------------------------------------------------*/
		/*		                      Mini 4 Bar                                             */
		/*		                          Code                                               */
		/*---------------------------------------------------------------------------------*/

		if(vexRT[Btn6U] == 1)
		{
			motor(MB) = -127;
		}
		else if(vexRT[Btn6D] == 1)
		{
			motor(MB) = 127;

		}
		else
		{
			motor(MB) = 0;
		}

	}
}
