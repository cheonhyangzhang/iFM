//300
#include "iFM.h"

char* mymakecode(char src[])
{
	int i,lenth;
	char temp[100],*ret;
	strcpy(temp,src);
	ret=temp;
	lenth=strlen(ret);
	for(i=0;i<lenth/3;i++)
	{
		ret[i]+=1;
	}
	for(i=lenth/3;i<(lenth*2)/3;i++)
	{
		ret[i]+=2;
	}
	for(i=(lenth*2)/3;i<lenth;i++)
	{
		ret[i]+=3;
	}
	return ret;
}

char* mydemakecode(char src[])
{
	int i,lenth;
	char temp[100],*ret;
	strcpy(temp,src);
	ret=temp;
	lenth=strlen(ret);
	for(i=0;i<lenth/3;i++)
	{
		ret[i]-=1;
	}
	for(i=lenth/3;i<(lenth*2)/3;i++)
	{
		ret[i]-=2;
	}
	for(i=(lenth*2)/3;i<lenth;i++)
	{
		ret[i]-=3;
	}
	return ret;
}

//不回显的getch方法
int mygetch(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr( STDIN_FILENO, &oldt ); // 记录旧设置
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt ); // 设置新的
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt ); // 设置旧的

	return ch;
}

int login(struct buffer * shmptr)//登录用户 成功返回1 否则返回0
{
    int i=0,find=0;
	int len = 0;
	char c;
	char username[max_username];
	char password[max_username];

	if(shmptr -> language == 0)
		printf("Username:");
	else
		printf("用户名：");
	scanf("%s",username);
	c = getchar();
	if(shmptr -> language == 0)
		printf("Password:");
	else
		printf("密码：");

	while(1)
	{
		c = mygetch();
		if(c == '\n')
		{
			password[len] = '\0';
			printf("\n");
			break;
		}
		password[len++] = c;
		printf("*");    // 每读入一个字符输出一个*
	}

	//printf("%s\n",password);

    for (i=0;i<shmptr->usernames.num_user;i++)
        {
            if (strcmp(shmptr->usernames.usernamesdetail[i].username,username)==0)
                {
                    shmptr->current_user=i;
                    find=1;
                    break;
                }
            else
                continue;
        }

    if (find ==1 )
        {
            if (strcmp(shmptr->usernames.usernamesdetail[i].password,password)==0)
                {
                    shmptr->current_user=i;
                    find =1;
                    return 1;
                }
            else
                {
					if(shmptr -> language == 0)
					{
						printf("Wrong passwords!!!\n");
						printf("Please check your passwords!\n");
					}
					else
					{
						printf("密码错误！！！\n");
						printf("请检查你的密码！\n");
					}
                 return 0;
                }

        }
    else
        {
			if(shmptr -> language == 0)
			{
				printf("No such account has been registered!\n");
				printf("Please check your username!\n");
			}
			else
			{
				printf("用户名不存在！\n");
				printf("请检查你的用户名！\n");
			}
            return 0;
        }
}




int myregister(struct buffer * shmptr) //注册用户 成功返回1 否则返回0
{
	int len = 0;
	char c;
	char username[max_username];
	char password[max_username];

	if(shmptr -> language == 0)
		printf("Username:");
	else
		printf("用户名：");
	scanf("%s",username);
	c = getchar();
	if(shmptr -> language == 0)
		printf("Password:");
	else
		printf("密码：");

	while(1)
	{
		c = mygetch();
		if(c == '\n')
		{
			password[len] = '\0';
			printf("\n");
			break;
		}
		password[len++] = c;
		printf("*");    // 每读入一个字符输出一个*
}

    if (shmptr->usernames.num_user==max_user)//超出用户数量
        {
			if(shmptr -> language == 0)
	            printf("The number of users has reached max value!\n");
			else
				printf("注册用户数已满！\n");
            return 0;
        }
    else
        {
            strcpy(shmptr->usernames.usernamesdetail[shmptr->usernames.num_user].username,username);
            strcpy(shmptr->usernames.usernamesdetail[shmptr->usernames.num_user].password,password);
            shmptr->usernames.num_user++;
			if(shmptr -> language == 0)
			{
				printf("A account named %s has been created!\n",username);
				printf("Please remmember your password!\n");
			}
			else
			{
				printf("创建用户: %s\n",username);
				printf("请记住你的密码！\n");
			}
            return 1;
        }

}

int save_user(struct buffer * shmptr)//存储用户名和密码
{
	char ch;
	char fname[max_path];
	int i,j;
	char * forcode;
	getcwd(fname,max_path);
	FILE *fp;
	for (i=0;i<shmptr->usernames.num_user;i++)
        {
            forcode=mymakecode(shmptr->usernames.usernamesdetail[i].username);
            strcpy(shmptr->usernames.usernamesdetail[i].username,forcode);
            forcode=mymakecode(shmptr->usernames.usernamesdetail[i].password);
            strcpy(shmptr->usernames.usernamesdetail[i].password,forcode);
        }
	strcat(fname,"/iFMusers");
	fp=fopen(fname,"w");
	if(fp==NULL)
	{
		return 0;
	}
	fputc(shmptr->usernames.num_user+'0',fp);
    fputc('\n',fp);

	for(i=0;i<shmptr->usernames.num_user;i++)
	{

		j=0;
		while((ch=shmptr->usernames.usernamesdetail[i].username[j++])!='\0')
		{
			fputc(ch,fp);
		}
		fputc('\n',fp);

        forcode=mydemakecode(shmptr->usernames.usernamesdetail[i].username);
        strcpy(shmptr->usernames.usernamesdetail[i].username,forcode);

		j=0;
		while((ch=shmptr->usernames.usernamesdetail[i].password[j++])!='\0')
		{
			fputc(ch,fp);
		}
		fputc('\n',fp);
	}
	return 1;
}
int load_user(struct buffer * shmptr)//读取用户偏好信息
{
	char ch;
	char fname[max_path];
	int i,j;
	char * forcode;
	getcwd(fname,max_path);
	FILE *fp;
	strcat(fname,"/iFMusers");
	fp=fopen(fname,"r");
	if(fp==NULL)
	{
		return 0;
	}
	if((ch=fgetc(fp))!=EOF)
	{
	    shmptr->usernames.num_user=ch-'0';
	    fgetc(fp);
		for(i=0;i<shmptr->usernames.num_user;i++)
		{
			j=0;
			while((ch=fgetc(fp))!='\n')
				{
					shmptr->usernames.usernamesdetail[i].username[j++]=ch;
				}
			shmptr->usernames.usernamesdetail[i].username[j]='\0';
			j=0;
			while((ch=fgetc(fp))!='\n')
				{
					shmptr->usernames.usernamesdetail[i].password[j++]=ch;
				}
			shmptr->usernames.usernamesdetail[i].password[j]='\0';

		}

	}
    for (i=0;i<shmptr->usernames.num_user;i++)
        {
            forcode=mydemakecode(shmptr->usernames.usernamesdetail[i].username);
            strcpy(shmptr->usernames.usernamesdetail[i].username,forcode);
            forcode=mydemakecode(shmptr->usernames.usernamesdetail[i].password);
            strcpy(shmptr->usernames.usernamesdetail[i].password,forcode);
        }
	return 1;
}



