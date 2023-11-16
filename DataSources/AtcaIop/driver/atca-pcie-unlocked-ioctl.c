/**
 *
 * @file kc705-unlocked-ioctl.c
 * @author Bernardo Carvalho
 * @date 2014-06-27
 * @brief Contains the functions handling the different ioctl calls.
 *
 * Copyright 2014 - 2019 IPFN-Instituto Superior Tecnico, Portugal
 * Creation Date  2014-06-27
 *
 * Licensed under the EUPL, Version 1.2 only (the "Licence");
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence, available in 23 official languages of
 * the European Union, at:
 * https://joinup.ec.europa.eu/community/eupl/og_page/eupl-text-11-12
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the Licence is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the Licence for the specific language governing permissions and
 * limitations under the Licence.
 *
 */

/* Configuration for the driver (what should be compiled in, module name,
 * etc...) */
#include "config.h"

/* Internal definitions for all parts (includes, prototypes, data, macros) */
#include "common.h"


/**
 * _unlocked_ioctl
 */
long _unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {

    int i;
    int err = 0, retval = 0;
    unsigned long flags = 0;
    u32 tmp;
    struct atca_eo_config eo_obj;
    struct atca_wo_config wo_obj;
    COMMAND_REG cReg;
    STATUS_REG sReg;
    PCIE_DEV *pciDev; /* for device information */

    u32 byteSize;

    /* retrieve the device information  */
    pciDev = (PCIE_DEV *)filp->private_data;

    sReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaStatus);
    if (sReg.reg32 == 0xFFFFFFFF)
        PDEBUG("ioctl status Reg:0x%X, cmd: 0x%X\n", sReg.reg32, cmd);

    /**
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
     */
    if (_IOC_TYPE(cmd) != ATCA_PCIE_IOP_MAGIC)
        return -ENOTTY;
    if (_IOC_NR(cmd) > ATCA_PCIE_IOP_MAXNR)
        return -ENOTTY;

    /*
     * the direction is a bitmask, and VERIFY_WRITE catches R/W
     * transfers. `Type' is user-oriented, while
     * access_ok is kernel-oriented, so the concept of "read" and
     * "write" is reversed
     */
    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
    //    err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err =  !access_ok((void __user *)arg, _IOC_SIZE(cmd));
    //    err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    if (err)
        return -EFAULT;
    switch (cmd) {

        case ATCA_PCIE_IOPG_STATUS:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            //  ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- ----- -----
            tmp = ioread32((void *) &pciDev->pModDmaHregs->dmaStatus);
            printk(KERN_DEBUG "%s dmaStatus:0x%08X, dmaDebug0:0x%08X, dmaDebug1:0x%08X\n", DRV_NAME, tmp,
                ioread32((void *) &pciDev->pModDmaHregs->dmaDebug0),
                ioread32((void *) &pciDev->pModDmaHregs->dmaDebug1));
            //  ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);

            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPT_IRQ_ENABLE:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            cReg.reg32 = ioread32((void *) &pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.DmaIntE = 1;
            iowrite32(cReg.reg32, (void *) &pciDev->pModDmaHregs->dmaControl);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            // ----- ----- ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPT_IRQ_DISABLE:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.DmaIntE = 0;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            // ----- ----- ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPT_ACQ_ENABLE:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            pciDev->mismatches = 0;
            /*pciDev->curr_buf = 0;*/
            pciDev->max_buffer_count = 0;
            atomic_set(&pciDev->rd_condition, 0);
            cReg.cmdFlds.AcqE = 1;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPT_ACQ_DISABLE:
            retval = pciDev->max_buffer_count;
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            // ----- ----- ----- -----
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.AcqE = 0;
            cReg.cmdFlds.StrG = 0;
            //    cReg.cmdFlds.STRG=0;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            // ----- ----- ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);

            break;
        case ATCA_PCIE_IOPT_DMA_ENABLE:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            // ----- ----- ----- -----
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.DmaE = 1;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            // ----- ----- ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);

            break;
        case ATCA_PCIE_IOPT_DMA_DISABLE:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            // ----- ----- ----- -----
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.DmaE = 0;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            // ----- ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            // ----- ----- ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);

            break;
        case ATCA_PCIE_IOPT_DMA_RESET:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.DmaRst = 1;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            byteSize = ioread32((void *)&pciDev->pModDmaHregs->dmaByteSize);
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            // clear DMA buffers
            for(i = 0; i < DMA_BUFFS; i++){
               memset(pciDev->dmaIO.buf[i].addr_v, 0, byteSize);
               memset(pciDev->dmaRT.buf[i].addr_v, 0, PAGE_SIZE);
            //memset(pciDev->dmaPollBuff.addr_v, 0, PAGE_SIZE);
            }
            //    udelay(10);
            cReg.cmdFlds.DmaRst = 0;
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            udelay(20);
            break;
            /*
               case ATCA_PCIE_IOPG_COUNTER:
               spin_lock_irqsave(&pciDev->irq_lock, flags);
               tmp = PCIE_READ32((void*) &pciDev->pHregs->hwcounter);
               spin_unlock_irqrestore(&pciDev->irq_lock, flags);
               if(copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
               return -EFAULT;
               break;
               */
        case ATCA_PCIE_IOPS_RDTMOUT:
            retval = __get_user(tmp, (int __user *)arg);
            if (!retval)
                pciDev->wt_tmout = tmp * HZ;
            break;


        case ATCA_PCIE_IOPT_SOFT_TRIG:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            //  ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.StrG = 1;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            // ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPS_DMA_SIZE:
            retval = __get_user(tmp, (int __user *)arg);
            if (!retval) {
                spin_lock_irqsave(&pciDev->irq_lock, flags);
                iowrite32(tmp, (void *)&pciDev->pModDmaHregs->dmaByteSize); // write the buffer size to the FPGA
                spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            }
            break;

        case ATCA_PCIE_IOPG_DMA_SIZE:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            tmp = ioread32((void *)&pciDev->pModDmaHregs->dmaByteSize);
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPT_STREAM_ENABLE:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.StreamE = 1;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            PDEBUG("%s ioctl control Reg:0x%08X\n", DRV_NAME,
                    ioread32((void *) &pciDev->pModDmaHregs->dmaControl));
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPT_STREAM_DISABLE:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.StreamE = 0;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPT_CHOP_ON:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.ChopOn = 1;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            // -----  DEVICE SPECIFIC CODE ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPT_CHOP_OFF:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            cReg.reg32 = ioread32((void *)&pciDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.ChopOn = 0;
            iowrite32(cReg.reg32, (void *)&pciDev->pModDmaHregs->dmaControl);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPS_EO_OFFSETS:
            if (copy_from_user((void *)&eo_obj, (void *)arg,
                        sizeof(struct atca_eo_config))) {
                /*pr_err("copy_from_user failed.\n");*/
                return -EFAULT;
            }
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            for (i = 0; i < ADC_CHANNELS; i++) {
                PDEBUGG("ioctl S eo_off Reg:%d, off: 0x%08X", i, eo_obj.offset[i]);
                iowrite32(eo_obj.offset[i],
                        (void *)&pciDev->pModDmaHregs->eOffsets[i]);
            }
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPG_EO_OFFSETS:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            for (i = 0; i < ADC_CHANNELS; i++) {
                eo_obj.offset[i] = ioread32((void *)&pciDev->pModDmaHregs->eOffsets[i]);
                PDEBUGG("ioctl G eo_off Reg:%d, off: %d", i, eo_obj.offset[i]);
            }
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &eo_obj, sizeof(struct atca_eo_config)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPS_WO_OFFSETS:
            if (copy_from_user((void *)&wo_obj, (void *)arg,
                        sizeof(struct atca_wo_config))) {
                /*pr_err("copy_from_user failed.\n");*/
                return -EFAULT;
            }
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            for (i = 0; i < ADC_CHANNELS; i++) {
                PDEBUGG("ioctl S wo_off Reg:%d, off: %d", i, wo_obj.offset[i]);
                iowrite32(wo_obj.offset[i],
                        (void *)&pciDev->pModDmaHregs->wOffsets[i]);
            }
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPG_WO_OFFSETS:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            for (i = 0; i < ADC_CHANNELS; i++) {
                wo_obj.offset[i] = ioread32((void *)&pciDev->pModDmaHregs->wOffsets[i]);
                PDEBUGG("ioctl G wo_off Reg:%d, off: %d", i, wo_obj.offset[i]);
            }
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &wo_obj, sizeof(struct atca_wo_config)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPS_CHOP_COUNTERS:
            retval = __get_user(tmp, (int __user *)arg);
            if (!retval) {
                PDEBUG("ioctl S chopCounters Reg: 0x%08X", tmp);
                spin_lock_irqsave(&pciDev->irq_lock, flags);
                iowrite32(tmp, (void *)&pciDev->pModDmaHregs->chopCountrs);
                spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            }
            break;

        case ATCA_PCIE_IOPG_CHOP_COUNTERS:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            tmp = ioread32((void *)&pciDev->pModDmaHregs->chopCountrs);
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            PDEBUG("ioctl G chopCounters Reg: 0x%08X", tmp);
            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPG_CONTROL:
            spin_lock_irqsave(&pciDev->irq_lock, flags);
            tmp = ioread32((void *) &pciDev->pModDmaHregs->dmaControl);
            spin_unlock_irqrestore(&pciDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;

            /*
             *  case KC705_PCIE_IOCG_TMR0COUNT:
             *    spin_lock_irqsave(&pciDev->irq_lock, flags);
             *      tmp = ioread32((void*) &pciDev->pHregs->timer0Count);
             *    spin_unlock_irqrestore(&pciDev->irq_lock, flags);
             *    if(copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
             *      return -EFAULT;
             *    break;
             *

            */
        default: /* redundant, as cmd was checked against MAXNR */
            return -ENOTTY;
    }
    return retval;
}

//  vim: syntax=c ts=4 sw=4 sts=4 sr et
