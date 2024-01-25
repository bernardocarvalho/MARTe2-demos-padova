






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

#include "../include/atca-v6-pcie-ioctl.h"

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
    //STATUS_REG sReg;
    PCIE_DEV *pcieDev; /* for device information */
    void __iomem *reg;
    //u32 w;

    u32 byteSize;

    /* retrieve the device information  */
    pcieDev = (PCIE_DEV *)filp->private_data;

    reg = &pcieDev->pModDmaHregs->dmaStatus;
    mutex_lock(&pcieDev->io_lock);
    tmp = ioread32(reg);
    mutex_unlock(&pcieDev->io_lock);
    err = (tmp == 0xFFFFFFFF);
    if (err) {
        printk(KERN_ERR "ioctl status Reg: 0x08%X, cmd: 0x%X\n", tmp, cmd);
        return -EFAULT;
    }

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
            //spin_lock_irqsave(&pcieDev->irq_lock, flags);
            reg = &pcieDev->pModDmaHregs->dmaStatus;
            mutex_lock(&pcieDev->io_lock);
            //  ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- ----- -----
            tmp = ioread32(reg);
            //tmp = ioread32((void *) &pcieDev->pModDmaHregs->dmaStatus);
            //printk(KERN_DEBUG "%s dmaStatus:0x%08X, dmaDebug0:0x%08X, dmaDebug1:0x%08X\n", DRV_NAME, tmp,
            //    ioread32((void *) &pciDev->pModDmaHregs->dmaDebug0),
            //    ioread32((void *) &pciDev->pModDmaHregs->dmaDebug1));
            //  ----- ----- ----- ----- ----- DEVICE SPECIFIC CODE ----- ----- -----
            mutex_unlock(&pcieDev->io_lock);
            //spin_unlock_irqrestore(&pcieDev->irq_lock, flags);

            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPT_IRQ_ENABLE:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.DmaIntE = 1;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);
            break;

        case ATCA_PCIE_IOPT_IRQ_DISABLE:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.DmaIntE = 0;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);
            break;

        case ATCA_PCIE_IOPT_ACQ_ENABLE:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            pcieDev->mismatches = 0;
            pcieDev->max_buffer_count = 0;
            atomic_set(&pcieDev->rd_condition, 0);
            cReg.cmdFlds.AcqE = 1;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);
            break;

        case ATCA_PCIE_IOPT_ACQ_DISABLE:
            retval = pcieDev->max_buffer_count;
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.AcqE = 0;
            cReg.cmdFlds.StrG = 0;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);

            break;
        case ATCA_PCIE_IOPT_DMA_ENABLE:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.DmaE = 1;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);
            break;
        case ATCA_PCIE_IOPT_DMA_DISABLE:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.DmaE = 0;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);

            break;
        case ATCA_PCIE_IOPT_DMA_RESET:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            cReg.reg32 = ioread32((void *)&pcieDev->pModDmaHregs->dmaControl);
            cReg.cmdFlds.DmaRst = 1;
            iowrite32(cReg.reg32, (void *)&pcieDev->pModDmaHregs->dmaControl);
            byteSize = ioread32((void *)&pcieDev->pModDmaHregs->dmaByteSize);
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            // clear DMA buffers
            for(i = 0; i < DMA_BUFFS; i++){
               memset(pcieDev->dmaIO.buf[i].addr_v, 0, byteSize);
               memset(pcieDev->dmaRT.buf[i].addr_v, 0, PAGE_SIZE);
            //memset(pcieDev->dmaPollBuff.addr_v, 0, PAGE_SIZE);
            }
            //    udelay(10);
            cReg.cmdFlds.DmaRst = 0;
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            iowrite32(cReg.reg32, (void *)&pcieDev->pModDmaHregs->dmaControl);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            udelay(20);
            break;
            /*
               case ATCA_PCIE_IOPG_COUNTER:
               spin_lock_irqsave(&pcieDev->irq_lock, flags);
               tmp = PCIE_READ32((void*) &pcieDev->pHregs->hwcounter);
               spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
               if(copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
               return -EFAULT;
               break;
               */
        case ATCA_PCIE_IOPS_RDTMOUT:
            retval = __get_user(tmp, (int __user *)arg);
            if (!retval)
                pcieDev->wt_tmout = tmp * HZ;
            break;


        case ATCA_PCIE_IOPT_SOFT_TRIG:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.StrG = 1;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);
            break;

        case ATCA_PCIE_IOPS_DMA_SIZE:
            retval = __get_user(tmp, (int __user *)arg);
            if (!retval) {
                spin_lock_irqsave(&pcieDev->irq_lock, flags);
                iowrite32(tmp, (void *)&pcieDev->pModDmaHregs->dmaByteSize); // write the buffer size to the FPGA
                spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            }
            break;

        case ATCA_PCIE_IOPG_DMA_SIZE:
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            tmp = ioread32((void *)&pcieDev->pModDmaHregs->dmaByteSize);
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPT_STREAM_ENABLE:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.StreamE = 1;
            iowrite32(cReg.reg32, reg);
            printk(KERN_DEBUG "%s STREAM_ENABLE: dmaControl: 0x%08X\n", DRV_NAME, ioread32(reg));
            mutex_unlock(&pcieDev->io_lock);
            break;

        case ATCA_PCIE_IOPT_STREAM_DISABLE:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.StreamE = 0;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);
            break;

        case ATCA_PCIE_IOPT_CHOP_ON:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.ChopOn = 1;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);
            break;

        case ATCA_PCIE_IOPT_CHOP_OFF:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            cReg.reg32 = ioread32(reg);
            cReg.cmdFlds.ChopOn = 0;
            iowrite32(cReg.reg32, reg);
            mutex_unlock(&pcieDev->io_lock);
            break;

        case ATCA_PCIE_IOPS_EO_OFFSETS:
            if (copy_from_user((void *)&eo_obj, (void *)arg,
                        sizeof(struct atca_eo_config))) {
                /*pr_err("copy_from_user failed.\n");*/
                return -EFAULT;
            }
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            for (i = 0; i < ADC_CHANNELS; i++) {
                PDEBUGG("ioctl S eo_off Reg:%d, off: 0x%08X", i, eo_obj.offset[i]);
                iowrite32(eo_obj.offset[i],
                        (void *)&pcieDev->pModDmaHregs->eOffsets[i]);
            }
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPG_EO_OFFSETS:
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            for (i = 0; i < ADC_CHANNELS; i++) {
                eo_obj.offset[i] = ioread32((void *)&pcieDev->pModDmaHregs->eOffsets[i]);
                PDEBUGG("ioctl G eo_off Reg:%d, off: %d", i, eo_obj.offset[i]);
            }
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &eo_obj, sizeof(struct atca_eo_config)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPS_WO_OFFSETS:
            if (copy_from_user((void *)&wo_obj, (void *)arg,
                        sizeof(struct atca_wo_config))) {
                /*pr_err("copy_from_user failed.\n");*/
                return -EFAULT;
            }
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            for (i = 0; i < ADC_CHANNELS; i++) {
                PDEBUGG("ioctl S wo_off Reg:%d, off: %d", i, wo_obj.offset[i]);
                iowrite32(wo_obj.offset[i],
                        (void *)&pcieDev->pModDmaHregs->wOffsets[i]);
            }
            // ----- ----- DEVICE SPECIFIC CODE ----- -----
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            break;

        case ATCA_PCIE_IOPG_WO_OFFSETS:
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            for (i = 0; i < ADC_CHANNELS; i++) {
                wo_obj.offset[i] = ioread32((void *)&pcieDev->pModDmaHregs->wOffsets[i]);
                PDEBUGG("ioctl G wo_off Reg:%d, off: %d", i, wo_obj.offset[i]);
            }
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &wo_obj, sizeof(struct atca_wo_config)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPS_CHOP_COUNTERS:
            retval = __get_user(tmp, (int __user *)arg);
            if (!retval) {
                PDEBUGG("ioctl S chopCounters Reg: 0x%08X", tmp);
                spin_lock_irqsave(&pcieDev->irq_lock, flags);
                iowrite32(tmp, (void *)&pcieDev->pModDmaHregs->chopCountrs);
                spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            }
            break;

        case ATCA_PCIE_IOPG_CHOP_COUNTERS:
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            tmp = ioread32((void *)&pcieDev->pModDmaHregs->chopCountrs);
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            PDEBUGG("ioctl G chopCounters Reg: 0x%08X", tmp);
            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;

        case ATCA_PCIE_IOPG_CONTROL:
            reg = &pcieDev->pModDmaHregs->dmaControl;
            mutex_lock(&pcieDev->io_lock);
            tmp = ioread32(reg);
            mutex_unlock(&pcieDev->io_lock);
            //spin_lock_irqsave(&pcieDev->irq_lock, flags);
            //tmp = ioread32((void *) &pcieDev->pModDmaHregs->dmaControl);
            //spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;
        
        case ATCA_PCIE_IOPS_CHOP_DISABLE:
            retval = __get_user(tmp, (int __user *)arg);
            if (!retval) {
                spin_lock_irqsave(&pcieDev->irq_lock, flags);
                iowrite32(tmp, (void *)&pcieDev->pModDmaHregs->chopDisable);
                spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            }
            break;

        case ATCA_PCIE_IOPG_CHOP_DISABLE:
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            tmp = ioread32((void *) &pcieDev->pModDmaHregs->chopDisable);
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;
        /*
        case ATCA_PCIE_IOPS_DACS_REG:
            retval = __get_user(tmp, (int __user *)arg);
            if (!retval) {
                spin_lock_irqsave(&pcieDev->irq_lock, flags);
                iowrite32(tmp, (void *)&pcieDev->pModDmaHregs->dacsReg);
                spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            }
            break;
*/
        case ATCA_PCIE_IOPG_DACS_REG:
            spin_lock_irqsave(&pcieDev->irq_lock, flags);
            tmp = ioread32((void *)&pcieDev->pModDmaHregs->dacsReg);
            spin_unlock_irqrestore(&pcieDev->irq_lock, flags);
            if (copy_to_user((void __user *)arg, &tmp, sizeof(u32)))
                return -EFAULT;
            break;

        default: /* redundant, as cmd was checked against MAXNR */
            return -ENOTTY;
    }
    return retval;
}

//  vim: syntax=c ts=4 sw=4 sts=4 sr et
