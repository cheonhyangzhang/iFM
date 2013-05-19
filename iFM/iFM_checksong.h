//129
#include "iFM.h"
//对音乐库进行操作 code by daimili
void update(char* spath, char* dname,struct buffer * shmptr)
{
	int i,flag=0,j=0;
	for (j=0;;j++)
                {
             if (dname[j]=='\0')
                 break;
                }
     for (;j>0;j--)
                {
                    if (dname[j]=='.')
                        {
                            dname[j]='\0';
                            break;
                        }

                }
	if(shmptr->music_empty==0)		//不为第一次进入，主函数中判断，全局变量传递
	{
		for(i=0;i<shmptr->music_box.num_song;i++)
		{
			if(strcmp(spath,shmptr->music_box.song_detail[i].song_path)==0)
			{
				shmptr->music_box.song_detail[i].repeat=1;
				flag++;
				break;
			}
		}
		if(!flag)
		{
			strcpy(shmptr->music_box.song_detail[shmptr->music_box.num_song].song_path,spath);
			strcpy(shmptr->music_box.song_detail[shmptr->music_box.num_song].song_name,dname);
			shmptr->music_box.song_detail[shmptr->music_box.num_song].like=1;
			shmptr->music_box.song_detail[shmptr->music_box.num_song++].repeat=1;
		}
	}
	else	//第一次进入
	{
            strcpy(shmptr->music_box.song_detail[shmptr->music_box.num_song].song_path,spath);
			strcpy(shmptr->music_box.song_detail[shmptr->music_box.num_song].song_name,dname);
			shmptr->music_box.song_detail[shmptr->music_box.num_song].like=1;
			shmptr->music_box.song_detail[shmptr->music_box.num_song++].repeat=1;

	}
}

void regulate(struct buffer * shmptr)
{
	int i,j,num_modify=0;
	j=shmptr->music_box.num_song;
	for(i=0;i<shmptr->music_box.num_song;i++)
	{
		if(shmptr->music_box.song_detail[i].repeat==0)
		{
			for(;j>i;j--)
			{
				if(shmptr->music_box.song_detail[j].repeat==1)
				{
					strcpy(shmptr->music_box.song_detail[i].song_path,shmptr->music_box.song_detail[j].song_path);
					strcpy(shmptr->music_box.song_detail[i].song_name,shmptr->music_box.song_detail[j].song_name);
					shmptr->music_box.song_detail[i].repeat=shmptr->music_box.song_detail[j].repeat;
					shmptr->music_box.song_detail[i].like=shmptr->music_box.song_detail[j].like;
					num_modify++;
				}
			}
			if(j<=i)
				break;
		}
	}
	shmptr->music_box.num_song-=num_modify;
}

void sum_up(struct buffer * shmptr)
{
    int i=0;
    shmptr->music_box.sum_like=0;

    for(i=0;i<shmptr->music_box.num_song;i++)
    {
            shmptr->music_box.sum_like+=shmptr->music_box.song_detail[i].like;
        }
}

void check_song(char dirpath[],struct buffer * shmptr)
{
    struct dirent* ent = NULL;
    char spath[MAX_PATH] = "";
    DIR *pDir;
    struct stat file_stat ;
    strcpy(spath,dirpath) ;
    pDir=opendir(dirpath);
    if (pDir == NULL)
    {
		if(shmptr -> language == 0)
			printf("Please enter a correct path!\n");
		else
			printf("请输入正确路径!\n");
		return;
    }
    while (NULL != (ent=readdir(pDir)))
    {
    	if(strcmp(ent->d_name,"..")==0||strcmp(ent->d_name,".")==0)
			continue ;
        if (ent->d_type == 4)
        {
                strcat(spath,"/");
                strcat(spath,ent->d_name) ;
                stat(spath,&file_stat);
                check_song(spath,shmptr);
                strcpy(spath,dirpath) ;
         }
         else
         {
                strcat(spath,"/");
                strcat(spath,ent->d_name) ;
                update(spath,ent->d_name,shmptr);
                if (shmptr->music_empty==1)
                    shmptr->music_empty=0;
                strcpy(spath,dirpath);
         }
    }
	regulate(shmptr);
    sum_up(shmptr);
}

//对音乐库操作结束 end code by daimili
