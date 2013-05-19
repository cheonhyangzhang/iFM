//103
#ifndef IFM_H_
#define IFM_H_

#include <stdio.h>
#include "sys/ipc.h"
#include <stdlib.h>
#include "string.h"
#include "sys/types.h"
#include "unistd.h"
#include "signal.h"
#include "sys/shm.h"
#include "dirent.h"
#include "sys/stat.h"
#include "time.h"
#include "math.h"
#include "sys/wait.h"
#include "termios.h"

#define max_song 1000 //定义最多可存放的歌曲数量
#define max_song_name 100 //定义最长歌曲名字
#define max_path 1024 //定义可以处理的最长路径
#define MAX_PATH 1024
#define max_username 30 //定义最长用户名
#define max_user 30 //定义最大用户数量

// 歌曲信息结构
typedef struct song_info{
	char song_name[max_song_name];//歌曲名字
	char song_path[max_path];//歌曲绝对路径
	int like;//歌曲喜好程度
	int repeat;// 用来判断更新信息
}SONG_INFO;

// 音乐库结构
typedef struct music_store{
    int num_song;//歌曲总数
	int num_like;//喜欢的歌曲数量  // 未 搞定
	int num_drop;//被放入垃圾堆中的文件
	int sum_like;// 所有喜欢的歌曲的like的和
	SONG_INFO song_detail[max_song];
}MUSIC_STORE;

//用户信息结构
typedef struct user_info{
	char username[max_username];
	char password[max_username];
}USER_INFO;

typedef struct usernamesinfo{
    USER_INFO usernamesdetail[max_user]; //存储用户信息
    int num_user;
}USERNAMESINFO;

struct buffer
{pid_t pid_temp;//用来记录启动播放 音乐的进程的ID号
 MUSIC_STORE music_box;//定义音乐库  为了便于各个函数 使用和修改 音乐库信息 定义为全局变量
 int index_playing_song;// 播放的歌曲在歌曲库中的索引值
 int index_last_song; //定义上一次播放过的音乐
 int music_empty;//音乐库是否为空
 USERNAMESINFO usernames;  // 信息初始化问题？？？？？？？？？
 int current_user;//当前用户标识
 int language;//程序语言
};


//字符型转化为整型
int transition_char_int(char *);

//加载用户信息
int load_info(int ,struct buffer *);

//整型转化为字符型
char* transition_int_char(int);


//保存用户信息
int save_info(int ,struct buffer *);

//随机选择歌曲
int choose_song(struct buffer *);

//更新音乐库信息
void update(char*, char*,struct buffer *);

//整理音乐库，删除垃圾信息
void regulate(struct buffer *);

//计算总喜好度
void sum_up(struct buffer *);

//搜索音乐文件夹
void check_song(char *,struct buffer *);

//查看垃圾箱中的音乐
void show_drop(struct buffer *);

//查看喜欢的音乐列表
void show_like(struct buffer *);

//恢复垃圾箱中的音乐
void antidrop(int,struct buffer *);

#endif
