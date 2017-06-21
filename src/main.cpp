#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

#define MAX_CTE 4.0
#define MAX_SPEED 60.0

int main()
{
  uWS::Hub h;

  // TODO: Initialize the PID controllers

  PID steerPID;
  steerPID.Init(0.2, 0.001, 1.0);

  h.onMessage([&steerPID](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_signal;
          double throttle_signal;

          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */

          double speed_factor = (MAX_SPEED - fabs(speed))/MAX_SPEED;
          std::cout << "speed factor = " << speed_factor << std::endl;

          double cte_factor = (MAX_CTE - fabs(cte))/MAX_CTE;
          std::cout << "cte factor = " << cte_factor << std::endl;

          double norm_cte = cte_factor/(cte_factor + speed_factor);
          double norm_speed = 1.0 - norm_cte;

          double kp_factor = 0.7*(1.0 - norm_cte) + 0.3*(norm_speed);
          double kd_factor = 0.7*(1.0 - norm_cte) + 0.3*(1.0 - norm_speed);

          double norm_kp_factor = kp_factor/(kp_factor + kd_factor);
          double norm_kd_factor = 1.0 - norm_kp_factor;

          std::cout << "kp_factor = " << norm_kp_factor << std::endl;
          std::cout << "kd_factor = " << norm_kd_factor << std::endl;

          throttle_signal = 1.0*speed_factor*cte_factor;

          // PID steer controller
          steerPID.UpdateError(cte);
          steer_signal = steerPID.TotalError(norm_kp_factor);

          // normalize steer control signal
          if(steer_signal > 1.0){
            steer_signal = 1.0;
          } else if(steer_signal < -1.0){
            steer_signal = -1.0;
          }

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_signal << std::endl;
          std::cout << "Throttle = " << throttle_signal << std::endl;
          std::cout << "Speed (mph) = " << speed << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_signal;
          msgJson["throttle"] = throttle_signal;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
