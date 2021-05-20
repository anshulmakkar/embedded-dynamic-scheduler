#include <FreeRTOS.h>

#include <task.h>
#include <stdio.h>
#include <string.h>
#include <semphr.h>
#include <string.h>

#include "task_manager.h"
#include "system.h"

//task_register_tree task_register_tree_var =
//RB_INITIALIZER(task_register_tree);
//RB_GENERATE(task_register_tree_t, task_register_cons_t, tasks, task_register_cons_cmp)
//
extern task_register_cons * simplec;
int division(int dividend, int divisor)
{
    int quotient = 1;
    //int remainder = 0;

    int neg = 1;
    if ((dividend>0 &&divisor<0)||(dividend<0 && divisor>0))
        neg = -1;

    // Convert to positive
    unsigned int tempdividend = (dividend < 0) ? -dividend : dividend;
    unsigned int tempdivisor = (divisor < 0) ? -divisor : divisor;

    if (tempdivisor == tempdividend) {
    	//remainder = 0;
        return 1*neg;
    }
    else if (tempdividend < tempdivisor) {
        //if (dividend < 0)
        	//remainder = tempdividend*neg;
        //else
        //	remainder = tempdividend;
        return 0;
    }
    while (tempdivisor<<1 <= tempdividend)
    {
        tempdivisor = tempdivisor << 1;
        quotient = quotient << 1;
    }

    // Call division recursively
    if(dividend < 0)
        quotient = quotient*neg + division(-(tempdividend-tempdivisor), divisor);
    else
        quotient = quotient*neg + division(tempdividend-tempdivisor, divisor);
     return quotient;
 }

/*int strcmp(const char *a,const char *b){
  if (! (*a | *b)) return 0;
  return (*a!=*b) ? *a-*b : strcmp(++a,++b);
}*/

int check_elf_magic(Elf32_Ehdr *hdr)
{
	vDirectPrintMsg("check_elf_magic \n");
	if (hdr->e_ident[0] != ELFMAG0)
		return 0;

	if (hdr->e_ident[1] != ELFMAG1)
		return 0;

	if (hdr->e_ident[2] != ELFMAG2)
		return 0;

	if (hdr->e_ident[3] != ELFMAG3)
		return 0;

	return 1;
}
char *get_shstr(Elf32_Ehdr *elf_h, Elf32_Shdr *shstr_hdr, Elf32_Word index)
{
	char *data = (char *)((u_int32_t)elf_h + shstr_hdr->sh_offset);
	return data + index;
}

Elf32_Half find_section_index(char *name, Elf32_Ehdr *elf_h)
{
	int i;
    Elf32_Shdr *sect_hdr = (Elf32_Shdr *)((u_int32_t)elf_h + elf_h->e_shoff);
    Elf32_Shdr *shstr_hdr = &sect_hdr[elf_h->e_shstrndx];
    for (i = 0; i < elf_h->e_shnum; i++)
    {
    	char *a = get_shstr(elf_h, shstr_hdr, sect_hdr[i].sh_name);
    	if (strcmp(a, name) == 0)
    		return i;
    }
    return 0;
}

Elf32_Shdr *find_section(char *name, Elf32_Ehdr *elf_h)
{
    int index = find_section_index(name, elf_h);
    Elf32_Shdr  *sect_hdr = (Elf32_Shdr *)((u_int32_t)elf_h + elf_h->e_shoff);

    /* section header with index 0 is the NULL sectin , and not used */
    return index == 0 ? (Elf32_Shdr *)NULL : &sect_hdr[index];
}

