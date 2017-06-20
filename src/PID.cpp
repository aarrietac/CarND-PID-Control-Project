#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
  this->error = 0.0;
  this->prev_error = 0.0;

  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;

  this->p_error = 0.0;
  this->i_error = 0.0;
  this->d_error = 0.0;
}

void PID::UpdateError(double cte) {
  this->error = cte;
  this->p_error = this->error;
  this->i_error += this->error;
  this->d_error = (this->error - this->prev_error);
  this->prev_error = this->error;
}

double PID::TotalError() {
  double ctrl_signal = -(this->Kp*this->p_error + this->Ki*this->i_error + this->Kd*this->d_error);
  return  ctrl_signal;
}
