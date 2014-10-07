/*
 * Copyright 2000 by Hans Reiser, licensing governed by reiserfs/README
 */

/*
 * Written by Alexander Zarochentcev.
 *
 * The kernel part of the (on-line) reiserfs resizer.
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/errno.h>
#include "reiserfs.h"
#include <linux/buffer_head.h>

int reiserfs_resize(struct super_block *s, unsigned long block_count_new)
{
	int err = 0;
	struct reiserfs_super_block *sb;
	struct reiserfs_bitmap_info *bitmap;
	struct reiserfs_bitmap_info *info;
	struct reiserfs_bitmap_info *old_bitmap = SB_AP_BITMAP(s);
	struct buffer_head *bh;
	struct reiserfs_transaction_handle th;
	unsigned int bmap_nr_new, bmap_nr;
	unsigned int block_r_new, block_r;

	struct reiserfs_list_bitmap *jb;
	struct reiserfs_list_bitmap jbitmap[JOURNAL_NUM_BITMAPS];

	unsigned long int block_count, free_blocks;
	int i;
	int copy_size;

	sb = SB_DISK_SUPER_BLOCK(s);

	if (SB_BLOCK_COUNT(s) >= block_count_new) {
		printk("can\'t shrink filesystem on-line\n");
		return -EINVAL;
	}

	
	bh = sb_bread(s, block_count_new - 1);
	if (!bh) {
		printk("reiserfs_resize: can\'t read last block\n");
		return -EINVAL;
	}
	bforget(bh);

	if (SB_BUFFER_WITH_SB(s)->b_blocknr * SB_BUFFER_WITH_SB(s)->b_size
	    != REISERFS_DISK_OFFSET_IN_BYTES) {
		printk
		    ("reiserfs_resize: unable to resize a reiserfs without distributed bitmap (fs version < 3.5.12)\n");
		return -ENOTSUPP;
	}

	
	block_r = SB_BLOCK_COUNT(s) -
			(reiserfs_bmap_count(s) - 1) * s->s_blocksize * 8;

	
	bmap_nr_new = block_count_new / (s->s_blocksize * 8);
	block_r_new = block_count_new - bmap_nr_new * s->s_blocksize * 8;
	if (block_r_new)
		bmap_nr_new++;
	else
		block_r_new = s->s_blocksize * 8;

	
	block_count = SB_BLOCK_COUNT(s);
	bmap_nr = reiserfs_bmap_count(s);

	
	if (bmap_nr_new > bmap_nr) {
		
		if (reiserfs_allocate_list_bitmaps(s, jbitmap, bmap_nr_new) < 0) {
			printk
			    ("reiserfs_resize: unable to allocate memory for journal bitmaps\n");
			return -ENOMEM;
		}
		copy_size = bmap_nr_new < bmap_nr ? bmap_nr_new : bmap_nr;
		copy_size =
		    copy_size * sizeof(struct reiserfs_list_bitmap_node *);
		for (i = 0; i < JOURNAL_NUM_BITMAPS; i++) {
			struct reiserfs_bitmap_node **node_tmp;
			jb = SB_JOURNAL(s)->j_list_bitmap + i;
			memcpy(jbitmap[i].bitmaps, jb->bitmaps, copy_size);

			node_tmp = jb->bitmaps;
			jb->bitmaps = jbitmap[i].bitmaps;
			vfree(node_tmp);
		}

		bitmap =
		    vzalloc(sizeof(struct reiserfs_bitmap_info) * bmap_nr_new);
		if (!bitmap) {
			printk("reiserfs_resize: unable to allocate memory.\n");
			return -ENOMEM;
		}
		for (i = 0; i < bmap_nr; i++)
			bitmap[i] = old_bitmap[i];

		for (i = bmap_nr; i < bmap_nr_new; i++) {
			bh = sb_bread(s, i * s->s_blocksize * 8);
			if (!bh) {
				vfree(bitmap);
				return -EIO;
			}
			memset(bh->b_data, 0, sb_blocksize(sb));
			reiserfs_set_le_bit(0, bh->b_data);
			reiserfs_cache_bitmap_metadata(s, bh, bitmap + i);

			set_buffer_uptodate(bh);
			mark_buffer_dirty(bh);
			reiserfs_write_unlock(s);
			sync_dirty_buffer(bh);
			reiserfs_write_lock(s);
			
			bitmap[i].free_count = sb_blocksize(sb) * 8 - 1;
			brelse(bh);
		}
		
		SB_AP_BITMAP(s) = bitmap;
		vfree(old_bitmap);
	}

	err = journal_begin(&th, s, 10);
	if (err)
		return err;

	
	info = SB_AP_BITMAP(s) + bmap_nr - 1;
	bh = reiserfs_read_bitmap_block(s, bmap_nr - 1);
	if (!bh) {
		int jerr = journal_end(&th, s, 10);
		if (jerr)
			return jerr;
		return -EIO;
	}

	reiserfs_prepare_for_journal(s, bh, 1);
	for (i = block_r; i < s->s_blocksize * 8; i++)
		reiserfs_clear_le_bit(i, bh->b_data);
	info->free_count += s->s_blocksize * 8 - block_r;

	journal_mark_dirty(&th, s, bh);
	brelse(bh);

	
	info = SB_AP_BITMAP(s) + bmap_nr_new - 1;
	bh = reiserfs_read_bitmap_block(s, bmap_nr_new - 1);
	if (!bh) {
		int jerr = journal_end(&th, s, 10);
		if (jerr)
			return jerr;
		return -EIO;
	}

	reiserfs_prepare_for_journal(s, bh, 1);
	for (i = block_r_new; i < s->s_blocksize * 8; i++)
		reiserfs_set_le_bit(i, bh->b_data);
	journal_mark_dirty(&th, s, bh);
	brelse(bh);

	info->free_count -= s->s_blocksize * 8 - block_r_new;
	
	reiserfs_prepare_for_journal(s, SB_BUFFER_WITH_SB(s), 1);
	free_blocks = SB_FREE_BLOCKS(s);
	PUT_SB_FREE_BLOCKS(s,
			   free_blocks + (block_count_new - block_count -
					  (bmap_nr_new - bmap_nr)));
	PUT_SB_BLOCK_COUNT(s, block_count_new);
	PUT_SB_BMAP_NR(s, bmap_would_wrap(bmap_nr_new) ? : bmap_nr_new);
	s->s_dirt = 1;

	journal_mark_dirty(&th, s, SB_BUFFER_WITH_SB(s));

	SB_JOURNAL(s)->j_must_wait = 1;
	return journal_end(&th, s, 10);
}
