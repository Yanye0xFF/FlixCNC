/*
 * int_priority.h
 * @brief 中断优先级配置
 * Created on: Apr 4, 2020
 * Author: Yanye
 */

#ifndef _INT_PRIORITY_H_
#define _INT_PRIORITY_H_

/**
 * 中断控制器优先级配置
 * 0x0是最高优先级，0x7F是最低优先级
 */
typedef enum _int_priority {
	// 限位传感器GPIO中断
	LIMITS_PRIORITY = 0,
	// WIFI网卡SPI数据收发
	NETWORK_PRIORITY,
	// WIFI网卡中断线
	NETWORK_NOTIFY_PRIORITY,
	// 主轴控制器I2C中断
	SPINDLE_CONTROLLER_PRIORITY,
	// 执行雕刻任务定时器
	STEPPER_TIMER_PRIORITY,
	// 回原点定时器
	HOMING_TIMER_PRIORITY,
	// DMTIMER7 延时定时器
	DEALY_TIMER_PRIORITY,
	// GUI界面刷新定时器
    GUI_TIMER_PRIORITY,
} INT_PRIORITY;

#endif
