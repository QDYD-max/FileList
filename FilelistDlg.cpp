// FilelistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Filelist.h"
#include "FilelistDlg.h"
#include "FileOpen.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#define BLOCKSIZE 1024  // 磁盘块大小  
#define SIZE 1024000  // 虚拟磁盘空间大小  
#define END 65535  // FAT中的文件结束标志  
#define FREE 0  // FAT中盘块空闲标志  
#define ROOTBLOCKNUM 2  // 根目录区所占盘块数  
#define MAXOPENFILE 10  // 最多同时打开文件个数t  
#define MAXTEXT 10000  


#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilelistDlg dialog

/* 文件控制块 */  
typedef struct FCB  
{  
	char filename[8];  // 文件名  
	char exname[3];  // 文件扩展名  
	unsigned char attribute;  // 文件属性字段，值为0时表示目录文件，值为1时表示数据文件  
	unsigned short time;  // 文件创建时间  
	unsigned short date;  // 文件创建日期  
	unsigned short first;  // 文件起始盘块号  
	unsigned long length;  // 文件长度  
	char free;  // 表示目录项是否为空，若值为0，表示空，值为1，表示已分配  
}fcb;  

/* 文件分配表 */  
typedef struct FAT  
{  
    unsigned short id;  // 磁盘块的状态（空闲的，最后的，下一个）  
}fat;  
/* 用户打开文件表 */  
typedef struct USEROPEN  
{  
	char filename[8];  // 文件名  
	char exname[3];  // 文件扩展名  
	unsigned char attribute;  // 文件属性字段，值为0时表示目录文件，值为1时表示数据文件  
	unsigned short time;  // 文件创建时间  
	unsigned short date;  // 文件创建日期  
	unsigned short first;  // 文件起始盘块号  
	unsigned long length;  // 文件长度（对数据文件是字节数，对目录文件可以是目录项个数）  
	char free;  // 表示目录项是否为空，若值为0，表示空，值为1，表示已分配  
	
	unsigned short dirno;  // 相应打开文件的目录项在父目录文件中的盘块号  
	int diroff;  // 相应打开文件的目录项在父目录文件的dirno盘块中的目录项序号  
	char dir[80];  // 相应打开文件所在的目录名，这样方便快速检查出指定文件是否已经打开  
	int father;  // 父目录在打开文件表项的位置  
	int count;  // 读写指针在文件中的位置,文件的总字符数  
	char fcbstate;  // 是否修改了文件的FCB的内容，如果修改了置为1，否则为0  
	char topenfile;  // 表示该用户打开表项是否为空，若值为0，表示为空，否则表示已被某打开文件占据  
}useropen;  

/* 引导块 */  
typedef struct BLOCK0  
{  
    char magic[10];  // 文件系统魔数  
    char information[200];  // 存储一些描述信息，如磁盘块大小、磁盘块数量、最多打开文件数等  
    unsigned short root;  // 根目录文件的起始盘块号  
    unsigned char *startblock;  // 虚拟磁盘上数据区开始位置  
}block0;  


unsigned char *myvhard;  // 指向虚拟磁盘的起始地址  
useropen openfilelist[MAXOPENFILE];  // 用户打开文件表数组  
int curdir;  // 用户打开文件表中的当前目录所在打开文件表项的位置  
char currentdir[80];  // 记录当前目录的目录名（包括目录的路径）  
unsigned char* startp;  // 记录虚拟磁盘上数据区开始位置  
char myfilename[] = "myfilesys";//文件系统的文件名 

CString title;

void CFilelistDlg::startsys()  
{  
    FILE *fp;  
    unsigned char buf[SIZE];  
    fcb *root;  
    int i;  
    myvhard = (unsigned char *)malloc(SIZE);//申请虚拟磁盘空间  
    memset(myvhard, 0, SIZE);//将myvhard中前SIZE个字节用 0 替换并返回 myvhard  
    if((fp = fopen(myfilename, "r")) != NULL)  
    {  
        fread(buf, SIZE, 1, fp);//将二进制文件读取到缓冲区  
        fclose(fp);//关闭打开的文件，缓冲区数据写入文件，释放系统提供文件资源  
        if(strcmp(((block0 *)buf)->magic, "10101010"))//判断开始的8个字节内容是否为文件系统魔数  
        {  
            printf("myfilesys is not exist,begin to creat the file...\n");  
            my_format();  
        }  
        else  
        {  
            for(i = 0; i < SIZE; i++)  
                myvhard[i] = buf[i];  
        }  
    }  
    else  
    {  
        printf("myfilesys is not exist,begin to creat the file...\n");  
        my_format();  
    }  
    root = (fcb *)(myvhard + 5 * BLOCKSIZE);  
    strcpy(openfilelist[0].filename, root->filename);  
    strcpy(openfilelist[0].exname, root->exname);  
    openfilelist[0].attribute = root->attribute;  
    openfilelist[0].time = root->time;  
    openfilelist[0].date = root->date;  
    openfilelist[0].first = root->first;  
    openfilelist[0].length = root->length;  
    openfilelist[0].free = root->free;  
    openfilelist[0].dirno = 5;  
    openfilelist[0].diroff = 0;  
    strcpy(openfilelist[0].dir, "\\root\\");  
    openfilelist[0].father = 0;  
    openfilelist[0].count = 0;  
    openfilelist[0].fcbstate = 0;  
    openfilelist[0].topenfile = 1;  
    for(i = 1; i < MAXOPENFILE; i++)  
        openfilelist[i].topenfile = 0;  
    curdir = 0;  
    strcpy(currentdir, "\\root\\");  
    startp = ((block0 *)myvhard)->startblock;  
}  

