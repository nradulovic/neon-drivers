/*
 * peripheral.c
 *
 *  Created on: Apr 12, 2015
 *      Author: nenad
 */


#include "mcu/peripheral.h"

struct npdrv * npdrv_request(uint32_t dev_id)
{
	const struct npdev *		pdev;

	pdev = nprofile_pdev(dev_id);

	/* NOTE 1:
	 * First check if this is the device class we are asked for was found.
	 */
	if (pdev) {
		struct npdrv *			pdrv;

		pdrv = pdev->pdrv;

		/* NOTE 2:
		 * Check if we have not reached the maximum number of users for this
		 * driver.
		 */
		if (ncore_atomic_read(&pdrv->ref) < pdev->max_ref) {
			ncore_atomic_inc(&pdrv->ref);

			/* NOTE 3:
			 * Check if this is the first time we are asked for this driver
			 */
			if (ncore_atomic_read(&pdrv->ref) == 1) {
				pdrv->pdev = pdev;
				pdrv->data = NULL;
			}

			return (pdrv);
		}
	}

	return (NULL);
}

void npdrv_release(struct npdrv * pdrv)
{
	/* NOTE 1:
	 * Remove one user for this driver.
	 */
	ncore_atomic_dec(&pdrv->ref);
}
