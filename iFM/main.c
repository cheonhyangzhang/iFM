//400
#include "iFM.h"
#include "iFM_show.h"
#include "iFM_checksong.h"
#include "iFM_choosesong.h"
#include "iFM_loadsave.h"
#include "iFM_user.h"

int main()
{
    char command[10]; //用来接受命令
//    char username[max_username];//定义用户名
 //   char password[max_username];//定义口令
    char input_dir[max_path];//用来保存用户输入的文件夹绝对路径
    pid_t pid=1; //用来创建第一个子进程
    pid_t pid_2=1;//用来创建第二个子进程
    struct buffer * shmptr;//创建指向缓冲区的指针
    int shmid;
    int i,j;
    int first_play =0;
	int number,templang;

    shmid = shmget(IPC_PRIVATE, sizeof(struct buffer), 0600); // 申请主存共享段  将共享段标志号 赋值给shmid
    shmptr = shmat(shmid, 0, 0);//将共享段 与本进程虚 空间 连接


    printf("iFM 0.1 (c) 2011     Fm FoR U!\n");
    if (load_user(shmptr)==0)
        shmptr->usernames.num_user=0;

    printf("Please choose an language: 1 for English 2 for Chinese.\n");
    printf("请选择语言：1 选择 英语 2 选择 汉语\n");
        scanf("%d",&templang);
    shmptr->language=templang-1;

    while (1)
    {
    if (shmptr->language == 0)
    {
    printf("\n \"l\" for log in.If you have a  account for iFM\n");
    printf("No account for iFM? Tpye \"r\" to register an account\n");
    printf("Or type \"e\" to say goodbye to iFM\niFM:");
    }
    else if (shmptr->language == 1)
    {

    printf("\n输入 \"l\" 登录.如果你有一个iFM的帐户的话\n");
    printf("没有iFM的帐户? 输入 \"r\" 来注册一个帐户吧\n");
    printf("或者输入\"e\" 来和iFM说再见\niFM:");
    }
    scanf("%s",command);
    if (command[0] == 'l')//login
    {
        if (login(shmptr)==1)//登录成功
            {
                break;
            }
        else
            continue;

    }
    else if (command[0] == 'r')//register
    {
        myregister(shmptr);
    }
    else if (command[0] == 'e') //exit
        {
            if (shmptr->usernames.num_user!=0)
                {save_user(shmptr);
                }
            exit(0);
        }
    else
        {
            if (shmptr->language == 0)
                printf("Error!\n");
            else
                printf("错误!\n");

        }
    }//while

    if (load_info(shmptr->current_user,shmptr)==0)
       {
       shmptr->music_box.num_song=0;
       shmptr->music_box.num_like=0;
       shmptr->music_box.num_drop=0;
       shmptr->music_box.sum_like=0;

       }
    if (shmptr->music_box.num_song==0)
        shmptr->music_empty=1; // dangrous  no change to 0
    shmptr->index_last_song=0;

    first_play =1;

    if (shmptr->language == 0)
        printf("See more help? Type \"help\"\n");
    else
        printf("查看更多帮助? 输入 \"help\"\n");
    while (pid>0)//循环 获取用户命令
    {
     if (first_play ==1&&shmptr->music_box.num_song!=0) // 刚打开
        {
	   first_play =0;
         strcpy(command,"on");
        }
     else
        {
	     printf("\n%s\'s iFM:",shmptr->usernames.usernamesdetail[shmptr->current_user].username);
         scanf("%s",command);
        }
     // 帮助信息
     if (strcmp(command,"help") == 0)
        {
         if (shmptr->language == 0)
         {
         printf("\nYou could use iFM as Follows:\n");
         printf("\"setdir\"or\"s\": Tell iFM where is the directory containing music files.\n");
         printf("\"like\"or\"l\": If you like the song you hear.\n");
         printf("\"drop\"or\"d\": If you don't wanna hear the song you hear any more.\n");
         printf("\"next\"or\"n\": iFM will play next song for you.\n");
         printf("\"playlist\"or\"p\": Show what is in the playlis.\n");
         printf("\"exit\"or\"e\": Say goodbye to iFM.\n");
		 printf("\"show-drop\"or\"sd\": list the songs droped by you.\n");
		 printf("\"show-like\"or\"sl\": list the songs you like.\n");
		 printf("\"recover\"or\"re\": recover songs droped by you.\n");
         }
         else
         {
         printf("\n你可以这样使用iFM:\n");
         printf("\"setdir\"或者\"s\": 告诉 iFM 一个之包含音乐文件的文件夹。\n");
         printf("\"like\"或者\"l\": 如果你喜欢这首歌。\n");
         printf("\"drop\"或者\"d\": 如果你再也不想听到这首歌。\n");
         printf("\"next\"或者\"n\": 跳过这首歌\n");
         printf("\"playlist\"或者\"p\": 显示播放列表所有歌曲\n");
         printf("\"exit\"或者\"e\": 和iFM说再见\n");
		 printf("\"show-drop\"或者\"sd\": 显示你喜欢的歌曲列表\n");
		 printf("\"show-like\"或者\"sl\": 显示垃圾桶中的信息\n");
		 printf("\"recover\"或者\"re\": 恢复垃圾桶中的信息\n");
         }
        }
     //开始播放
     else if (strcmp(command,"on")==0)
                {
		if (shmptr->music_box.num_song==0)
                {
                    if (shmptr->language == 0)
                        printf("\nPlease set a dir of music first!\n");
                    else
                        printf("\n请先设置一个文件夹!\n");
                }
             else
                {
		 pid = fork();
                }

            }
     /*
     //停止播放
     else if (strcmp(command,"off")==0)
            {
                    kill(shmptr->pid_temp,SIGKILL);
                    kill(pid,SIGKILL);
            }
     */ // 如果要开启停止播放功能 要修改代码 使在iFM 关闭的时候无能使用除了on的功能
      //喜欢操作
     else if (strcmp(command,"like")==0||strcmp(command,"l")==0)
            {
              if (shmptr->music_box.num_song==0)
	                {
                      if (shmptr->language == 0)
                        printf("\nPlease set a dir of music first!\n");
                      else
                        printf("\n请先设置一个文件夹!\n");
	                }
              else
              {
                if (shmptr->music_box.song_detail[shmptr->index_playing_song].like==1)
                        shmptr->music_box.num_like++;
                if (shmptr->music_box.song_detail[shmptr->index_playing_song].like==5)
                        {
                            if (shmptr->language == 0)
                                printf("Favorite like is 5!You could not make it bigger!\n");
                            else
                                printf("最大喜欢值就是5！你不能把它变得更大哦！\n");
                        }
                else
                {
                shmptr->music_box.song_detail[shmptr->index_playing_song].like+=1;//修改喜好度
                shmptr->music_box.sum_like++;
                if (shmptr->language == 0)
                    printf("The evaluation of this song by you has changed to %d\n",shmptr->music_box.song_detail[shmptr->index_playing_song].like);
                else
                    printf("歌曲的评价值已经变成 %d\n",shmptr->music_box.song_detail[shmptr->index_playing_song].like);
                }
              }
            }
     //丢入垃圾桶
     else if (strcmp(command,"drop")==0||strcmp(command,"d")==0)
            {
                if (shmptr->music_box.num_song==0)
	                {
                      if (shmptr->language == 0)
                        printf("\nPlease set a dir of music first!\n");
                      else
                        printf("\n请先设置一个文件夹!\n");
	                }
                else
                {
                if (shmptr->music_box.song_detail[shmptr->index_playing_song].like>1)
                    shmptr->music_box.num_like--;
                shmptr->music_box.song_detail[shmptr->index_playing_song].like=0;//修改喜好度
                shmptr->music_box.sum_like-=shmptr->music_box.song_detail[shmptr->index_playing_song].like;

                shmptr->music_box.num_drop++;
                if (shmptr->language == 0)
                    printf("Song %s has been dropped!You will never hear it any more!\n",shmptr->music_box.song_detail[shmptr->index_playing_song].song_name);
                else
                    printf("歌曲 %s 已经被丢入垃圾箱!你不需要再忍受这首歌了哦!\n",shmptr->music_box.song_detail[shmptr->index_playing_song].song_name);
                kill(shmptr->pid_temp,SIGKILL);
                kill(pid,SIGKILL);
                pid = fork();
                }
            }
     //下一首歌曲
     else if (strcmp(command,"next")==0||strcmp(command,"n")==0)
            {
            if (shmptr->music_box.num_song==0)
                {
                 if (shmptr->language == 0)
                        printf("\nPlease set a dir of music first!\n");
                 else
                        printf("\n请先设置一个文件夹!\n");
                }
            else
            {
             kill(shmptr->pid_temp,SIGKILL);
             kill(pid,SIGKILL);
             pid = fork();
            }
            }
     //定义文件夹
     else if (strcmp(command,"setdir")==0||strcmp(command,"s")==0)
            {
             if (shmptr->language == 0)
                printf("Please enter the dir path of music:");
             else
                printf("请输入文件夹的绝对路径：");
             scanf("%s",input_dir);
             if (shmptr->music_box.num_song!=0)
                {
                    for (j=0;j<shmptr->music_box.num_song;j++)
                        {
                            shmptr->music_box.song_detail[j].repeat=0;
                        }
                }
             check_song(input_dir,shmptr);
            }
     //查看播放列表
     else if (strcmp(command,"playlist")==0||strcmp(command,"p")==0)
			{
                if (shmptr->music_box.num_song==0)
                {
                 if (shmptr->language == 0)
                      printf("\nPlease set a dir of music first!\n");
                 else
                      printf("\n请先设置一个文件夹!\n");
                }
               else
                {
                if (shmptr->language == 0)
                    {
                    printf("There are %d songs in the list:\n",shmptr->music_box.num_song);
                    printf("%d songs you like.\n",shmptr->music_box.num_like);
                    printf("%d songs you drop.\n",shmptr->music_box.num_drop);
                    }
                else
                    {
                    printf("在列表中有 %d 首歌:\n",shmptr->music_box.num_song);
                    printf("%d 首歌你喜欢.\n",shmptr->music_box.num_like);
                    printf("%d 首哥被放入垃圾箱.\n",shmptr->music_box.num_drop);
                    }
                for (i=0;i<shmptr->music_box.num_song;i++)
                    {
                     if (shmptr->language == 0)
                        printf("\n%d: %s   Like: %d\n",i+1,shmptr->music_box.song_detail[i].song_name,shmptr->music_box.song_detail[i].like);
                     else
                        printf("\n%d: %s   喜欢程度: %d\n",i+1,shmptr->music_box.song_detail[i].song_name,shmptr->music_box.song_detail[i].like);
                    }
                }
            }
     //退出程序
     else if (strcmp(command,"exit")==0||strcmp(command,"e")==0)
            {
                kill(shmptr->pid_temp,SIGKILL);
                kill(pid,SIGKILL);
                save_user(shmptr);
                save_info(shmptr->current_user,shmptr);
                exit(0);
            }
	 //查看垃圾箱
	 else if (strcmp(command,"show-drop")==0||strcmp(command,"sd")==0)
			{
			    if (shmptr->music_box.num_song==0)
                {
                 if (shmptr->language == 0)
                      printf("\nPlease set a dir of music first!\n");
                 else
                      printf("\n请先设置一个文件夹!\n");
                }
				show_drop(shmptr);
			}
	 //查看喜欢的音乐
	 else if (strcmp(command,"show-like")==0||strcmp(command,"sl")==0)
			{
			    if (shmptr->music_box.num_song==0)
                {
                 if (shmptr->language == 0)
                      printf("\nPlease set a dir of music first!\n");
                 else
                      printf("\n请先设置一个文件夹!\n");
                }
				show_like(shmptr);
			}
	 else if (strcmp(command,"recover")==0||strcmp(command,"re")==0)
			{
			    if (shmptr->music_box.num_song==0)
                {
                 if (shmptr->language == 0)
                      printf("\nPlease set a dir of music first!\n");
                 else
                      printf("\n请先设置一个文件夹!\n");
                }
				if (shmptr->language == 0 )
					printf("please enter the number of song listed by \"sl\".enter 0 to recover all songs.\n");
				else
					printf("请输入由命令\"sl\"列出的编号。输入0恢复所有歌曲。\n");
				scanf("%d",&number);
				antidrop(number,shmptr);
			}
     //错误信息
     else
        {
            if (shmptr->language == 0)
                printf("Wrong use! Please type \"help\" for more about how to use iFM\n");
            else
                printf("错误使用! 请输入\"help\" 了来了解更多如何使用iFM\n");
        }
    }

    if (pid>0)//主进程 不再需要代码
        {
        }
    else if (pid == 0)//子进程

        {
         while (pid_2>0)// 循环创建子进程
         {
         pid_2=fork();
         if (pid_2>0)
            {
             shmptr = shmat(shmid, 0, 0); //挂接共享段
             shmptr->pid_temp = pid_2;//将播放音乐的 子进程ID号写入共享区
             shmdt(shmptr);//脱离共享内存块
             wait(0);

            }
         else if (pid_2 == 0)
            {
		shmptr = shmat(shmid, 0, 0); //挂接共享段
             do
              {
		shmptr->index_playing_song=choose_song(shmptr);
              }
             while (shmptr->index_playing_song==shmptr->index_last_song);
             shmptr->index_last_song=shmptr->index_playing_song;
             if (shmptr->language == 0)
             {
                printf("\nPlaying %s\n",shmptr->music_box.song_detail[shmptr->index_playing_song].song_name);
                printf("The evaluation is %d\n:",shmptr->music_box.song_detail[shmptr->index_playing_song].like);
             }
             else
             {
                printf("\n正在播放 %s\n",shmptr->music_box.song_detail[shmptr->index_playing_song].song_name);
                printf("喜欢程度 %d\n:",shmptr->music_box.song_detail[shmptr->index_playing_song].like);
             }
             execl("/usr/bin/xterm","xterm","-e","mplayer",shmptr->music_box.song_detail[shmptr->index_playing_song].song_path,NULL);//
             shmdt(shmptr);//脱离共享内存块
             exit(0);
            }
         }


         exit(0);
        }
    shmdt(shmptr);//脱离共享内存块
    return 0;
}