void CFilelistDlg::my_format()  
{  
    FILE *fp;  
    fat *fat1, *fat2;  
    block0 *blk0;  
    time_t now;  
    struct tm *nowtime;  
    fcb *root;  
    int i;  
    blk0 = (block0 *)myvhard;  
    fat1 = (fat *)(myvhard + BLOCKSIZE);  
    fat2 = (fat *)(myvhard + 3 * BLOCKSIZE);  
    root = (fcb *)(myvhard + 5 * BLOCKSIZE);  
    strcpy(blk0->magic, "10101010");  
    strcpy(blk0->information, "My FileSystem Ver 1.0 \n Blocksize=1KB Whole size=1000KB Blocknum=1000 RootBlocknum=2\n");  
    blk0->root = 5;  
    blk0->startblock = (unsigned char *)root;  
    for(i = 0; i < 5; i++)  
    {  
        fat1->id = END;  
        fat2->id = END;  
        fat1++;  
        fat2++;  
    }  
    fat1->id = 6;  
    fat2->id = 6;  
    fat1++;  
    fat2++;  
    fat1->id = END;  
    fat2->id = END;  
    fat1++;  
    fat2++;  
    for(i = 7; i < SIZE / BLOCKSIZE; i++)  
    {  
        fat1->id = FREE;  
        fat2->id = FREE;  
        fat1++;  
        fat2++;  
    }  
    now = time(NULL);  
    nowtime = localtime(&now);  
    strcpy(root->filename, ".");  
    strcpy(root->exname, "");  
    root->attribute = 0x28;  
    root->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;  
    root->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;  
    root->first = 5;  
    root->length = 2 * sizeof(fcb);  
    root->free = 1;  
    root++;  
    now = time(NULL);  
    nowtime = localtime(&now);  
    strcpy(root->filename, "..");  
    strcpy(root->exname, "");  
    root->attribute = 0x28;  
    root->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;  
    root->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;  
    root->first = 5;  
    root->length = 2 * sizeof(fcb);  
    root->free = 1;  
    fp = fopen(myfilename, "w");  
    fwrite(myvhard, SIZE, 1, fp);  
    fclose(fp);  
}  
void CFilelistDlg::my_cd(char *dirname)  
{  
    char *dir;  
    int fd;  
    dir = strtok(dirname, "\\");//分解字符串为一组字符串。dirname为要分解的字符串，"\\"为分隔符字符串   
    if(strcmp(dir, "..") == 0)  
    {  
        if(curdir)  
            curdir = my_close(curdir);  
        return;  
    }  
    else if(strcmp(dir, "root") == 0)  
    {  
        while(curdir)  
            curdir = my_close(curdir);  
        dir = strtok(NULL, "\\");  
    }  
    while(dir)  
    {  
        fd = my_open(dir);  
        if(fd != -1)  
            curdir = fd;  
        else  
            return;  
        dir = strtok(NULL, "\\");  
    }  
}  
void CFilelistDlg::my_mkdir(char *dirname)  
{  
    fcb *fcbptr;  
    fat *fat1, *fat2;  
    time_t now;  
    struct tm *nowtime;  
    char text[MAXTEXT];  
    unsigned short blkno;  
    int rbn, fd, i;  
    fat1 = (fat *)(myvhard + BLOCKSIZE);  
    fat2 = (fat *)(myvhard + 3 * BLOCKSIZE);  
    openfilelist[curdir].count = 0;  
    rbn = do_read(curdir, openfilelist[curdir].length, text);  
    fcbptr = (fcb *)text;  
    for(i = 0; i < rbn / sizeof(fcb); i++)//在当前目录下找，是否有重名目录  
    {  
        if(strcmp(fcbptr->filename, dirname) == 0 && strcmp(fcbptr->exname, "") == 0)  
        {  
            MessageBox("Error,the dirname is already exist!\n");  
            return;  
        }  
        fcbptr++;  
    }  
    fcbptr = (fcb *)text;  
    for(i = 0; i < rbn / sizeof(fcb); i++)  
    {  
        if(fcbptr->free == 0)  
            break;  
        fcbptr++;  
    }  
    blkno = findblock();//寻找空闲盘块  
    if(blkno == -1)  
        return;  
    (fat1 + blkno)->id = END;  
    (fat2 + blkno)->id = END;  
    now = time(NULL);  
    nowtime = localtime(&now);  
    strcpy(fcbptr->filename, dirname);  
    strcpy(fcbptr->exname, "");  
    fcbptr->attribute = 0x30;  
    fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;  
    fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;  
    fcbptr->first = blkno;  
    fcbptr->length = 2 * sizeof(fcb);  
    fcbptr->free = 1;  
    openfilelist[curdir].count = i * sizeof(fcb);  
    do_write(curdir, (char *)fcbptr, sizeof(fcb), 2);  
	
    fd = my_open(dirname);//建立新目录的'.','..'目录  
    if(fd == -1)  
        return;  
    fcbptr = (fcb *)malloc(sizeof(fcb));  
    now = time(NULL);  
    nowtime = localtime(&now);  
    strcpy(fcbptr->filename, ".");  
    strcpy(fcbptr->exname, "");  
    fcbptr->attribute = 0x28;  
    fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;  
    fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;  
    fcbptr->first = blkno;  
    fcbptr->length = 2 * sizeof(fcb);  
    fcbptr->free = 1;  
    do_write(fd, (char *)fcbptr, sizeof(fcb), 2);  
    now = time(NULL);  
    nowtime = localtime(&now);  
    strcpy(fcbptr->filename, "..");  
    strcpy(fcbptr->exname, "");  
    fcbptr->attribute = 0x28;  
    fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;  
    fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;  
    fcbptr->first = blkno;  
    fcbptr->length = 2 * sizeof(fcb);  
    fcbptr->free = 1;  
    do_write(fd, (char *)fcbptr, sizeof(fcb), 2);  
    free(fcbptr);  
    my_close(fd);  
	
    fcbptr = (fcb *)text;  
    fcbptr->length = openfilelist[curdir].length;  
    openfilelist[curdir].count = 0;  
    do_write(curdir, (char *)fcbptr, sizeof(fcb), 2);  
    openfilelist[curdir].fcbstate = 1;  
}  

