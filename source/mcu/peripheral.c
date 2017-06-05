/*
 * peripheral.c
 *
 *  Created on: Apr 12, 2015
 *      Author: nenad
 */


#include "mcu/peripheral.h"
#include <stddef.h>

struct npdrv * npdrv_request(uint32_t dev_id)
{
    const struct npdev *        pdev;
    struct npdrv *              pdrv;

    pdrv = NULL;
    pdev = nprofile_pdev(dev_id);

    /* NOTE 1:
     * First check if this is the device class we are asked for was found.
     */
    if (pdev) {
        ncore_lock              lock;

        pdrv = pdev->pdrv;

        /* NOTE 2:
         * Check if we have not reached the maximum number of users for this
         * driver.
         */
        ncore_lock_enter(&lock);

        if (pdrv->ref < pdev->max_ref) {
            pdrv->ref++;

            /* NOTE 3:
             * Check if this is the first time we are asked for this driver
             */
            if (pdrv->ref == 1) {
                pdrv->pdev = pdev;
                pdrv->data = NULL;
            }
        }
        ncore_lock_exit(&lock);
    }

    return (pdrv);
}



void npdrv_release(struct npdrv * pdrv)
{
    ncore_lock                  lock;
    /* NOTE 1:
     * Remove one user for this driver.
     */
    ncore_lock_enter(&lock);
    pdrv->ref--;
    ncore_lock_exit(&lock);
}



struct npdrv * npdrv_from_id(uint32_t dev_id)
{
    const struct npdev *        pdev;
    struct npdrv *              pdrv;

    pdev = nprofile_pdev(dev_id);

    if (pdev) {
        pdrv = npdev_to_pdrv(pdev);

        return (pdrv);
    }

    return (NULL);
}
