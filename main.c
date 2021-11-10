/*Block Device Driver To create a 512Kb Block on RAM named  and Partition 
 *it into 2 Logical Partition 
 * Author SUNIL, Assignment-2 */
#include <linux/version.h> 	/* LINUX_VERSION_CODE  */
#include <linux/blk-mq.h>	
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>	/* invalidate_bdev */
#include <linux/bio.h>

static int major_number = 0;
static int sector_size = 512;
static int numberofsectors = 1024;	/* How big the drive is */

/*
 * Minor number and partition management.
 */
#define minor_number 2

/*defining partition*/
#define KERNEL_SECTOR_SIZE 512
#define MBR_SIZE KERNEL_SECTOR_SIZE
#define MBR_DISK_SIGNATURE_OFFSET 440
#define MBR_DISK_SIGNATURE_SIZE 4
#define PARTITION_TABLE_OFFSET 446
#define PARTITION_ENTRY_SIZE 16 
#define PARTITION_TABLE_SIZE 64 
#define MBR_SIGNATURE_OFFSET 510
#define MBR_SIGNATURE_SIZE 2
#define MBR_SIGNATURE 0xAA55

//defining paritition table
typedef struct
{
	unsigned char boot_type; // 0x00 - Inactive; 0x80 - Active (Bootable)
	unsigned char start_head;
	unsigned char start_sec:6;
	unsigned char start_cyl_hi:2;
	unsigned char start_cyl;
	unsigned char part_type;
	unsigned char end_head;
	unsigned char end_sec:6;
	unsigned char end_cyl_hi:2;
	unsigned char end_cyl;
	unsigned int abs_start_sec;
	unsigned int sec_in_part;
} PartEntry;
//two partitions
typedef PartEntry PartTable[2];

static PartTable def_part_table =
{
	{
		boot_type: 0x00,
		start_head: 0x00,
		start_sec: 0x2,
		start_cyl: 0x00,
		part_type: 0x83,
		end_head: 0x00,
		end_sec: 0x20,
		end_cyl: 0x09,
		abs_start_sec: 0x00000001,
		sec_in_part: 0x0000013F
	},
	{
		boot_type: 0x00,
		start_head: 0x00,
		start_sec: 0x1,
		start_cyl: 0x14,
		part_type: 0x83,
		end_head: 0x00,
		end_sec: 0x20,
		end_cyl: 0x1F,
		abs_start_sec: 0x00000280,
		sec_in_part: 0x00000180
	},
	{	
	},
	{
	}
};

struct sblock_dev {
        int length;                       /* Device length in sectors */
        u8 *data_array;                       /* The data_array array */
        spinlock_t spinlock;                /* For mutual exclusion */
	struct blk_mq_tag_set tag;	/* tag added */
        struct request_queue *queue;    /* The device request queue */
        struct gendisk *gd;             /* The gendisk structure */
}device;

//static struct sblock_dev *Devices = NULL;

static void block_transfer(struct sblock_dev *s_dev, unsigned long sec,
		unsigned long n_sec, char *buff, int direct)
{
	unsigned long offset = sec*KERNEL_SECTOR_SIZE;
	unsigned long nbytes = n_sec*KERNEL_SECTOR_SIZE;

	if ((offset + nbytes) > s_dev->length) {
		printk (KERN_NOTICE "Beyond-end direct (%ld %ld)\n", offset, nbytes);
		return;
	}
	if (direct)
		memcpy(s_dev->data_array + offset, buff, nbytes);
	else
		memcpy(buff, s_dev->data_array + offset, nbytes);
}
static void copy_mbr(u8 *disk)
{
	memset(disk, 0x0, MBR_SIZE);
	*(unsigned long *)(disk + MBR_DISK_SIGNATURE_OFFSET) = 0x36E5756D;
	memcpy(disk + PARTITION_TABLE_OFFSET, &def_part_table, PARTITION_TABLE_SIZE);
	*(unsigned short *)(disk + MBR_SIGNATURE_OFFSET) = MBR_SIGNATURE;
}



static inline struct request_queue *blk_generic_alloc_queue(int flags)
{
	return (blk_alloc_queue(flags));
}