Elf32_Sym *find_symbol(char *name, Elf32_Ehdr *elf_h)
{
	u_int32_t i;
	char *tname;
	Elf32_Shdr *symtab_sect = NULL;
	Elf32_Shdr *strtab_sect = NULL;
	Elf32_Sym *syms = NULL;
	u_int32_t n = 0;


	symtab_sect = find_section(".symtab", elf_h);
	strtab_sect = find_section(".strtab", elf_h);

	if (symtab_sect == NULL)
	{
		vDirectPrintMsg("symtab section not found");
		return NULL;
	}
	if(strtab_sect == NULL)
	{
		vDirectPrintMsg("strtab section not found");
		return NULL;
	}
	if (symtab_sect->sh_entsize != sizeof(Elf32_Sym))
	{
		vDirectPrintMsg("Wrong .symtab entry size");
		return NULL;
	}

	syms = (Elf32_Sym *)((u_int32_t)elf_h + symtab_sect->sh_offset);
	//n = division(symtab_sect->sh_size, symtab_sect->sh_entsize);
	n = symtab_sect->sh_size/ symtab_sect->sh_entsize;

	for ( i =0 ; i < n; i++)
	{
		tname = get_shstr(elf_h, strtab_sect, syms[i].st_name);
		if(strcmp(tname, name) == 0)
			return &syms[i];
	}
	return NULL;
}

int find_symbols_in_elfhs(Elf32_Sym *in_symbol, Elf32_Sym **out_symbol,
		task_register_cons **out_symbol_trc, task_register_cons *app_trc)
{
	Elf32_Sym *final_symbol = NULL;
	char *symbol_name = NULL;
	Elf32_Shdr *strtab_sect = NULL;

	strtab_sect = find_section(".dynstr", app_trc->elfh);
	symbol_name = get_shstr(app_trc->elfh, strtab_sect, in_symbol->st_name);
	if (in_symbol->st_shndx == SHN_UNDEF)
		vDirectPrintMsg("Not handled \n");
	final_symbol = find_symbol(symbol_name, app_trc->elfh);

	*out_symbol = final_symbol;
	*out_symbol_trc = app_trc; //I am not looking in any othe ther elf file.
	return 1;
}

