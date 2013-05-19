//176
#include "iFM.h"
//code by laoseng
//查看垃圾箱中的音乐
void show_drop(struct buffer * shmptr)
{
	int drop_number;	//歌曲编号
	int i;

	drop_number = 1;

	if(shmptr -> music_box.num_drop == 0)
	{
		if(shmptr -> language == 0)
			printf("There are no songs droped by you!\n");
		else
			printf("垃圾箱中没有歌曲！\n");
		return;
	}
	else
	{
		if(shmptr -> language == 0)
		{
			printf("There are %d songs droped by you!\n",shmptr -> music_box.num_drop);
			printf("ID \t Name\n");
		}
		else
		{
			printf("垃圾箱中有%d首歌曲！\n",shmptr -> music_box.num_drop);
			printf("编号 \t 歌曲名称\n");
		}
	}

	for( i = 0; i < shmptr -> music_box.num_song; i ++ )
		if(shmptr -> music_box.song_detail[i].like == 0)
		{
			printf("%d \t %s\n",drop_number,shmptr -> music_box.song_detail[i].song_name);
			drop_number ++;
		}
	if(shmptr -> language == 0)
		printf("list completed.\n");
	else
		printf("列表完成.\n");

	return;
}

//查看喜欢的歌曲列表，按喜好程序排序
void show_like(struct buffer * shmptr)
{
	int like,id;
	int i;

	like = 5;

	if(shmptr -> music_box.num_like == 0)
	{
		if(shmptr -> language == 0)
			printf("There are no songs you like!\n");
		else
			printf("没有你喜欢的歌曲！\n");
		return;
	}
	else
	{
		if(shmptr -> language == 0)
			printf("There are %d songs you like!\n",shmptr -> music_box.num_like);
		else
			printf("总共有%d首你喜欢的歌曲！\n",shmptr -> music_box.num_like);
	}
	while(like > 1)
	{
		id = 1;
		if(shmptr -> language == 0)
		{
			printf("Songs with the evaluation of %d:\n",like);
			printf("ID \t Name\n");
		}
		else
		{
			printf("评价为%d的歌曲如下：\n",like);
			printf("编号 \t 歌曲名称\n");
		}
		for(i = 0; i < shmptr->music_box.num_song; i ++)
		{
			if(shmptr -> music_box.song_detail[i].like == like)
			{
				printf("%d \t %s\n",id,shmptr -> music_box.song_detail[i].song_name);
				id ++;
			}
			else
				continue;
		}
		if(shmptr -> language == 0)
			printf("%d songs in total.\n",id - 1);
		else
			printf("总共有%d首.\n",id - 1);
		like --;
	}
	if(shmptr -> language == 0)
		printf("list completed.\n");
	else
		printf("列表完成.\n");

	return;
}

//恢复垃圾箱中的音乐，配合show_drop使用
void antidrop(int id,struct buffer * shmptr)
{

	int drop_number;	//歌曲编号
	int i;

	drop_number = 0;

	if(shmptr -> music_box.num_drop == 0)
	{
		if(shmptr -> language == 0)
			printf("There are no songs droped by you!\n");
		else
			printf("垃圾箱中没有歌曲！\n");
		return;
	}
	if(id > shmptr -> music_box.num_drop || id < 0)
	{
		if(shmptr -> language == 0)
		{
			printf("Input error!\n");
			printf("Try the ingeter below %d.\n",shmptr -> music_box.num_drop);
		}
		else
		{
			printf("错误编号！\n");
			printf("请输入0或小于%d的正整数。\n",shmptr -> music_box.num_drop);
		}
		return;
	}
	if(id == 0)	//还原所有歌曲
	{
		for(i = 0;i < shmptr -> music_box.num_song;i ++)
			if(shmptr -> music_box.song_detail[i].like == 0)
				shmptr -> music_box.song_detail[i].like = 1;
		shmptr -> music_box.sum_like += shmptr -> music_box.num_drop;
		shmptr -> music_box.num_drop = 0;
		if(shmptr -> language == 0)
			printf("all the songs that had been droped are recovered.\n");
		else
			printf("垃圾箱中的所有歌曲已恢复。\n");
		return;
	}
	else
	{
		for(i = 0;i < shmptr -> music_box.num_song;i ++)
		{
			if(shmptr -> music_box.song_detail[i].like == 0)
				drop_number ++;
			if(drop_number == id)
			{
				shmptr -> music_box.song_detail[i].like = 1;
				shmptr -> music_box.sum_like += 1;
				shmptr -> music_box.num_drop -= 1;
				if(shmptr -> language == 0)
					printf("Song [%s] is recovered.\n",shmptr -> music_box.song_detail[i].song_name);
				else
					printf("歌曲 【%s】 已恢复。\n",shmptr -> music_box.song_detail[i].song_name);
				break;
			}
		}
		return;
	}

	return;
}

//code end by laoseng