void CFilelistDlg::my_rmdir(char* dirname)
{
    fcb* fcbptr, * fcbptr2;
    fat* fat1, * fat2, * fatptr1, * fatptr2;
    char text[MAXTEXT], text2[MAXTEXT];
    unsigned short blkno;
    int rbn, rbn2, fd, i, j;
    fat1 = (fat*)(myvhard + BLOCKSIZE);
    fat2 = (fat*)(myvhard + 3 * BLOCKSIZE);
    if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
    {
        printf("Error,can't remove this directory.\n");
        return;
    }
    openfilelist[curdir].count = 0;
    rbn = do_read(curdir, openfilelist[curdir].length, text);
    fcbptr = (fcb*)text;
    for (i = 0; i < rbn / sizeof(fcb); i++)//查找要删除的目录  
    {
        if (strcmp(fcbptr->filename, dirname) == 0 && strcmp(fcbptr->exname, "") == 0)
            break;
        fcbptr++;
    }
    if (i == rbn / sizeof(fcb))
    {
        printf("Error,the directory is not exist.\n");
        return;
    }
    fd = my_open(dirname);
    rbn2 = do_read(fd, openfilelist[fd].length, text2);
    fcbptr2 = (fcb*)text2;
    for (j = 0; j < rbn2 / sizeof(fcb); j++)//判断要删除目录是否为空  
    {
        if (strcmp(fcbptr2->filename, ".") && strcmp(fcbptr2->filename, "..") && strcmp(fcbptr2->filename, ""))
        {
            /*my_close(fd);
            printf("Error,the directory is not empty.\n"); 
            return;*/
			//MessageBox("Error,the directory is not empty.\n");  
			my_close(fd);  
			my_cd(dirname);
			my_ls();
			//CString iii;
			//iii.Format("%d",m_local_list.GetItemCount());
			//MessageBox(iii);
			for(i = 2; i < m_local_list.GetItemCount() ; i++)
			{
			if (strcmp(m_local_list.GetItemText(i, 2),"文件夹") == 0)
			{
			my_rmdir(m_local_list.GetItemText(i, 0).GetBuffer(0));
			my_ls();
			}
			else
			{
			my_rm(m_local_list.GetItemText(i, 0).GetBuffer(0));
			my_ls();
			}
			//iii.Format("%d",m_local_list.GetItemCount());
			//MessageBox(iii);
			}
			my_cd("..");
			my_rmdir(dirname);
			return;
        }
        fcbptr2++;
    }
    blkno = openfilelist[fd].first;
    while (blkno != END)
    {
        fatptr1 = fat1 + blkno;
        fatptr2 = fat2 + blkno;
        blkno = fatptr1->id;
        fatptr1->id = FREE;
        fatptr2->id = FREE;
    }
    my_close(fd);
    strcpy(fcbptr->filename, "");
    fcbptr->free = 0;
    openfilelist[curdir].count = i * sizeof(fcb);
    do_write(curdir, (char*)fcbptr, sizeof(fcb), 2);
    openfilelist[curdir].fcbstate = 1;
}

