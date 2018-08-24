/*
 * main.cpp

 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */
#include "mainpp.h"
#include "ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "ros/time.h"

void TurnLED(const std_msgs::Bool &led){
	if(led.data){
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	}
}

ros::NodeHandle nh;

std_msgs::String str_msg;
std_msgs::Bool status_msg;
ros::Time last;
ros::Publisher chatter("chatter", &str_msg);
ros::Publisher status("status", &status_msg);
ros::Subscriber<std_msgs::Bool> turn_led("turn_led", TurnLED );
char hello[] = "Hello world!";



void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	nh.getHardware()->reset_rbuf();
}

void setup(void)
{
	nh.initNode();
	nh.advertise(chatter);
	nh.subscribe(turn_led);
	nh.advertise(status);
	last = nh.now();
}

void loop(void)
{
	if(nh.now().toSec() - last.toSec() >= 1){
		status_msg.data = true;
		status.publish(&status_msg);
		str_msg.data = hello;
		chatter.publish(&str_msg);
		last = nh.now();
	}

	nh.spinOnce();
	HAL_Delay(1);
}

