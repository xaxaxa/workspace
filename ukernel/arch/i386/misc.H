/*
 * misc.H
 *
 *  Created on: Nov 5, 2013
 *      Author: xaxaxa
 */

#ifndef MISC_H_
#define MISC_H_

static const int PAGESIZE = 4096;
static const ptr_int MMAP_ERRVAL_START = (ptr_int) -2048;
#define IS_MMAP_ERROR(x) ((ptr_int)x>=MMAP_ERRVAL_START)
#define MMAP_ERROR(x) int((ptr_int)0xFFFFFFFF-(ptr_int)x)
#define TO_MMAP_ERROR(err) (u8*)((ptr_int)0xFFFFFFFF-(ptr_int)err)
#endif /* MISC_H_ */