void CFilelistDlg::my_ls()  
{  
    fcb *fcbptr;  
    char text[MAXTEXT]; 
	char tempstr[256];
	CString buff;
	//CString testString;
	//CString tmpString;
    int rbn, i;  
	int iItem; 
    openfilelist[curdir].count = 0;  
    rbn = do_read(curdir, openfilelist[curdir].length, text);  
    fcbptr = (fcb *)text;  
	m_local_list.DeleteAllItems();
    for(i = 0; i < rbn / sizeof(fcb); i++)  
    {  
        if(fcbptr->free == 1)  
        {  
            if(fcbptr->attribute & 0x20)
			{
				if(strcmp(fcbptr->filename , ".")==0)
				{
					LV_ITEM lvitem;
					memset((char *)&lvitem, '\0', sizeof(LV_ITEM));
					lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
					lvitem.iItem = i;
					lvitem.iImage = 2;
					lvitem.iSubItem = 0;
					lvitem.stateMask = 0;
					iItem = m_local_list.InsertItem(&lvitem);
					m_local_list.SetItemText(i, 0,_T("root"));
					m_local_list.SetItemData(iItem, 1);
					m_local_list.SetItemText(i, 2,_T("（返回根目录）")); 
				}
				else if(strcmp(fcbptr->filename , "..")==0)
				{
					LV_ITEM lvitem;
					memset((char *)&lvitem, '\0', sizeof(LV_ITEM));
					lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
					lvitem.iItem = i;
					lvitem.iImage = 2;
					lvitem.iSubItem = 0;
					lvitem.stateMask = 0;
					iItem = m_local_list.InsertItem(&lvitem);
					m_local_list.SetItemText(i, 0,_T(fcbptr->filename));
					m_local_list.SetItemData(iItem, 1);
					m_local_list.SetItemText(i, 2,_T("（返回上级目录）")); 
				}
				else
				{	
					LV_ITEM lvitem;
					memset((char *)&lvitem, '\0', sizeof(LV_ITEM));
					lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
					lvitem.iItem = i;
					lvitem.iImage = 1;
					lvitem.iSubItem = 0;
					lvitem.stateMask = 0;
					iItem = m_local_list.InsertItem(&lvitem);
					//iItem = m_local_list.InsertItem(i, _T(fcbptr->filename));
					//MessageBox(fcbptr->filename);//逆子
					m_local_list.SetItemText(i, 0,_T(fcbptr->filename));
					m_local_list.SetItemData(iItem, 1);
					m_local_list.SetItemText(i, 2,_T("文件夹")); 
					buff.Format("%d/%d/%d",(fcbptr->date >> 9) + 1980, (fcbptr->date >> 5) & 0x000f, fcbptr->date & 0x001f);
					m_local_list.SetItemText(i, 3,buff);  
					buff.Format("%02d:%02d:%02d",fcbptr->time >> 11, (fcbptr->time >> 5) & 0x003f, fcbptr->time & 0x001f * 2);
					m_local_list.SetItemText(i, 4, buff); 
					//printf("%s\\\t\t<DIR>\t\t%d/%d/%d\t%02d:%02d:%02d\n", fcbptr->filename, (fcbptr->date >> 9) + 1980, (fcbptr->date >> 5) & 0x000f, fcbptr->date & 0x001f, fcbptr->time >> 11, (fcbptr->time >> 5) & 0x003f, fcbptr->time & 0x001f * 2);  
				}
				
			}
            else  
			{
				LV_ITEM lvitem;
				memset((char *)&lvitem, '\0', sizeof(LV_ITEM));
				lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
				lvitem.iItem = i;
				lvitem.iImage = 0;
				lvitem.iSubItem = 0;
				lvitem.stateMask = 0;
				iItem = m_local_list.InsertItem(&lvitem);
				buff.Format("%s.%s",fcbptr->filename, fcbptr->exname);
                //iItem = m_local_list.InsertItem(i, buff);
				m_local_list.SetItemText(i,0, buff);
				m_local_list.SetItemData(iItem, 1);
				buff.Format("%dB",(int)(fcbptr->length));
				m_local_list.SetItemText(i, 1, buff);
				buff.Format("%s文件",fcbptr->exname);
				m_local_list.SetItemText(i, 2,buff);
				buff.Format("%d/%d/%d",(fcbptr->date >> 9) + 1980, (fcbptr->date >> 5) & 0x000f, fcbptr->date & 0x001f);
				m_local_list.SetItemText(i, 3,buff);  
				buff.Format("%02d:%02d:%02d",fcbptr->time >> 11, (fcbptr->time >> 5) & 0x003f, fcbptr->time & 0x001f * 2);
				m_local_list.SetItemText(i, 4, buff); 
				//printf("%s.%s\t\t%dB\t\t%d/%d/%d\t%02d:%02d:%02d\t\n", fcbptr->filename, fcbptr->exname, (int)(fcbptr->length), (fcbptr->date >> 9) + 1980, (fcbptr->date >> 5) & 0x000f, fcbptr->date & 0x1f, fcbptr->time >> 11, (fcbptr->time >> 5) & 0x3f, fcbptr->time & 0x1f * 2);  
				
			}
			//tmpString.Format(" ,%s",fcbptr->filename);
			//testString = testString + tmpString;
		}
        fcbptr++;  
    }  
	//MessageBox(testString);
}  
void CFilelistDlg::my_create(char *filename)  
{  
    fcb *fcbptr;  
    fat *fat1, *fat2;  
    char *fname, *exname, text[MAXTEXT];  
    unsigned short blkno;  
    int rbn, i;  
    time_t now;  
    struct tm *nowtime;  
    fat1 = (fat *)(myvhard + BLOCKSIZE);  
    fat2 = (fat *)(myvhard + BLOCKSIZE);  
    fname = strtok(filename, ".");  
    exname = strtok(NULL, ".");  
    if(strcmp(fname, "") == 0)  
    {  
        MessageBox("Error,creating file must have a right name.\n");  
        return;  
    }  
    if(!exname)  
    {  
        MessageBox("Error,creating file must have a extern name.\n");  
        return;  
    }  
    openfilelist[curdir].count = 0;  
    rbn = do_read(curdir, openfilelist[curdir].length, text);  
    fcbptr = (fcb *)text;  
    for(i = 0; i < rbn / sizeof(fcb); i++)  
    {  
        if(strcmp(fcbptr->filename, fname) == 0 && strcmp(fcbptr->exname, exname) == 0)  
        {  
            MessageBox("Error,the filename is already exist!\n");  
            return;  
        }  
        fcbptr++;  
    }  
    fcbptr = (fcb *)text;  
    for(i = 0; i < rbn / sizeof(fcb); i++)  
    {  
        if(fcbptr->free == 0)  
            break;  
        fcbptr++;  
    }  
    blkno = findblock();  
    if(blkno == -1)  
        return;  
    (fat1 + blkno)->id = END;  
    (fat2 + blkno)->id = END;  
	
    now = time(NULL);  
    nowtime = localtime(&now);  
    strcpy(fcbptr->filename, fname);  
    strcpy(fcbptr->exname, exname);  
    fcbptr->attribute = 0x00;  
    fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;  
    fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;  
    fcbptr->first = blkno;  
    fcbptr->length = 0;  
    fcbptr->free = 1;  
    openfilelist[curdir].count = i * sizeof(fcb);  
    do_write(curdir, (char *)fcbptr, sizeof(fcb), 2);  
    fcbptr = (fcb *)text;  
    fcbptr->length = openfilelist[curdir].length;  
    openfilelist[curdir].count = 0;  
    do_write(curdir, (char *)fcbptr, sizeof(fcb), 2);  
    openfilelist[curdir].fcbstate = 1;  
}  
void CFilelistDlg::my_rm(char *filename)  
{  
    fcb *fcbptr;  
    fat *fat1, *fat2, *fatptr1, *fatptr2;  
    char *fname, *exname, text[MAXTEXT];  
    unsigned short blkno;  
    int rbn, i;  
    fat1 = (fat *)(myvhard + BLOCKSIZE);  
    fat2 = (fat *)(myvhard + 3 * BLOCKSIZE);  
    fname = strtok(filename, ".");  
    exname = strtok(NULL, ".");  
    if(strcmp(fname, "") == 0)  
    {  
        printf("Error,removing file must have a right name.\n");  
        return;  
    }  
    if(!exname)  
    {  
        printf("Error,removing file must have a extern name.\n");  
        return;  
    }  
    openfilelist[curdir].count = 0;  
    rbn = do_read(curdir, openfilelist[curdir].length, text);  
    fcbptr = (fcb *)text;  
    for(i = 0; i < rbn / sizeof(fcb); i++)  
    {  
        if(strcmp(fcbptr->filename, fname) == 0 && strcmp(fcbptr->exname, exname) == 0)  
            break;  
        fcbptr++;  
    }  
    if(i == rbn / sizeof(fcb))  
    {  
        printf("Error,the file is not exist.\n");  
        return;  
    }  
    openfilelist[curdir].count = 0;  
    rbn = do_read(curdir, openfilelist[curdir].length, text);  
    fcbptr = (fcb *)text;  
    for(i = 0; i < rbn / sizeof(fcb); i++)  
    {  
        if(strcmp(fcbptr->filename, fname) == 0 && strcmp(fcbptr->exname, exname) == 0)  
            break;  
        fcbptr++;  
    }  
    if(i == rbn / sizeof(fcb))  
    {  
        printf("Error,the file is not exist.\n");  
        return;  
    }  
    blkno = fcbptr->first;  
    while(blkno != END)  
    {  
        fatptr1 = fat1 + blkno;  
        fatptr2 = fat2 + blkno;  
        blkno = fatptr1->id;  
        fatptr1->id = FREE;  
        fatptr2->id = FREE;  
    }  
    strcpy(fcbptr->filename, "");  
    fcbptr->free = 0;  
    openfilelist[curdir].count = i * sizeof(fcb);  
    do_write(curdir, (char *)fcbptr, sizeof(fcb), 2);  
    openfilelist[curdir].fcbstate = 1;  
}  
int CFilelistDlg::my_open(char *filename)  
{  
    fcb *fcbptr;  
    char *fname, exname[3], *str, text[MAXTEXT];  
    int rbn, fd, i;  
    fname = strtok(filename, ".");  
    str = strtok(NULL, ".");  
    if(str)  
        strcpy(exname, str);  
    else  
        strcpy(exname, "");  
    for(i = 0; i < MAXOPENFILE; i++)  
    {  
        if(strcmp(openfilelist[i].filename, fname) == 0 && strcmp(openfilelist[i].exname, exname) == 0 && i != curdir)  
        {  
            printf("Error,the file is already open.\n");  
            return -1;  
        }  
    }  
    openfilelist[curdir].count = 0;  
    rbn = do_read(curdir, openfilelist[curdir].length, text);  
    fcbptr = (fcb *)text;  
    for(i = 0; i < rbn / sizeof(fcb); i++)  
    {  
        if(strcmp(fcbptr->filename, fname) == 0 && strcmp(fcbptr->exname, exname) == 0)  
            break;  
        fcbptr++;  
    }  
    if(i == rbn / sizeof(fcb))  
    {  
        printf("Error,the file is not exist.\n");  
        return -1;  
    }  
    fd = findopenfile();  
    if(fd == -1)  
        return -1;  
    strcpy(openfilelist[fd].filename, fcbptr->filename);  
    strcpy(openfilelist[fd].exname, fcbptr->exname);  
    openfilelist[fd].attribute = fcbptr->attribute;  
    openfilelist[fd].time = fcbptr->time;  
    openfilelist[fd].date = fcbptr->date;  
    openfilelist[fd].first = fcbptr->first;  
    openfilelist[fd].length = fcbptr->length;  
    openfilelist[fd].free = fcbptr->free;  
    openfilelist[fd].dirno = openfilelist[curdir].first;  
    openfilelist[fd].diroff = i;  
    strcpy(openfilelist[fd].dir, openfilelist[curdir].dir);  
    strcat(openfilelist[fd].dir, filename);  
    if(fcbptr->attribute & 0x20)  
        strcat(openfilelist[fd].dir, "\\");  
    openfilelist[fd].father = curdir;  
    openfilelist[fd].count = 0;  
    openfilelist[fd].fcbstate = 0;  
    openfilelist[fd].topenfile = 1;  
    return fd;  
}  
int CFilelistDlg::my_close(int fd)  
{  
    fcb *fcbptr;  
    int father = openfilelist[fd].father; 
    if(fd < 0 || fd >= MAXOPENFILE)  
    {  
        printf("Error,the file is not exist.\n");  
        return -1;  
    }  
    if(openfilelist[fd].fcbstate)  
    {  
        fcbptr = (fcb *)malloc(sizeof(fcb));  
        strcpy(fcbptr->filename, openfilelist[fd].filename);  
        strcpy(fcbptr->exname, openfilelist[fd].exname);  
        fcbptr->attribute = openfilelist[fd].attribute;  
        fcbptr->time = openfilelist[fd].time;  
        fcbptr->date = openfilelist[fd].date;  
        fcbptr->first = openfilelist[fd].first;  
        fcbptr->length = openfilelist[fd].length;  
        fcbptr->free = openfilelist[fd].free;  
        father = openfilelist[fd].father;  
        openfilelist[father].count = openfilelist[fd].diroff * sizeof(fcb);  
        do_write(father, (char *)fcbptr, sizeof(fcb), 2);  
        free(fcbptr);  
        openfilelist[fd].fcbstate = 0;  
    }  
    strcpy(openfilelist[fd].filename, "");  
    strcpy(openfilelist[fd].exname, "");  
    openfilelist[fd].topenfile = 0;  
    return father;  
}  
int CFilelistDlg::my_write(int fd)  
{  
    fat *fat1, *fat2, *fatptr1, *fatptr2;  
    int wstyle, len, ll, tmp;  
    char text[MAXTEXT];  
    unsigned short blkno;  
    fat1 = (fat *)(myvhard + BLOCKSIZE);  
    fat2 = (fat *)(myvhard + 3 * BLOCKSIZE);  
    if(fd < 0 || fd >= MAXOPENFILE)  
    {  
        MessageBox("The file is not exist!\n");  
        return -1;  
    }  
    /*while(1)  
    {  
	printf("Please enter the number of write style:\n1.cut write\t2.cover write\t3.add write\n");  
	scanf("%d", &wstyle);  
	if(wstyle > 0 && wstyle < 4)  
	break;  
	printf("Input Error!");  
} */ 
    wstyle = 1;  
    switch(wstyle)  
    {  
	case 1:  
		blkno = openfilelist[fd].first;  
		fatptr1 = fat1 + blkno;  
		fatptr2 = fat2 + blkno;  
		blkno = fatptr1->id;  
		fatptr1->id = END;  
		fatptr2->id = END;  
		while(blkno != END)  
		{  
			fatptr1 = fat1 + blkno;  
			fatptr2 = fat2 + blkno;  
			blkno = fatptr1->id;  
			fatptr1->id = FREE;  
			fatptr2->id = FREE;  
		}  
		openfilelist[fd].count = 0;  
		openfilelist[fd].length = 0;  
		break;  
	case 2:  
		openfilelist[fd].count = 0;  
		break;  
	case 3:  
		openfilelist[fd].count = openfilelist[fd].length;  
		break;  
	default:  
		break;  
    }  
    ll = 0;  
    //printf("please input write data(end with Ctrl+Z):\n"); 
	CFilelistApp *app = (CFilelistApp *)AfxGetApp(); 
	strcpy(text,app->tmptext);
	if(strcmp(text,""))
    {  
        len = strlen(text);  
        //text[len++] = '\n';  
        //text[len] = '\0';  
        tmp = do_write(fd, text, len, wstyle);  
        if(tmp != -1)  
            ll += tmp;  
        if(tmp < len)  
        {  
            printf("Wirte Error!");  
            return ll;  
        }
		//MessageBox(text);
    }  
    return ll;  
}  