int link_relocations(task_register_cons *app_trc)
{
    long unsigned int i, j;
    Elf32_Shdr *app_symsect = NULL;
    Elf32_Shdr *strtab_sect = NULL;
    Elf32_Shdr *s = NULL;

    Elf32_Sym *app_symtab = NULL;
    Elf32_Sym *app_symbol =  NULL;
    Elf32_Sym * final_symbol = NULL;

    task_register_cons *symbol_trc = NULL;
    u_int32_t address = 0;
    u_int32_t *rel_address = NULL;

    s = (Elf32_Shdr*)((u_int32_t)app_trc->elfh + app_trc->elfh->e_shoff);

    app_symsect = find_section(".dynsym", app_trc->elfh);
    strtab_sect = find_section(".dynstr", app_trc->elfh);

    if (app_symsect == NULL)
    {
    	vDirectPrintMsg("could not find the .symtab section");
    }

    if (strtab_sect == NULL)
    {
    	vDirectPrintMsg("could not find .strtab section.");
    }

    app_symtab = (Elf32_Sym *)((u_int32_t)app_trc->elfh + app_symsect->sh_offset);

    /* for all relocation segments in the application */
    for (i = 0; i < app_trc->elfh->e_shnum; i++)
    {
    	if (s[i].sh_type != SHT_REL && s[i].sh_type != SHT_RELA)
    		continue;
    	if (s[i].sh_type == SHT_RELA)
    	{
    		vDirectPrintMsg("SHT_RELA is not supported\n");
    		return 0;
    	}
    	Elf32_Rel *r = (Elf32_Rel *)((u_int32_t)app_trc->elfh + s[i].sh_offset);

    	/* for all relocations in the relocation segment */
    	//for (j = 0; j < division(s[i].sh_size, s[i].sh_entsize); j++)
    	for (j = 0; j < s[i].sh_size/s[i].sh_entsize; j++)
    	{
    		switch (ELF32_R_TYPE(r[j].r_info))
    		{
    			case R_ARM_JUMP_SLOT:
    			case R_ARM_GLOB_DAT:
    			break;
    			case R_ARM_RELATIVE:
    				vDirectPrintMsg("not handled relative\n");
    				continue; //not handled.
    			default:
    				vDirectPrintMsg("Found not supported relocation type\n");
    			return 0;
    		}
    		/* 1 find the symbol in the give elf binaries. */
    		app_symbol = &app_symtab[ELF32_R_SYM(r[j].r_info)];
    		if (!find_symbols_in_elfhs(app_symbol, &final_symbol, &symbol_trc, app_trc))
    		{
    			vDirectPrintMsg("cannot locate the symbol\n");
    			return 0;
    		}
    		/* find absolute address of the symbol */
    		if (final_symbol->st_shndx == SHN_ABS)
    		{
    			address = final_symbol->st_value;
    		}
    		else if (symbol_trc == NULL)
    		{
    			/* if hte symbol_trc is NULL the symbol resides in the system binary */
    			address = final_symbol->st_value;
    		}
    		else
    		{
    			Elf32_Shdr *section_sect = (Elf32_Shdr*)((u_int32_t)symbol_trc->elfh + symbol_trc->elfh->e_shoff);
    			if (section_sect[final_symbol->st_shndx].sh_flags & SHF_ALLOC)
    			{
    				/* if SHF_ALLOC flag is set, the section should alreaby be allocated somewhere */
    				address = (u_int32_t)symbol_trc->cont_mem + final_symbol->st_value;
    			}
    			else
    			{
    				/*else we will fall back on the addressing the symbol in the elf. */
    				address = (u_int32_t)symbol_trc->elfh + (u_int32_t)final_symbol->st_value;
    			}
    		}
    		/*3 set the relocation address */
    		switch (ELF32_R_TYPE(r[j].r_info))
    		{
    			case R_ARM_JUMP_SLOT:
    				rel_address = (u_int32_t*)((u_int32_t)app_trc->cont_mem + r[j].r_offset);
    				*rel_address = (u_int32_t)address;
    				//vDirectPrintMsg("jump slot address");
    			break;
    			case R_ARM_GLOB_DAT:
    				rel_address = (u_int32_t*)((u_int32_t)app_trc->cont_mem + r[j].r_offset);
    				*rel_address = (u_int32_t)address;
    				//vDirectPrintMsg("jump glob address");
    			break;
    			default:
    			    vDirectPrintMsg("unknown relocation type\n");
    				return 0;
    		}
    	}
    }
    return 1;
}

