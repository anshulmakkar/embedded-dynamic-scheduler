/***********************************************************************************/
/* Copyright (c) 2013, Wictor Lund. All rights reserved.			   */
/* Copyright (c) 2013, Åbo Akademi University. All rights reserved.		   */
/* 										   */
/* Redistribution and use in source and binary forms, with or without		   */
/* modification, are permitted provided that the following conditions are met:	   */
/*      * Redistributions of source code must retain the above copyright	   */
/*        notice, this list of conditions and the following disclaimer.		   */
/*      * Redistributions in binary form must reproduce the above copyright	   */
/*        notice, this list of conditions and the following disclaimer in the	   */
/*        documentation and/or other materials provided with the distribution.	   */
/*      * Neither the name of the Åbo Akademi University nor the		   */
/*        names of its contributors may be used to endorse or promote products	   */
/*        derived from this software without specific prior written permission.	   */
/* 										   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND */
/* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE	   */
/* DISCLAIMED. IN NO EVENT SHALL ÅBO AKADEMI UNIVERSITY BE LIABLE FOR ANY	   */
/* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES	   */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;	   */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND	   */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT	   */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS   */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 		   */
/***********************************************************************************/

#ifndef _APPLICATIONS_H_
#define _APPLICATIONS_H_

#include <FreeRTOS.h>

#include "system.h"
#include "elf.h"
//#include "types.h"

#define APPLICATION_TASK_PRIORITY 2

extern u_int8_t _simple_elf_start;
extern u_int8_t _simple_elf_end;
extern u_int8_t _simple_v1_elf_start;
extern u_int8_t _simple_v1_elf_end;
extern void* simple_elf_v1;

//extern u_int8_t _binary_obj_app_image_elf_start;
//extern u_int8_t _binary_obj_app_image_elf_end;

#define APPLICATION_ELF(app) ((Elf32_Ehdr *)&_ ## app ## _elf_start)
#define APPLICATION_ELF_V1(app) ((Elf32_Ehdr *)&_ ## app ## _elf_start)

//#define APPLICATION_ELF(app) ((Elf32_Ehdr *)&_ ## app ## _elf_start)
#ifdef HAS_SYSTEM_ELF_SYMBOL
#define SYSTEM_ELF ((Elf32_Ehdr *)&_system_elf_start)
#else /* HAS_SYSTEM_ELF_SYMBOL */
#ifdef HAS_BINARY_REGISTER
#include <System/memory_layout.h>
#define SYSTEM_ELF (BINARY_REGISTER[0].elfh)
#else /* HAS_BINARY_REGISTER */
#define SYSTEM_ELF ((Elf32_Ehdr *)NULL)
#endif /* HAS_BINARY_REGISTER */
#endif /* HAS_SYSTEM_ELF_SYMBOL */

#endif /* _APPLICATIONS_H_ */