int CFilelistDlg::do_write(int fd, char *text, int len, char wstyle)  
{  
    fat *fat1, *fat2, *fatptr1, *fatptr2;  
    unsigned char *buf, *blkptr;  
    unsigned short blkno, blkoff;  
    int i, ll;  
    fat1 = (fat *)(myvhard + BLOCKSIZE);  
    fat2 = (fat *)(myvhard + 3 * BLOCKSIZE);  
    buf = (unsigned char *)malloc(BLOCKSIZE);  
    if(buf == NULL)  
    {  
        printf("malloc failed!\n");  
        return -1;  
    }  
    blkno = openfilelist[fd].first;  
    blkoff = openfilelist[fd].count;  
    fatptr1 = fat1 + blkno;  
    fatptr2 = fat2 + blkno;  
    while(blkoff >= BLOCKSIZE)  
    {  
        blkno = fatptr1->id;  
        if(blkno == END)  
        {  
            blkno = findblock();  
            if(blkno == -1)  
            {  
                free(buf);  
                return -1;  
            }  
            fatptr1->id = blkno;  
            fatptr2->id = blkno;  
            fatptr1 = fat1 + blkno;  
            fatptr2 = fat2 + blkno;  
            fatptr1->id = END;  
            fatptr2->id = END;  
        }  
        else  
        {  
            fatptr1 = fat1 + blkno;  
            fatptr2 = fat2 + blkno;  
        }  
        blkoff = blkoff - BLOCKSIZE;  
    }  
	
    ll = 0;  
    while(ll < len)  
    {  
        blkptr = (unsigned char *)(myvhard + blkno * BLOCKSIZE);  
        for(i = 0; i < BLOCKSIZE; i++)  
            buf[i] = blkptr[i];  
        for(;blkoff < BLOCKSIZE; blkoff++)  
        {  
            buf[blkoff] = text[ll++];  
            openfilelist[fd].count++;  
            if(ll == len)  
                break;  
        }  
        for(i = 0; i < BLOCKSIZE; i++)  
            blkptr[i] = buf[i];  
        if(ll < len)  
        {  
            blkno = fatptr1->id;  
            if(blkno == END)  
            {  
                blkno = findblock();  
                if(blkno == -1)  
                    break;  
                fatptr1->id = blkno;  
                fatptr2->id = blkno;  
                fatptr1 = fat1 + blkno;  
                fatptr2 = fat2 + blkno;  
                fatptr1->id = END;  
                fatptr2->id = END;  
            }  
            else  
            {  
                fatptr1 = fat1 + blkno;  
                fatptr2 = fat2 + blkno;  
            }  
            blkoff = 0;  
		}  
    }  
    if(openfilelist[fd].count > openfilelist[fd].length)  
        openfilelist[fd].length = openfilelist[fd].count;  
    openfilelist[fd].fcbstate = 1;  
    free(buf);  
    return ll;  
}  
int CFilelistDlg::do_read(int fd, int len, char *text)  
{  
    fat *fat1, *fatptr;  
    unsigned char *buf, *blkptr;  
    unsigned short blkno, blkoff;  
    int i, ll;  
    fat1 = (fat *)(myvhard + BLOCKSIZE);  
    buf = (unsigned char *)malloc(BLOCKSIZE);  
    if(buf == NULL)  
    {  
        printf("malloc failed!\n");  
        return -1;  
    }  
    blkno = openfilelist[fd].first;  
    blkoff = openfilelist[fd].count;  
    if(blkoff >= openfilelist[fd].length)  
    {  
		//MessageBox("Read out of range!");
        //puts("Read out of range!"); 
        free(buf);  
        return -1;  
    }  
    fatptr = fat1 + blkno;  
    while(blkoff >= BLOCKSIZE)  
    {  
        blkno = fatptr->id;  
        blkoff = blkoff - BLOCKSIZE;  
        fatptr = fat1 + blkno;  
    }  
    ll = 0;  
    while(ll < len)  
    {  
        blkptr = (unsigned char *)(myvhard + blkno * BLOCKSIZE);  
        for(i = 0; i < BLOCKSIZE; i++)  
            buf[i] = blkptr[i];  
        for(; blkoff < BLOCKSIZE; blkoff++)  
        {  
            text[ll++] = buf[blkoff];  
            openfilelist[fd].count++;  
            if(ll == len || openfilelist[fd].count == openfilelist[fd].length)  
                break;  
        }  
        if(ll < len && openfilelist[fd].count != openfilelist[fd].length)  
        {  
            blkno = fatptr->id;  
            if(blkno == END)  
                break;  
            blkoff = 0;  
            fatptr = fat1 + blkno;  
        }  
    }  
    text[ll] = '\0';  
    free(buf);  
    return ll;  
}
int CFilelistDlg::my_read(int fd, int len)  
{  
    char text[MAXTEXT];  
    int ll;  
    if(fd < 0 || fd >= MAXOPENFILE)  
    {  
        MessageBox("The File is not exist!\n");  
        return -1;  
    }  
    openfilelist[fd].count = 0;  
    ll = do_read(fd, len, text);  
    if(ll != -1) 
	{
		//printf("%s", text); 
		CFilelistApp *app = (CFilelistApp *)AfxGetApp(); 
		strcpy(app->tmptext,text);
	}
    else  
		//MessageBox("Read Error!");
        printf("Read Error!\n");  
    return ll;  
}  
void CFilelistDlg::my_exitsys()  
{  
    FILE *fp;  
    while(curdir)  
        curdir = my_close(curdir);  
    fp = fopen(myfilename, "w");  
    fwrite(myvhard, SIZE, 1, fp);  
    fclose(fp);  
    free(myvhard);  
}  
unsigned short CFilelistDlg::findblock()  
{  
    unsigned short i;  
    fat *fat1, *fatptr;  
    fat1 = (fat *)(myvhard + BLOCKSIZE);  
    for(i = 7; i < SIZE / BLOCKSIZE; i++)  
    {  
        fatptr = fat1 + i;  
        if(fatptr->id == FREE)  
            return i;  
    }  
    printf("Error,Can't find free block!\n");  
    return -1;  
}  

