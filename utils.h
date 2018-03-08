/*
 * utils.h
 *
 *  Created on: Mar 5, 2018
 *      Author: dachee
 */

#ifndef UTILS_H_
#define UTILS_H_

//#undef bool
//#undef true
//#undef false
//typedef enum
//{
//	false = 0,
//	true = 1,
//} bool;

#define SIZEOF_ARRAY(arr)		(sizeof(arr) / sizeof(arr[0]))
#define FOREACH(ptr, arr)		for (ptr = arr; ptr < arr + SIZEOF_ARRAY(arr); ptr++)
#define FOREACH_REV(ptr, arr)	for (ptr = arr + SIZEOF_ARRAY(arr) - 1; ptr >= arr; ptr--)

#endif /* UTILS_H_ */
