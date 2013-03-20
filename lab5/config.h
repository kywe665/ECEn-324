/*********************************************************
 * config.h - Configuration data for the driver.c program.
 *********************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* 
 * CPEs for the baseline (naive) version of the rotate function that
 * was handed out to the students. Rd is the measured CPE for a dxd
 * image. Run the driver.c program on your system to get these
 * numbers.  
 */
#define R64    3.3
#define R128   4.1
#define R256   7.1
#define R512   10.6
#define R1024  14.0

/* 
 * CPEs for the baseline (naive) version of the smooth function that
 * was handed out to the students. Sd is the measure CPE for a dxd
 * image. Run the driver.c program on your system to get these
 * numbers.  
 */
/*#define S32   298
#define S64   298
#define S128  300
#define S256  301
#define S512  302*/
#define S32   189.6
#define S64   192.8
#define S128  196.5
#define S256  197.0
#define S512  197.4


#endif /* _CONFIG_H_ */