int CFilelistDlg::findopenfile()  
{  
    int i;  
    for(i = 0; i < MAXTEXT; i++)  
    {  
        if(openfilelist[i].topenfile == 0)  
            return i;  
    }  
    printf("Error,open too many files!\n");  
    return -1;  
}


CFilelistDlg::CFilelistDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFilelistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilelistDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



void CFilelistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilelistDlg)
	DDX_Control(pDX, IDC_LIST1, m_local_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilelistDlg, CDialog)
//{{AFX_MSG_MAP(CFilelistDlg)
ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
ON_COMMAND(ID_ROPEN, OnRopen)
ON_COMMAND(ID_RDELETE, OnRdelete)
ON_MESSAGE(NUM_CREATEFILE,createFile)
ON_MESSAGE(NUM_CREATEFOLDER,createFolder)
ON_MESSAGE(NUM_SAVEFILE,saveFile)
ON_MESSAGE(NUM_CLOSE,closeFile)
ON_WM_CLOSE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilelistDlg message handlers
BOOL CFilelistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_local_list.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	
	//int cmdn=-1;
    
	startsys();
	title.Format("%s",openfilelist[curdir].dir);
	this->SetWindowText(title);
	m_local_list.InsertColumn(0, "名称", LVCFMT_LEFT, 130);
    m_local_list.InsertColumn(1, "大小", LVCFMT_RIGHT, 80);
	m_local_list.InsertColumn(2, "类型", LVCFMT_RIGHT, 200);
    m_local_list.InsertColumn(3, "日期", LVCFMT_RIGHT, 100);
    m_local_list.InsertColumn(4, "时间", LVCFMT_RIGHT, 100);
	
	m_ImageList.Create(22, 22, ILC_MASK | ILC_COLOR32, 2, 0);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDR_FILELITYPE));       //文件图标
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_FOLDER));    //目录图标
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	//设置图像列表和list控件关联
	m_local_list.SetImageList(&m_ImageList, LVSIL_SMALL); 
	
	my_ls();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CFilelistDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString selectType,selectName;
	char *typestr,*namestr;
	int iItem = m_local_list.GetSelectionMark(); 
	
	if (m_local_list.GetSelectedCount() == 0 || m_local_list.GetItemData(iItem) != 1)
		return;
	//MessageBox("双击");
	
	if (iItem != -1)
	{
		if (m_local_list.GetItemData(iItem) == 1)
		{
			selectType = m_local_list.GetItemText(iItem, 2); //获取类型
			selectName = m_local_list.GetItemText(iItem, 0);
		}
	}
	
	typestr=selectType.GetBuffer(0);
	namestr=selectName.GetBuffer(0);
	if(strcmp(typestr,"文件夹")==0 || strcmp(namestr,"root")==0 || strcmp(namestr,"..")==0)
	{

		if (namestr && (openfilelist[curdir].attribute & 0x20))
		{
			my_cd(namestr);
			title.Format("%s",openfilelist[curdir].dir);
			this->SetWindowText(title);	
			my_ls();
		}	
	}
	else
	{
		CFilelistApp *app = (CFilelistApp *)AfxGetApp(); 
		strcpy(app->filenamestr,namestr);
		curdir = my_open(namestr);
		if (!(openfilelist[curdir].attribute & 0x20))
			my_read(curdir, openfilelist[curdir].length);
		else
			MessageBox("No files opened.\n");
		CFileOpen dlg;
		dlg.DoModal();		
	}
	
	*pResult = 0;
}


void CFilelistDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	CMenu menu,*popmenu;
	CPoint point;
	int iItem = m_local_list.GetSelectionMark(); 
	if (m_local_list.GetSelectedCount() == 0 || m_local_list.GetItemData(iItem) != 1)
	{
		menu.LoadMenu(IDR_MENU1);//IDR_MENU_RBUTTONDOWN为菜单资源
		popmenu = menu.GetSubMenu(0);
		GetCursorPos(&point);//point为鼠标位置
		popmenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);//在当前位置呼出菜单
	}
	else
	{
		menu.LoadMenu(IDR_MENU2);
		popmenu = menu.GetSubMenu(0);
		GetCursorPos(&point);
		popmenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	}
	
	*pResult = 0;
}

LRESULT CFilelistDlg::createFile(WPARAM wparam,LPARAM lparam)
{
	CFilelistApp *app = (CFilelistApp *)AfxGetApp(); 
	if (app->name_str && (openfilelist[curdir].attribute & 0x20))
		my_create(app->name_str);
	else
		printf("Please input the right command.\n");
	my_ls();
	//AfxMessageBox(app->name_str);
	return 0;
}

LRESULT CFilelistDlg::createFolder(WPARAM wparam,LPARAM lparam)
{
	CFilelistApp *app = (CFilelistApp *)AfxGetApp(); 
	if ((openfilelist[curdir].attribute & 0x20))
		my_mkdir(app->name_str);
	else
		MessageBox("Please input the right command.");
	my_ls();
	//AfxMessageBox(app->name_str);
	return 0;
}