request_hook_fn_t task_find_request_hook(task_register_cons *trc)
{
	request_hook_fn_t ret = NULL;
	Elf32_Sym *request_hook_symbol = find_symbol("cpRequestHook", trc->elfh);
	if (request_hook_symbol)
	{
		ret = (request_hook_fn_t)((u_int32_t)trc->cont_mem +
				(u_int32_t)request_hook_symbol->st_value);
	}
	else
	    vDirectPrintMsg("Failed to find request_hook");
	return ret;
}
int task_alloc(task_register_cons *trc)
{
    int i;
    char buffer[50];
    /* e_shoff : section header table's file offset in bytes */
    Elf32_Shdr *s = (Elf32_Shdr *)((u_int32_t)trc->elfh + trc->elfh->e_shoff);
    u_int32_t alloc_size = 0;
    u_int32_t cm_addr = 0;

    /* find out hte size of the continuous regions that has to be allocated.
     * e_shnum = no. of entries in section header table
     * */
    for (i = 0; i < trc->elfh->e_shnum; i++) 
    {
        if (s[i].sh_flags & SHF_ALLOC) //if the section needs to be allocated
        {
            /* sh_addr = virtual address where the section should be mapped
             * sh_size : size of he section in process image in memory
             */
            u_int32_t s_req = s[i].sh_addr + s[i].sh_size;
            alloc_size = alloc_size > s_req ? alloc_size : s_req;
        }

    }
    //vDirectPrintMsg("Memory required for task %s :%lu\n", trc->name, alloc_size);
    vDirectPrintMsg("Memory required for task\n");
    
    Elf32_Shdr *section_hdr = (Elf32_Shdr *) ((u_int32_t)trc->elfh + trc->elfh->e_shoff);
    Elf32_Shdr *strtab_sect = &section_hdr[trc->elfh->e_shstrndx];

    if (strtab_sect == NULL)
    {
        //vDirectPrintMsg("found no.strtabe in elfh for task %s\n", trc->name);
        vDirectPrintMsg("found no.strtabe in elfh for task \n");
    }

    /* use TASKSECTION_MALLOC_CALL for allocating task memory)
     */
    cm_addr = (u_int32_t)TASKSECTION_MALLOC_CALL(alloc_size);
    if (cm_addr == 0)
    {
        //vDirectPrintMsg("could not allocate memory for task %s \n", trc->name);
        vDirectPrintMsg("could not allocate memory for task\n");
        return 0;
    }
    trc->cont_mem = (void *)cm_addr;
    trc->cont_mem_size = alloc_size;

    LIST_INIT(&trc->sections);

    for (i = 0; i < trc->elfh->e_shnum; i++)
    {
        if (s[i].sh_flags & SHF_ALLOC)
        {
            struct task_section_cons_t *tsc = (struct task_section_cons_t *)
                SYSTEM_MALLOC_CALL(sizeof(task_section_cons));
            if (tsc == NULL)
            {
                //vDirectPrintMsg("could not allocate memory for section while allocating mem for task %s\n", tsc->name);
                vDirectPrintMsg("could not allocate memory for section while allocating mem for task \n");
                return 0;
            }
            tsc->name = (char *)((u_int32_t)trc->elfh + (u_int32_t)strtab_sect->sh_offset + s[i].sh_name);
            //vDirectPrintMsg("processing allocation for section %s\n", tsc->name);
            sprintf(buffer, "allocation for section %s \n", tsc->name);
            vDirectPrintMsg(buffer);

            tsc->section_index = i;
            tsc->amem = (void *)(cm_addr + s[i].sh_addr);
            LIST_INSERT_HEAD(&trc->sections, tsc, sections);

            if (s[i].sh_type != SHT_NOBITS)
            {
                /* coy the section if it contains data */
                memcpy(tsc->amem, (void *)((u_int32_t)trc->elfh + (u_int32_t)s[i].sh_offset), s[i].sh_size);
            }
            else
            {
                bzero(tsc->amem, s[i].sh_size);
            }

        }

    }
    trc->request_hook = task_find_request_hook(trc);
    return 1;
}

task_register_cons *task_register(const char *name, Elf32_Ehdr *elfh)
{
    vDirectPrintMsg("tasks registered called:\n");

    struct task_register_cons_t *trc = 
        (task_register_cons *)SYSTEM_MALLOC_CALL(sizeof(task_register_cons));
    if (trc == NULL)
    {
        vDirectPrintMsg("Failed to allo in task register: \n");
        return NULL;
    }

    trc->name = name;
    trc->elfh = elfh;
    trc->task_handle = 0;
    trc->request_hook = NULL;
    //TASK_ACQUIRE_TR_LOCK();
    //RB_INSERT(task_register_tree_t, &task_register_tree_var, trc);
    //TASK_RELEASE_TR_LOCK();

    //trc->request_hook = NULL;
    trc->cont_mem = NULL;
    trc->cont_mem_size = 0;

    LIST_INIT(&trc->sections);
    //vDirectPrintMsg("tasks registered: %i \n", get_number_of_tasks());


    return trc;
}

