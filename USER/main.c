#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"
#include "los_swtmr.h"
#include "time.h"
#include "los_sys.h"
#include "usart.h"
UINT32 g_TestTskHandle;
VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 count=0;
	while(1)
	{
		count++;
		//printf("this is task 1,count is %d\r\n",count);//串口打印计数
		LED0=!LED0;
		uwRet = LOS_TaskDelay(200);//操作系统延时
		if(uwRet !=LOS_OK)
			return;
	}
}

UINT32 creat_task1(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;//任务优先级
	task_init_param.pcName = "task1";//任务名
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;//指定任务入口函数
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//设置任务堆栈大小
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//调用任务创建函数
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}
/*
 *串口数据处理任务
*/
VOID task2(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 count=0;
	while(1)
	{
		count++;
		//printf("This is task 2,count is %d\r\n",count);
		if(USART_RX_STA)
		{
			USART_printf(USART1,"%s\r\n",USART_RX_BUF);
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
			USART_RX_STA=0;
		}
		if(USART2_RX_STA)
		{
			USART_printf(USART2,"%s\r\n",USART2_RX_BUF);
			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));
			USART2_RX_STA=0;
		}
		uwRet = LOS_TaskDelay(10);
		if(uwRet !=LOS_OK)
		return;
	}
}

UINT32 creat_task2(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "task2";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
	if(uwRet !=LOS_OK)
	{
	return uwRet;
	}
	return uwRet;
}

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	delay_init();
	LED_Init();
	uart_init(9600);
	uart2_init(9600);
	LOS_HwiCreate(37,0,0,USART1_IRQHandler,NULL);
	LOS_HwiCreate(38,0,0,USART2_IRQHandler,NULL);
	USART_printf(USART1,"\r\n");
	USART_printf(USART1,"hello liteos\r\n");
	//Example_TskCaseEntry();
	

	LED0=1;	
	delay_ms(500);
	LED0=0;	
	delay_ms(500);
	LED0=1;	
	delay_ms(500);

//	USART_printf(USART1,"hello\r\n");
	
	
	//Timer_example();


	
	LED0=0;	
	delay_ms(500);
	
	uwRet = creat_task1();
	if(uwRet !=LOS_OK)
	{
	return uwRet;
	}
	uwRet = creat_task2();
	if(uwRet !=LOS_OK)
	{
	return uwRet;
	}
	return LOS_OK;
}