static blk_status_t block_request(struct blk_mq_hw_ctx *hctx, const struct blk_mq_queue_data* bd)   /* For blk-mq */
{
	struct request *req = bd->rq;
	struct sblock_dev *s_dev = req->rq_disk->private_data;
        struct bio_vec bvec;
        struct req_iterator iterate;
        sector_t pos_sector = blk_rq_pos(req);
	void	*buff;
	blk_status_t  ret;

	blk_mq_start_request (req);

	if (blk_rq_is_passthrough(req)) {
		printk (KERN_NOTICE "Skip non-fs request\n");
                ret = BLK_STS_IOERR;  //-EIO
			goto exit;
	}
	rq_for_each_segment(bvec, req, iterate)
	{
		size_t num_sector = blk_rq_cur_sectors(req);
		printk (KERN_NOTICE "dir %d sec %lld, nr %ld\n",
                        rq_data_dir(req),
                        pos_sector, num_sector);
		buff = page_address(bvec.bv_page) + bvec.bv_offset;
		block_transfer(s_dev, pos_sector, num_sector,
				buff, rq_data_dir(req) == WRITE);
		pos_sector += num_sector;
	}
	ret = BLK_STS_OK;
exit:
	blk_mq_end_request (req, ret);
	return ret;
}


/*
 * Transfer a single BIO.
 */
static int block_transfer_bio(struct sblock_dev *s_dev, struct bio *bio)
{
	struct bio_vec bvec;
	struct bvec_iter iterate;
	sector_t sec = bio->bi_iter.bi_sector;

	/* Do each segment independently. */
	bio_for_each_segment(bvec, bio, iterate) {
		//char *buff = __bio_kmap_atomic(bio, i, KM_USER0);
		char *buff = kmap_atomic(bvec.bv_page) + bvec.bv_offset;
		//block_transfer(s_dev, sec, bio_cur_bytes(bio) >> 9,
		block_transfer(s_dev, sec, (bio_cur_bytes(bio) / KERNEL_SECTOR_SIZE),
				buff, bio_data_dir(bio) == WRITE);
		//sec += bio_cur_bytes(bio) >> 9;
		sec += (bio_cur_bytes(bio) / KERNEL_SECTOR_SIZE);
		//__bio_kunmap_atomic(buff, KM_USER0);
		kunmap_atomic(buff);
	}
	return 0; /* Always "succeed" */
}

/*
 * Transfer a full request.
 */
static int block_transfer_request(struct sblock_dev *s_dev, struct request *req)
{
	struct bio *bio;
	int n_sec = 0;
    
	__rq_for_each_bio(bio, req) {
		block_transfer_bio(s_dev, bio);
		n_sec += bio->bi_iter.bi_size/KERNEL_SECTOR_SIZE;
	}
	return n_sec;
}


static int block_open(struct block_device *bdev, fmode_t mode)	 
{
	int ret=0;
	printk(KERN_INFO "mydiskdrive : open \n");
	goto out;

	out :
	return ret;

}

static void block_release(struct gendisk *disk, fmode_t mode)
{
	
	printk(KERN_INFO "mydiskdrive : closed \n");

}

static struct block_device_operations fops =
{
	.owner = THIS_MODULE,
	.open = block_open,
	.release = block_release,
};

static struct blk_mq_ops mq_ops_simple = {
    .queue_rq = block_request,
};

static int __init block_init(void)
{
	major_number = register_blkdev(major_number, "dof");
	if (major_number <= 0) {
		printk(KERN_INFO "sbull: unable to get major number\n");
		return -EBUSY;
	}
        struct sblock_dev* s_dev = &device;
	//setup partition table
	device.length = numberofsectors*sector_size;
	device.data_array = vmalloc(device.length);
	copy_mbr(device.data_array);
	spin_lock_init(&device.spinlock);		
	device.queue = blk_mq_init_sq_queue(&device.tag, &mq_ops_simple, 128, BLK_MQ_F_SHOULD_MERGE);
	blk_queue_logical_block_size(device.queue, sector_size);
	(device.queue)->queuedata = s_dev;
	device.gd = alloc_disk(minor_number);
	device.gd->major = major_number;
	device.gd->first_minor = 0;
	device.gd->minors = minor_number;
	device.gd->fops = &fops;
	device.gd->queue = s_dev->queue;
	device.gd->private_data = s_dev;
	sprintf(device.gd->disk_name,"dof");
	set_capacity(device.gd, numberofsectors*(sector_size/KERNEL_SECTOR_SIZE));
	add_disk(device.gd);	    
	return 0;
}

static void block_exit(void)
{
	del_gendisk(device.gd);
	unregister_blkdev(major_number, "mydisk");
	put_disk(device.gd);	
	blk_cleanup_queue(device.queue);
	vfree(device.data_array);
	spin_unlock(&device.spinlock);	
	printk(KERN_ALERT "mydiskdrive is unregistered");
}
	
module_init(block_init);
module_exit(block_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");
MODULE_DESCRIPTION("BLOCK DRIVER");