int task_link(task_register_cons *trc)
{
    if (!check_elf_magic(trc->elfh))
    {
        vDirectPrintMsg("elf magic not correct \n");
        return 0;
    }

    if (trc->cont_mem == NULL)
    {
        vDirectPrintMsg("Continuous memory not available\n");
        return 0;
    }

    if (!link_relocations(trc))
    {
        vDirectPrintMsg("relocation failed\n");
        return 0;
    }
    return 1;
}

int task_start(task_register_cons *trc)
{
	Elf32_Sym *entry_sym =  NULL;
	entry_ptr_t entry_point = NULL;
	entry_sym = find_symbol("_start", trc->elfh);
	//entry_sym = find_symbol("simple_entry", trc->elfh);

	if (entry_sym == NULL)
	{
		vDirectPrintMsg("task start: entry symbol not found\n");
		return 0;
	}
	entry_point = trc->cont_mem + entry_sym->st_value;
	if(entry_sym == NULL)
		vDirectPrintMsg("did not find entry for the task\n");

	if((u_int32_t)entry_sym & 0x03)
	{
		vDirectPrintMsg("entry point is not 4 byte aligned\n");
	}

	if(xTaskCreate((pdTASK_CODE)entry_point, (const char *)trc->name,
			configMINIMAL_STACK_SIZE, NULL,
			APPLICATION_TASK_PRIORITY, &trc->task_handle) != pdPASS)
	{
		vDirectPrintMsg("could not create task\n");
	}
	vDirectPrintMsg("tsk started successfully\n");
	vTaskDelay(2000);
	return 1;
}

int task_start_v1(task_register_cons *trc)
{
    Elf32_Sym *entry_sym =  NULL;
    entry_ptr_t entry_point = NULL;
    entry_sym = find_symbol("_start_v1", trc->elfh);
    //entry_sym = find_symbol("simple_entry", trc->elfh);

    if (entry_sym == NULL)
    {
        vDirectPrintMsg("task start: entry symbol not found\n");
        return 0;
    }
    entry_point = trc->cont_mem + entry_sym->st_value;
    if(entry_sym == NULL)
        vDirectPrintMsg("did not find entry for the task\n");

    if((u_int32_t)entry_sym & 0x03)
    {
        vDirectPrintMsg("entry point is not 4 byte aligned\n");
    }

    if(xTaskCreate((pdTASK_CODE)entry_point, (const char *)trc->name,
            configMINIMAL_STACK_SIZE, NULL,
            APPLICATION_TASK_PRIORITY, &trc->task_handle) != pdPASS)
    {
        vDirectPrintMsg("could not create task\n");
    }
    vDirectPrintMsg("tsk started successfully\n");
    return 1;
}

void  *task_get_section_address(task_register_cons *trc, Elf32_Half index)
{
    Elf32_Shdr *section_hdr = (Elf32_Shdr *)((u_int32_t)trc->elfh + trc->elfh->e_shoff);
    struct task_section_cons_t *p;

    if (index > trc->elfh->e_shnum)
        return NULL;

    if (section_hdr[index].sh_flags & SHF_ALLOC)
    {
        /* section should have been allocated uptil now */
        LIST_FOREACH(p, &trc->sections, sections)
        {
            if (p->section_index == index)
                return p->amem;
        }
    }
    else
    {
        vDirectPrintMsg("section address not allocated");
        /* section is not allocated , returning the address of the elf binary */
        return (void *)((u_int32_t)trc->elfh + section_hdr[index].sh_offset);
    }
    vDirectPrintMsg("section address not found");
    return NULL;
}

int task_call_crh(task_register_cons *trc, cp_req_t req_type)
{
	if (trc->request_hook != NULL)
		(trc->request_hook)(req_type);
	else
	    return 0;
	return 1;
}

int task_wait_for_checkpoint(task_register_cons *trc, cp_req_t req_type)
{
	if (!task_call_crh(trc, req_type))
	{
		vDirectPrintMsg("could not call checkpoint request hook");
		return 0;
	}

	return 1;

}

