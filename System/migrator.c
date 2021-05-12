#include <FreeRTOS.h>

#include <task.h>
#include <stdio.h>
#include <string.h>
#include <semphr.h>
#include <string.h>

#include "system.h"
#include "migrator.h"

xTaskHandle      migrator_task_handle;
#define RTU_DATA_SECTION_NAME ".rtu_data"
extern task_register_cons * simplec;

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
	if (!task_start_v1(new_trc))
    {
        vDirectPrintMsg("Failed to start task \n");
    }
#if 0
	new_trc->request_hook = task_find_request_hook(new_trc);
	if (new_trc->request_hook == NULL) {
	    vDirectPrintMsg("could not find checkpoint request hook durint RTU");
	    return ERROR_TASK_ALLOC;
	}

	/* find rtu_data section */
	Elf32_Half old_rtu_ndx = find_section_index(RTU_DATA_SECTION_NAME, trc->elfh);
	Elf32_Half new_rtu_ndx = find_section_index(RTU_DATA_SECTION_NAME, new_trc->elfh);
	Elf32_Shdr *old_rtu = find_section(RTU_DATA_SECTION_NAME, trc->elfh);
	Elf32_Shdr *new_rtu = find_section(RTU_DATA_SECTION_NAME, new_trc->elfh);

	if (old_rtu_ndx == 0 || new_rtu_ndx == 0 ||
	    old_rtu == NULL || new_rtu == NULL)
	{
	    vDirectPrintMsg("faile dto find the rtu data section");
	    return ERROR_RTU_SECTION_NOT_FOUND;
	}

	void *old_rtu_mem = task_get_section_address(trc, old_rtu_ndx);
	void *new_rtu_mem = task_get_section_address(new_trc, new_rtu_ndx);
	if (old_rtu_mem == NULL || new_rtu_mem == NULL)
    {
        vDirectPrintMsg("faile dto find the rtu data section address");
        return ERROR_RTU_SECTION_NOT_FOUND;
    }

	/*
     * Copy the .rtu_data section from the old to the new
     * software. We should here somewhere run the transformation
     * function too.
     */
	memcpy((void *)new_rtu_mem, (void *)old_rtu_mem, old_rtu->sh_size);


	/* start new task */
	if (!task_start(new_trc))
	{
		vDirectPrintMsg("Failed to start task \n");
	}
	vTaskDelete(trc->task_handle);
	task_free(trc);
#endif
	return ERROR_SUCCESS;
}

int migrator_task_loop()
{
	task_register_cons *trc = simplec;
	vDirectPrintMsg("migrator_task_loop\n");
	while (1)
	{
		vTaskDelay(1000/portTICK_RATE_MS);
		//if (( trc = task_find("simple")))
		if (trc != NULL)
		{
			if (!task_wait_for_checkpoint(trc, cp_req_rtu))
			{
				vDirectPrintMsg("Failed to reach rtu checkpoint");
				return 0;
			}
			Elf32_Ehdr *updated_sw = (Elf32_Ehdr *)simple_elf_v1;
			vDirectPrintMsg("Starting Runtime Update");
			if (!migrator_runtime_update(trc, updated_sw))
			{
				vDirectPrintMsg("Failed to update the software");
				return 0;
			}
			vDirectPrintMsg("Runtime update successful");
		}
		else
		    vDirectPrintMsg("migrator_loop not find simple\n");

	}
}
void migrator_task()
{
    vDirectPrintMsg("migrator_task\n");
	migrator_task_loop();
	while (1)
		vTaskSuspend(NULL);
}

int migrator_task_start()
{
    vDirectPrintMsg("migrator_task start\n");
	if(xTaskCreate(migrator_task, (const char *)"migrator_task",
			configMINIMAL_STACK_SIZE, NULL,
			2, migrator_task_handle) != pdPASS)
	{
		vDirectPrintMsg("Failed to create migrator task");
		return 0;
	}
	return 1;

}
