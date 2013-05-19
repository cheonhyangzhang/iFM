//27
#include "iFM.h"
// 随机选择歌曲 code by laoseng
int choose_song(struct buffer * shmptr)
{
	int random;
	int temp_sum_like;
	time_t t;
	int i;

	srand((unsigned)time(&t));
	random = rand()%shmptr->music_box.sum_like;

	temp_sum_like = 0;
	for(i = 0;i < shmptr->music_box.num_song;i ++)
	{
	    if(shmptr->music_box.song_detail[i].like == 0)
                continue;
		temp_sum_like += shmptr->music_box.song_detail[i].like;
		if(temp_sum_like > random)
		{
			return i;
		}
	}
	return 0;
}
//  code end by laoseng