LRESULT CFilelistDlg::saveFile(WPARAM wparam,LPARAM lparam)
{
	if (!(openfilelist[curdir].attribute & 0x20))
        my_write(curdir);
	else
		MessageBox("No files opened.\n");
	//CFilelistApp *app = (CFilelistApp *)AfxGetApp(); 
	//MessageBox(app->tmptext);
	if (!(openfilelist[curdir].attribute & 0x20))
		curdir = my_close(curdir);
	else
		MessageBox("No files opened.\n");
	my_ls();
	return 0;
}

LRESULT CFilelistDlg::closeFile(WPARAM wparam,LPARAM lparam)
{
	if (!(openfilelist[curdir].attribute & 0x20))
		curdir = my_close(curdir);
	else
		MessageBox("No files opened.\n");
	return 0;
}

void CFilelistDlg::OnRopen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your command handler code here
	OnDblclkList1(pNMHDR,pResult);
}

void CFilelistDlg::OnRdelete() 
{
	// TODO: Add your control notification handler code here
	CString selectType,selectName;
	char typestr[10],namestr[30];
	int iItem = m_local_list.GetSelectionMark(); 
	
	if (m_local_list.GetSelectedCount() == 0 || m_local_list.GetItemData(iItem) != 1)
		return;
	
	if (iItem != -1)
	{
		if (m_local_list.GetItemData(iItem) == 1)
		{
			selectType = m_local_list.GetItemText(iItem, 2); //获取类型
			selectName = m_local_list.GetItemText(iItem, 0);
		}
	}
	
	strcpy(typestr,selectType);
	strcpy(namestr,selectName);
	if(strcmp(typestr,"文件夹")==0)
	{
		if (namestr && (openfilelist[curdir].attribute & 0x20))
		{
			//MessageBox("删除文件夹");
			my_rmdir(namestr);
			//m_local_list.DeleteItem(iItem);
			my_ls();
		}
		
	}
	else
	{
		if (namestr && (openfilelist[curdir].attribute & 0x20))
		{
			my_rm(namestr);	
			//m_local_list.DeleteItem(iItem);
			my_ls();
		}		
	}
	
}

void CFilelistDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (openfilelist[curdir].attribute & 0x20)
	{
		my_exitsys();
	}
	CDialog::OnClose();
}
