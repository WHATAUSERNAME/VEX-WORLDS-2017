/* -----------------------------------------------------------------------------
                                  MIT License

                        Copyright (c) 2018 Jason McKinney

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

--------------------------------------------------------------------------------
	NERD_PID.c

	Created:  2016-06-30

	Minor Revisions:
	-	v1.0.0  Initial Release

--------------------------------------------------------------------------------
	The author asks that proper attribution be given for this software should the
	source be unavailable (for example, if compiled into a binary/used on a robot).

	The author can be contacted via email at jason_at_jmmckinney_dot_net
	or on the VEX Forum as jmmckinney.
-------------------------------------------------------------------------------- */


#ifndef NERD_PID
#define NERD_PID

/**
 * PID controller data structure
 */

typedef struct {
	float Kp;
	float Ki;
	float Kd;
	float inner_integral_band;
	float outer_integral_band;
	float sigma;
	float last_value;
	unsigned long last_time;
	float last_set_point;
} PID;

/**
 * initialize pid structure, set parameters
 *
 * @param pid instance of PID structure
 * @param Kp  PID Kp constant
 * @param Ki  PID Ki constant
 * @param Kd  PID Kd constant
 * @param inner_integral_band  inner bound of PID I summing cutoff
 * @param outer_integral_band  outer bound of PID I summing cutoff
 */
void pid_init (PID pid, float Kp, float Ki, float Kd, float inner_integral_band, float outer_integral_band) {
	pid.Kp = Kp;
	pid.Ki = Ki;
	pid.Kd = Kd;
	pid.inner_integral_band = inner_integral_band;
	pid.outer_integral_band = outer_integral_band;
	pid.sigma = 0;
	pid.last_value = 0;
	pid.last_time = nPgmTime;
}

/**
 * initialize pid structure, set parameters based on another PID structure
 *
 * @param pid  instance of PID structure
 * @param to_copy  PID instance to copy settings from
 */
void pid_init_copy (PID pid, PID to_copy) {
	pid.Kp = to_copy.Ki;
	pid.Ki = to_copy.Ki;
	pid.Kd = to_copy.Kd;
	pid.inner_integral_band = to_copy.inner_integral_band;
	pid.outer_integral_band = to_copy.outer_integral_band;
	pid.sigma = 0;
	pid.last_value = 0;
	pid.last_time = nPgmTime;
}

/**
 * calculate pid output
 *
 * @param pid  instance of PID structure
 * @param set_point  set point of PID controller
 * @param sensor_read  sensor/feedback value
 *
 * @return  output value of the control loop
 */
float
pid_calculate (PID pid, float set_point, float sensor_read) {
	float delta_time = (nPgmTime - pid.last_time)/1000.0;
	pid.last_time = nPgmTime;

	float sensor_rate = 0;
	if(delta_time > 0)
		sensor_rate = (sensor_read - pid.last_value) / delta_time;
	pid.last_value = sensor_read;

	float error = set_point - sensor_read;

	if(fabs(error) > pid.inner_integral_band && fabs(error) < pid.outer_integral_band)
		pid.sigma += error * delta_time;

	if (fabs (error) > pid.outer_integral_band)
		pid.sigma = 0;

	float output = error * pid.Kp
					+ pid.sigma * pid.Ki
					- sensor_rate * pid.Kd;

	return output;
}

/**
 * calculate PID output while velocity control is active. The velocity set point will be subtracted from the time derivative of the error
 *
 * @param pid  the PID controller to use for the calculation
 * @param set_point  the set point of the system
 * @param sensor_read  the value of the feedback sensor in the system
 * @param velocity_set  the velocity set point of the system
 *
 * @return  the output value of the control loop
 */
float
pid_calculate_with_velocity_set (PID pid, float set_point, float sensor_read, float velocity_set) {
	float delta_time = (nPgmTime - pid.last_time)/1000.0;
	pid.last_time = nPgmTime;

	float sensor_rate = 0;
	if(delta_time > 0)
		sensor_rate = (sensor_read - pid.last_value) / delta_time + velocity_set;
	pid.last_value = sensor_read;

	float error = set_point - sensor_read;

	if(fabs(error) > pid.inner_integral_band && fabs(error) < pid.outer_integral_band)
		pid.sigma += error * delta_time;

	if (fabs (error) > pid.outer_integral_band)
		pid.sigma = 0;

	float output = error * pid.Kp
					+ pid.sigma * pid.Ki
					- sensor_rate * pid.Kd;

	return output;
}

/**
 * calculate PID output for velocity control using feedforward instead of an error calculation, but still allowing for I and D components.
 *
 * @param pid  the PID controller to use for the calculation
 * @param set_point  the set point of the system
 * @param sensor_read  the value of the feedback sensor in the system
 *
 * @return  the output value of the control loop
 */
float
pid_calculate_velocity (PID pid, int set_point, int sensor_read) {
	float delta_time = (nPgmTime - pid.last_time)/1000.0;
	pid.last_time = nPgmTime;

	float sensor_rate = 0;
	if(delta_time > 0)
		sensor_rate = (sensor_read - pid.last_value) / delta_time;
	pid.last_value = sensor_read;

	float error = set_point - sensor_read;

	if(fabs(error) > pid.inner_integral_band && fabs(error) < pid.outer_integral_band)
		pid.sigma += error * delta_time;

	if (fabs (error) > pid.outer_integral_band)
		pid.sigma = 0;

	float output = set_point * pid.Kp
					+ pid.sigma * pid.Ki
					- sensor_rate * pid.Kd;

	return output;
}
#endif
