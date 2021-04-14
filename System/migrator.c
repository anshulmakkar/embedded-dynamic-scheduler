#include <FreeRTOS.h>

#include <task.h>
#include <stdio.h>
#include <string.h>
#include <semphr.h>
#include <string.h>

#include "app_config.h"
#include "system.h"
#include "print.h"

xTaskHandle      migrator_task_handle;

int migrator_runtime_update(task_register_cons *trc, Elf32_Ehdr *new_sw)
{
	/* make sure the task is suspended */
	vTaskSuspend(trc->task_handle);

	/* check elf magic */
	if(!check_elf_magic(new_sw))
	{
		vDirectPrintMsg("new software ELF wrong");
		return ERROR_ELF_MAGIC;
	}

	/* allocate memory for new task */
	task_register_cons * new_trc = task_register("simplev1", new_sw);
	/* allocate memory for new softeare */
	if (!task_alloc(new_trc))
	{
		vDirectPrintMsg("Failed to allocate task");
		return ERROR_TASK_ALLOC;
	}
	/* link new software */
	if (!task_link(new_trc))
	{
		vDirectPrintMsg("Failed to link task ");
		return 0;
	}

	new_trc->request_hook = task_find_request_hook(new_trc);
	if (new_trc->request_hook == NULL) {
	    vDirectPrintMsg("could not find checkpoint request hook durint RTU");
	    return ERROR_TA;
	}

	/* start new task */
	if (!task_start(new_trc))
	{
		vDirectPrintMsg("Failed to start task \n");
	}
}

int migrator_task_loop()
{
	task_register_cons *trc;
	while (1)
	{
		vTaskDelay(1000/portTICK_RATE_MS);
		if (( trc = task_find("simple")))
		{
			if (!task_wait_for_checkpoint(trc, cp_req_rtu))
			{
				vDirectPrintMsg("Failed to reach rtu checkpoint");
				return 0;
			}
			Elf32_Ehdr *updated_sw = (Elf32_Ehdr *)&_simple_v1_start;
			vDirectPrintMsg("Starting Runtime Update");
			if (!migrator_runtime_update(trc, updated_sw))
			{
				vDirectPrintMsg("Failed to update the software");
				return 0;
			}
			vDirectPrintMsg("Runtime update successful ");
		}

	}
}
void migrator_task()
{
	migrator_task_loop();
	while (1)
		vTaskSuspend(NULL);
}

void migrator_task_start()
{
	if(xTaskCreate(migrator_task, (const char *)"migrator_task",
			configMINIMAL_STACK_SIZE, NULL,
			PRIOR_FIX_FREQ_PERIODIC,migrator_task_handle) != pdPASS)
	{
		vDirectPrintMsg("Failed to create migrator task");
		return 0;
	}

}
