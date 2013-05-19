//197
#include "iFM.h"
//读写用户偏好信息部分代码
int transition_char_int(char *temp) //转换char型变量到int型
{
	int i,lenth,num=0;
	lenth=strlen(temp);
	for(i=lenth-1;i>=0;i--)
		num+=((int)((temp[i]-'0'))*pow(10,lenth-i-1));
	return num;
}

int load_info(int index_user,struct buffer * shmptr)//读取用户偏好信息
{
	char ch,fname[max_path],temp[max_path];
	int i,j;
	getcwd(fname,max_path);
	FILE *fp;
	strcat(fname,"/iFMconfig");
	strcat(fname,shmptr->usernames.usernamesdetail[index_user].username);
	fp=fopen(fname,"r");
	if(fp==NULL)
	{
		return 0;
	}
	if((ch=fgetc(fp))!=EOF)
	{
		i=0;
		temp[i++]=ch;
		while((ch=fgetc(fp))!='\n')
		{
			temp[i++]=ch;
		}
		temp[i]='\0';
            shmptr->music_box.num_song=transition_char_int(temp);
		i=0;
		while((ch=fgetc(fp))!='\n')
		{
			temp[i++]=ch;
		}
		temp[i]='\0';
        	shmptr->music_box.num_like=transition_char_int(temp);
		i=0;
		while((ch=fgetc(fp))!='\n')
		{
			temp[i++]=ch;
		}
		temp[i]='\0';
        	shmptr->music_box.num_drop=transition_char_int(temp);
		i=0;
		while((ch=fgetc(fp))!='\n')
		{
			temp[i++]=ch;
		}
		temp[i]='\0';
        	shmptr->music_box.sum_like=transition_char_int(temp);
		for(i=0;i<shmptr->music_box.num_song;i++)
		{
			j=0;
			while((ch=fgetc(fp))!='\n')
				{
					shmptr->music_box.song_detail[i].song_name[j++]=ch;
				}
			shmptr->music_box.song_detail[i].song_name[j]='\0';
			j=0;
			while((ch=fgetc(fp))!='\n')
				{
					shmptr->music_box.song_detail[i].song_path[j++]=ch;
				}
			shmptr->music_box.song_detail[i].song_path[j]='\0';
			j=0;
			while((ch=fgetc(fp))!='\n')
			{
				temp[j++]=ch;
			}
			temp[j]='\0';
			shmptr->music_box.song_detail[i].like=transition_char_int(temp);
			j=0;
			while((ch=fgetc(fp))!='\n')
			{
				temp[j++]=ch;
			}
			temp[j]='\0';
			shmptr->music_box.song_detail[i].repeat=transition_char_int(temp);

		}
	}
	return 1;
}

char* transition_int_char(int num)//转换int型变量到char型
{
	int i,j,k=0;
	int flag=0;
	char * temp;
	temp = (char *)malloc(40);
	if (num == 0)
	 {temp[0]='0';
	  temp[1]='\0';
	     }
    	else
    	{
		for(i=5;i>=0;i--)
		{
		j=(int)(num/pow(10,i));
		num=num-j*pow(10,i);
		if(j||flag==1)
        	{
             flag=1;
            temp[k++]=j+'0';
        	}
        }
	temp[k]='\0';
    }
	return temp;
}

int save_info(int index_user,struct buffer * shmptr)//存储用户信息
{
	char ch,fname[max_path],temp[max_path];
	int i,j,num;
	getcwd(fname,max_path);
	FILE *fp;
	strcat(fname,"/iFMconfig");
	strcat(fname,shmptr->usernames.usernamesdetail[index_user].username);
	fp=fopen(fname,"w");
	if(fp==NULL)
	{
		return 0;
	}

	num=shmptr->music_box.num_song;
	strcpy(temp,transition_int_char(num));
	i=0;
	while(temp[i]!='\0')
	{
		fputc(temp[i++],fp);
	}
	fputc('\n',fp);
	num=shmptr->music_box.num_like;
	strcpy(temp,transition_int_char(num));
	i=0;
	while(temp[i]!='\0')
	{
		fputc(temp[i++],fp);
	}
		fputc('\n',fp);
	num=shmptr->music_box.num_drop;
	strcpy(temp,transition_int_char(num));
	i=0;
	while(temp[i]!='\0')
	{
		fputc(temp[i++],fp);
	}
		fputc('\n',fp);
	num=shmptr->music_box.sum_like;
	strcpy(temp,transition_int_char(num));
	i=0;
	while(temp[i]!='\0')
	{
		fputc(temp[i++],fp);
	}
		fputc('\n',fp);
	for(i=0;i<shmptr->music_box.num_song;i++)
	{
		j=0;
		while((ch=shmptr->music_box.song_detail[i].song_name[j++])!='\0')
		{
			fputc(ch,fp);
		}
		fputc('\n',fp);
		j=0;
		while((ch=shmptr->music_box.song_detail[i].song_path[j++])!='\0')
		{
			fputc(ch,fp);
		}
		fputc('\n',fp);
		num=shmptr->music_box.song_detail[i].like;
		strcpy(temp,transition_int_char(num));
		j=0;
		while(temp[j]!='\0')
		{
			fputc(temp[j++],fp);
		}
		fputc('\n',fp);
		num=shmptr->music_box.song_detail[i].repeat;
		strcpy(temp,transition_int_char(num));
		j=0;
		while(temp[j]!='\0')
		{
			fputc(temp[j++],fp);
		}
		fputc('\n',fp);
	}
	return 1;
}
//end 读写用户偏好信息
